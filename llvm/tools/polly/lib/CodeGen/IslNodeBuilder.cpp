//===------ IslNodeBuilder.cpp - Translate an isl AST into a LLVM-IR AST---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the IslNodeBuilder, a class to translate an isl AST into
// a LLVM-IR AST.
//
//===----------------------------------------------------------------------===//

#include "polly/CodeGen/IslNodeBuilder.h"
#include "polly/CodeGen/BlockGenerators.h"
#include "polly/CodeGen/CodeGeneration.h"
#include "polly/CodeGen/IslAst.h"
#include "polly/CodeGen/IslExprBuilder.h"
#include "polly/CodeGen/LoopGenerators.h"
#include "polly/CodeGen/RuntimeDebugBuilder.h"
#include "polly/CodeGen/Utils.h"
#include "polly/Config/config.h"
#include "polly/DependenceInfo.h"
#include "polly/LinkAllPasses.h"
#include "polly/Options.h"
#include "polly/ScopInfo.h"
#include "polly/Support/GICHelper.h"
#include "polly/Support/SCEVValidator.h"
#include "polly/Support/ScopHelper.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "isl/aff.h"
#include "isl/ast.h"
#include "isl/ast_build.h"
#include "isl/list.h"
#include "isl/map.h"
#include "isl/set.h"
#include "isl/union_map.h"
#include "isl/union_set.h"

using namespace polly;
using namespace llvm;

#define DEBUG_TYPE "polly-codegen"

STATISTIC(VersionedScops, "Number of SCoPs that required versioning.");

// The maximal number of dimensions we allow during invariant load construction.
// More complex access ranges will result in very high compile time and are also
// unlikely to result in good code. This value is very high and should only
// trigger for corner cases (e.g., the "dct_luma" function in h264, SPEC2006).
static int const MaxDimensionsInAccessRange = 9;

static cl::opt<bool> PollyGenerateRTCPrint(
    "polly-codegen-emit-rtc-print",
    cl::desc("Emit code that prints the runtime check result dynamically."),
    cl::Hidden, cl::init(false), cl::ZeroOrMore, cl::cat(PollyCategory));

// If this option is set we always use the isl AST generator to regenerate
// memory accesses. Without this option set we regenerate expressions using the
// original SCEV expressions and only generate new expressions in case the
// access relation has been changed and consequently must be regenerated.
static cl::opt<bool> PollyGenerateExpressions(
    "polly-codegen-generate-expressions",
    cl::desc("Generate AST expressions for unmodified and modified accesses"),
    cl::Hidden, cl::init(false), cl::ZeroOrMore, cl::cat(PollyCategory));

static cl::opt<int> PollyTargetFirstLevelCacheLineSize(
    "polly-target-first-level-cache-line-size",
    cl::desc("The size of the first level cache line size specified in bytes."),
    cl::Hidden, cl::init(64), cl::ZeroOrMore, cl::cat(PollyCategory));

__isl_give isl_ast_expr *
IslNodeBuilder::getUpperBound(__isl_keep isl_ast_node *For,
                              ICmpInst::Predicate &Predicate) {
  isl_id *UBID, *IteratorID;
  isl_ast_expr *Cond, *Iterator, *UB, *Arg0;
  isl_ast_op_type Type;

  Cond = isl_ast_node_for_get_cond(For);
  Iterator = isl_ast_node_for_get_iterator(For);
  isl_ast_expr_get_type(Cond);
  assert(isl_ast_expr_get_type(Cond) == isl_ast_expr_op &&
         "conditional expression is not an atomic upper bound");

  Type = isl_ast_expr_get_op_type(Cond);

  switch (Type) {
  case isl_ast_op_le:
    Predicate = ICmpInst::ICMP_SLE;
    break;
  case isl_ast_op_lt:
    Predicate = ICmpInst::ICMP_SLT;
    break;
  default:
    llvm_unreachable("Unexpected comparision type in loop conditon");
  }

  Arg0 = isl_ast_expr_get_op_arg(Cond, 0);

  assert(isl_ast_expr_get_type(Arg0) == isl_ast_expr_id &&
         "conditional expression is not an atomic upper bound");

  UBID = isl_ast_expr_get_id(Arg0);

  assert(isl_ast_expr_get_type(Iterator) == isl_ast_expr_id &&
         "Could not get the iterator");

  IteratorID = isl_ast_expr_get_id(Iterator);

  assert(UBID == IteratorID &&
         "conditional expression is not an atomic upper bound");

  UB = isl_ast_expr_get_op_arg(Cond, 1);

  isl_ast_expr_free(Cond);
  isl_ast_expr_free(Iterator);
  isl_ast_expr_free(Arg0);
  isl_id_free(IteratorID);
  isl_id_free(UBID);

  return UB;
}

/// Return true if a return value of Predicate is true for the value represented
/// by passed isl_ast_expr_int.
static bool checkIslAstExprInt(__isl_take isl_ast_expr *Expr,
                               isl_bool (*Predicate)(__isl_keep isl_val *)) {
  if (isl_ast_expr_get_type(Expr) != isl_ast_expr_int) {
    isl_ast_expr_free(Expr);
    return false;
  }
  auto ExprVal = isl_ast_expr_get_val(Expr);
  isl_ast_expr_free(Expr);
  if (Predicate(ExprVal) != true) {
    isl_val_free(ExprVal);
    return false;
  }
  isl_val_free(ExprVal);
  return true;
}

int IslNodeBuilder::getNumberOfIterations(__isl_keep isl_ast_node *For) {
  assert(isl_ast_node_get_type(For) == isl_ast_node_for);
  auto Body = isl_ast_node_for_get_body(For);

  // First, check if we can actually handle this code.
  switch (isl_ast_node_get_type(Body)) {
  case isl_ast_node_user:
    break;
  case isl_ast_node_block: {
    isl_ast_node_list *List = isl_ast_node_block_get_children(Body);
    for (int i = 0; i < isl_ast_node_list_n_ast_node(List); ++i) {
      isl_ast_node *Node = isl_ast_node_list_get_ast_node(List, i);
      int Type = isl_ast_node_get_type(Node);
      isl_ast_node_free(Node);
      if (Type != isl_ast_node_user) {
        isl_ast_node_list_free(List);
        isl_ast_node_free(Body);
        return -1;
      }
    }
    isl_ast_node_list_free(List);
    break;
  }
  default:
    isl_ast_node_free(Body);
    return -1;
  }
  isl_ast_node_free(Body);

  auto Init = isl_ast_node_for_get_init(For);
  if (!checkIslAstExprInt(Init, isl_val_is_zero))
    return -1;
  auto Inc = isl_ast_node_for_get_inc(For);
  if (!checkIslAstExprInt(Inc, isl_val_is_one))
    return -1;
  CmpInst::Predicate Predicate;
  auto UB = getUpperBound(For, Predicate);
  if (isl_ast_expr_get_type(UB) != isl_ast_expr_int) {
    isl_ast_expr_free(UB);
    return -1;
  }
  auto UpVal = isl_ast_expr_get_val(UB);
  isl_ast_expr_free(UB);
  int NumberIterations = isl_val_get_num_si(UpVal);
  isl_val_free(UpVal);
  if (NumberIterations < 0)
    return -1;
  if (Predicate == CmpInst::ICMP_SLT)
    return NumberIterations;
  else
    return NumberIterations + 1;
}

/// Extract the values and SCEVs needed to generate code for a block.
static int findReferencesInBlock(struct SubtreeReferences &References,
                                 const ScopStmt *Stmt, const BasicBlock *BB) {
  for (const Instruction &Inst : *BB)
    for (Value *SrcVal : Inst.operands()) {
      auto *Scope = References.LI.getLoopFor(BB);
      if (canSynthesize(SrcVal, References.S, &References.SE, Scope)) {
        References.SCEVs.insert(References.SE.getSCEVAtScope(SrcVal, Scope));
        continue;
      } else if (Value *NewVal = References.GlobalMap.lookup(SrcVal))
        References.Values.insert(NewVal);
    }
  return 0;
}

isl_stat addReferencesFromStmt(const ScopStmt *Stmt, void *UserPtr,
                               bool CreateScalarRefs) {
  auto &References = *static_cast<struct SubtreeReferences *>(UserPtr);

  if (Stmt->isBlockStmt())
    findReferencesInBlock(References, Stmt, Stmt->getBasicBlock());
  else {
    assert(Stmt->isRegionStmt() &&
           "Stmt was neither block nor region statement");
    for (const BasicBlock *BB : Stmt->getRegion()->blocks())
      findReferencesInBlock(References, Stmt, BB);
  }

  for (auto &Access : *Stmt) {
    if (Access->isArrayKind()) {
      auto *BasePtr = Access->getScopArrayInfo()->getBasePtr();
      if (Instruction *OpInst = dyn_cast<Instruction>(BasePtr))
        if (Stmt->getParent()->contains(OpInst))
          continue;

      References.Values.insert(BasePtr);
      continue;
    }

    if (CreateScalarRefs)
      References.Values.insert(References.BlockGen.getOrCreateAlloca(*Access));
  }

  return isl_stat_ok;
}

/// Extract the out-of-scop values and SCEVs referenced from a set describing
/// a ScopStmt.
///
/// This includes the SCEVUnknowns referenced by the SCEVs used in the
/// statement and the base pointers of the memory accesses. For scalar
/// statements we force the generation of alloca memory locations and list
/// these locations in the set of out-of-scop values as well.
///
/// @param Set     A set which references the ScopStmt we are interested in.
/// @param UserPtr A void pointer that can be casted to a SubtreeReferences
///                structure.
static isl_stat addReferencesFromStmtSet(__isl_take isl_set *Set,
                                         void *UserPtr) {
  isl_id *Id = isl_set_get_tuple_id(Set);
  auto *Stmt = static_cast<const ScopStmt *>(isl_id_get_user(Id));
  isl_id_free(Id);
  isl_set_free(Set);
  return addReferencesFromStmt(Stmt, UserPtr);
}

/// Extract the out-of-scop values and SCEVs referenced from a union set
/// referencing multiple ScopStmts.
///
/// This includes the SCEVUnknowns referenced by the SCEVs used in the
/// statement and the base pointers of the memory accesses. For scalar
/// statements we force the generation of alloca memory locations and list
/// these locations in the set of out-of-scop values as well.
///
/// @param USet       A union set referencing the ScopStmts we are interested
///                   in.
/// @param References The SubtreeReferences data structure through which
///                   results are returned and further information is
///                   provided.
static void
addReferencesFromStmtUnionSet(isl_union_set *USet,
                              struct SubtreeReferences &References) {
  isl_union_set_foreach_set(USet, addReferencesFromStmtSet, &References);
  isl_union_set_free(USet);
}

__isl_give isl_union_map *
IslNodeBuilder::getScheduleForAstNode(__isl_keep isl_ast_node *For) {
  return IslAstInfo::getSchedule(For);
}

void IslNodeBuilder::getReferencesInSubtree(__isl_keep isl_ast_node *For,
                                            SetVector<Value *> &Values,
                                            SetVector<const Loop *> &Loops) {

  SetVector<const SCEV *> SCEVs;
  struct SubtreeReferences References = {
      LI, SE, S, ValueMap, Values, SCEVs, getBlockGenerator()};

  for (const auto &I : IDToValue)
    Values.insert(I.second);

  for (const auto &I : OutsideLoopIterations)
    Values.insert(cast<SCEVUnknown>(I.second)->getValue());

  isl_union_set *Schedule = isl_union_map_domain(getScheduleForAstNode(For));
  addReferencesFromStmtUnionSet(Schedule, References);

  for (const SCEV *Expr : SCEVs) {
    findValues(Expr, SE, Values);
    findLoops(Expr, Loops);
  }

  Values.remove_if([](const Value *V) { return isa<GlobalValue>(V); });

  /// Remove loops that contain the scop or that are part of the scop, as they
  /// are considered local. This leaves only loops that are before the scop, but
  /// do not contain the scop itself.
  Loops.remove_if([this](const Loop *L) {
    return S.contains(L) || L->contains(S.getEntry());
  });
}

void IslNodeBuilder::updateValues(ValueMapT &NewValues) {
  SmallPtrSet<Value *, 5> Inserted;

  for (const auto &I : IDToValue) {
    IDToValue[I.first] = NewValues[I.second];
    Inserted.insert(I.second);
  }

  for (const auto &I : NewValues) {
    if (Inserted.count(I.first))
      continue;

    ValueMap[I.first] = I.second;
  }
}

void IslNodeBuilder::createUserVector(__isl_take isl_ast_node *User,
                                      std::vector<Value *> &IVS,
                                      __isl_take isl_id *IteratorID,
                                      __isl_take isl_union_map *Schedule) {
  isl_ast_expr *Expr = isl_ast_node_user_get_expr(User);
  isl_ast_expr *StmtExpr = isl_ast_expr_get_op_arg(Expr, 0);
  isl_id *Id = isl_ast_expr_get_id(StmtExpr);
  isl_ast_expr_free(StmtExpr);
  ScopStmt *Stmt = (ScopStmt *)isl_id_get_user(Id);
  std::vector<LoopToScevMapT> VLTS(IVS.size());

  isl_union_set *Domain = isl_union_set_from_set(Stmt->getDomain());
  Schedule = isl_union_map_intersect_domain(Schedule, Domain);
  isl_map *S = isl_map_from_union_map(Schedule);

  auto *NewAccesses = createNewAccesses(Stmt, User);
  createSubstitutionsVector(Expr, Stmt, VLTS, IVS, IteratorID);
  VectorBlockGenerator::generate(BlockGen, *Stmt, VLTS, S, NewAccesses);
  isl_id_to_ast_expr_free(NewAccesses);
  isl_map_free(S);
  isl_id_free(Id);
  isl_ast_node_free(User);
}

void IslNodeBuilder::createMark(__isl_take isl_ast_node *Node) {
  auto *Id = isl_ast_node_mark_get_id(Node);
  auto Child = isl_ast_node_mark_get_node(Node);
  isl_ast_node_free(Node);
  // If a child node of a 'SIMD mark' is a loop that has a single iteration,
  // it will be optimized away and we should skip it.
  if (!strcmp(isl_id_get_name(Id), "SIMD") &&
      isl_ast_node_get_type(Child) == isl_ast_node_for) {
    bool Vector = PollyVectorizerChoice == VECTORIZER_POLLY;
    int VectorWidth = getNumberOfIterations(Child);
    if (Vector && 1 < VectorWidth && VectorWidth <= 16)
      createForVector(Child, VectorWidth);
    else
      createForSequential(Child, true);
    isl_id_free(Id);
    return;
  }
  create(Child);
  isl_id_free(Id);
}

void IslNodeBuilder::createForVector(__isl_take isl_ast_node *For,
                                     int VectorWidth) {
  isl_ast_node *Body = isl_ast_node_for_get_body(For);
  isl_ast_expr *Init = isl_ast_node_for_get_init(For);
  isl_ast_expr *Inc = isl_ast_node_for_get_inc(For);
  isl_ast_expr *Iterator = isl_ast_node_for_get_iterator(For);
  isl_id *IteratorID = isl_ast_expr_get_id(Iterator);

  Value *ValueLB = ExprBuilder.create(Init);
  Value *ValueInc = ExprBuilder.create(Inc);

  Type *MaxType = ExprBuilder.getType(Iterator);
  MaxType = ExprBuilder.getWidestType(MaxType, ValueLB->getType());
  MaxType = ExprBuilder.getWidestType(MaxType, ValueInc->getType());

  if (MaxType != ValueLB->getType())
    ValueLB = Builder.CreateSExt(ValueLB, MaxType);
  if (MaxType != ValueInc->getType())
    ValueInc = Builder.CreateSExt(ValueInc, MaxType);

  std::vector<Value *> IVS(VectorWidth);
  IVS[0] = ValueLB;

  for (int i = 1; i < VectorWidth; i++)
    IVS[i] = Builder.CreateAdd(IVS[i - 1], ValueInc, "p_vector_iv");

  isl_union_map *Schedule = getScheduleForAstNode(For);
  assert(Schedule && "For statement annotation does not contain its schedule");

  IDToValue[IteratorID] = ValueLB;

  switch (isl_ast_node_get_type(Body)) {
  case isl_ast_node_user:
    createUserVector(Body, IVS, isl_id_copy(IteratorID),
                     isl_union_map_copy(Schedule));
    break;
  case isl_ast_node_block: {
    isl_ast_node_list *List = isl_ast_node_block_get_children(Body);

    for (int i = 0; i < isl_ast_node_list_n_ast_node(List); ++i)
      createUserVector(isl_ast_node_list_get_ast_node(List, i), IVS,
                       isl_id_copy(IteratorID), isl_union_map_copy(Schedule));

    isl_ast_node_free(Body);
    isl_ast_node_list_free(List);
    break;
  }
  default:
    isl_ast_node_dump(Body);
    llvm_unreachable("Unhandled isl_ast_node in vectorizer");
  }

  IDToValue.erase(IDToValue.find(IteratorID));
  isl_id_free(IteratorID);
  isl_union_map_free(Schedule);

  isl_ast_node_free(For);
  isl_ast_expr_free(Iterator);
}

void IslNodeBuilder::createForSequential(__isl_take isl_ast_node *For,
                                         bool KnownParallel) {
  isl_ast_node *Body;
  isl_ast_expr *Init, *Inc, *Iterator, *UB;
  isl_id *IteratorID;
  Value *ValueLB, *ValueUB, *ValueInc;
  Type *MaxType;
  BasicBlock *ExitBlock;
  Value *IV;
  CmpInst::Predicate Predicate;
  bool Parallel;

  Parallel = KnownParallel || (IslAstInfo::isParallel(For) &&
                               !IslAstInfo::isReductionParallel(For));

  Body = isl_ast_node_for_get_body(For);

  // isl_ast_node_for_is_degenerate(For)
  //
  // TODO: For degenerated loops we could generate a plain assignment.
  //       However, for now we just reuse the logic for normal loops, which will
  //       create a loop with a single iteration.

  Init = isl_ast_node_for_get_init(For);
  Inc = isl_ast_node_for_get_inc(For);
  Iterator = isl_ast_node_for_get_iterator(For);
  IteratorID = isl_ast_expr_get_id(Iterator);
  UB = getUpperBound(For, Predicate);

  ValueLB = ExprBuilder.create(Init);
  ValueUB = ExprBuilder.create(UB);
  ValueInc = ExprBuilder.create(Inc);

  MaxType = ExprBuilder.getType(Iterator);
  MaxType = ExprBuilder.getWidestType(MaxType, ValueLB->getType());
  MaxType = ExprBuilder.getWidestType(MaxType, ValueUB->getType());
  MaxType = ExprBuilder.getWidestType(MaxType, ValueInc->getType());

  if (MaxType != ValueLB->getType())
    ValueLB = Builder.CreateSExt(ValueLB, MaxType);
  if (MaxType != ValueUB->getType())
    ValueUB = Builder.CreateSExt(ValueUB, MaxType);
  if (MaxType != ValueInc->getType())
    ValueInc = Builder.CreateSExt(ValueInc, MaxType);

  // If we can show that LB <Predicate> UB holds at least once, we can
  // omit the GuardBB in front of the loop.
  bool UseGuardBB =
      !SE.isKnownPredicate(Predicate, SE.getSCEV(ValueLB), SE.getSCEV(ValueUB));
  IV = createLoop(ValueLB, ValueUB, ValueInc, Builder, P, LI, DT, ExitBlock,
                  Predicate, &Annotator, Parallel, UseGuardBB);
  IDToValue[IteratorID] = IV;

  create(Body);

  Annotator.popLoop(Parallel);

  IDToValue.erase(IDToValue.find(IteratorID));

  Builder.SetInsertPoint(&ExitBlock->front());

  isl_ast_node_free(For);
  isl_ast_expr_free(Iterator);
  isl_id_free(IteratorID);
}

/// Remove the BBs contained in a (sub)function from the dominator tree.
///
/// This function removes the basic blocks that are part of a subfunction from
/// the dominator tree. Specifically, when generating code it may happen that at
/// some point the code generation continues in a new sub-function (e.g., when
/// generating OpenMP code). The basic blocks that are created in this
/// sub-function are then still part of the dominator tree of the original
/// function, such that the dominator tree reaches over function boundaries.
/// This is not only incorrect, but also causes crashes. This function now
/// removes from the dominator tree all basic blocks that are dominated (and
/// consequently reachable) from the entry block of this (sub)function.
///
/// FIXME: A LLVM (function or region) pass should not touch anything outside of
/// the function/region it runs on. Hence, the pure need for this function shows
/// that we do not comply to this rule. At the moment, this does not cause any
/// issues, but we should be aware that such issues may appear. Unfortunately
/// the current LLVM pass infrastructure does not allow to make Polly a module
/// or call-graph pass to solve this issue, as such a pass would not have access
/// to the per-function analyses passes needed by Polly. A future pass manager
/// infrastructure is supposed to enable such kind of access possibly allowing
/// us to create a cleaner solution here.
///
/// FIXME: Instead of adding the dominance information and then dropping it
/// later on, we should try to just not add it in the first place. This requires
/// some careful testing to make sure this does not break in interaction with
/// the SCEVBuilder and SplitBlock which may rely on the dominator tree or
/// which may try to update it.
///
/// @param F The function which contains the BBs to removed.
/// @param DT The dominator tree from which to remove the BBs.
static void removeSubFuncFromDomTree(Function *F, DominatorTree &DT) {
  DomTreeNode *N = DT.getNode(&F->getEntryBlock());
  std::vector<BasicBlock *> Nodes;

  // We can only remove an element from the dominator tree, if all its children
  // have been removed. To ensure this we obtain the list of nodes to remove
  // using a post-order tree traversal.
  for (po_iterator<DomTreeNode *> I = po_begin(N), E = po_end(N); I != E; ++I)
    Nodes.push_back(I->getBlock());

  for (BasicBlock *BB : Nodes)
    DT.eraseNode(BB);
}

void IslNodeBuilder::createForParallel(__isl_take isl_ast_node *For) {
  isl_ast_node *Body;
  isl_ast_expr *Init, *Inc, *Iterator, *UB;
  isl_id *IteratorID;
  Value *ValueLB, *ValueUB, *ValueInc;
  Type *MaxType;
  Value *IV;
  CmpInst::Predicate Predicate;

  // The preamble of parallel code interacts different than normal code with
  // e.g., scalar initialization. Therefore, we ensure the parallel code is
  // separated from the last basic block.
  BasicBlock *ParBB = SplitBlock(Builder.GetInsertBlock(),
                                 &*Builder.GetInsertPoint(), &DT, &LI);
  ParBB->setName("polly.parallel.for");
  Builder.SetInsertPoint(&ParBB->front());

  Body = isl_ast_node_for_get_body(For);
  Init = isl_ast_node_for_get_init(For);
  Inc = isl_ast_node_for_get_inc(For);
  Iterator = isl_ast_node_for_get_iterator(For);
  IteratorID = isl_ast_expr_get_id(Iterator);
  UB = getUpperBound(For, Predicate);

  ValueLB = ExprBuilder.create(Init);
  ValueUB = ExprBuilder.create(UB);
  ValueInc = ExprBuilder.create(Inc);

  // OpenMP always uses SLE. In case the isl generated AST uses a SLT
  // expression, we need to adjust the loop blound by one.
  if (Predicate == CmpInst::ICMP_SLT)
    ValueUB = Builder.CreateAdd(
        ValueUB, Builder.CreateSExt(Builder.getTrue(), ValueUB->getType()));

  MaxType = ExprBuilder.getType(Iterator);
  MaxType = ExprBuilder.getWidestType(MaxType, ValueLB->getType());
  MaxType = ExprBuilder.getWidestType(MaxType, ValueUB->getType());
  MaxType = ExprBuilder.getWidestType(MaxType, ValueInc->getType());

  if (MaxType != ValueLB->getType())
    ValueLB = Builder.CreateSExt(ValueLB, MaxType);
  if (MaxType != ValueUB->getType())
    ValueUB = Builder.CreateSExt(ValueUB, MaxType);
  if (MaxType != ValueInc->getType())
    ValueInc = Builder.CreateSExt(ValueInc, MaxType);

  BasicBlock::iterator LoopBody;

  SetVector<Value *> SubtreeValues;
  SetVector<const Loop *> Loops;

  getReferencesInSubtree(For, SubtreeValues, Loops);

  // Create for all loops we depend on values that contain the current loop
  // iteration. These values are necessary to generate code for SCEVs that
  // depend on such loops. As a result we need to pass them to the subfunction.
  for (const Loop *L : Loops) {
    const SCEV *OuterLIV = SE.getAddRecExpr(SE.getUnknown(Builder.getInt64(0)),
                                            SE.getUnknown(Builder.getInt64(1)),
                                            L, SCEV::FlagAnyWrap);
    Value *V = generateSCEV(OuterLIV);
    OutsideLoopIterations[L] = SE.getUnknown(V);
    SubtreeValues.insert(V);
  }

  ValueMapT NewValues;
  ParallelLoopGenerator ParallelLoopGen(Builder, P, LI, DT, DL);

  IV = ParallelLoopGen.createParallelLoop(ValueLB, ValueUB, ValueInc,
                                          SubtreeValues, NewValues, &LoopBody);
  BasicBlock::iterator AfterLoop = Builder.GetInsertPoint();
  Builder.SetInsertPoint(&*LoopBody);

  // Remember the parallel subfunction
  ParallelSubfunctions.push_back(LoopBody->getFunction());

  // Save the current values.
  auto ValueMapCopy = ValueMap;
  IslExprBuilder::IDToValueTy IDToValueCopy = IDToValue;

  updateValues(NewValues);
  IDToValue[IteratorID] = IV;

  ValueMapT NewValuesReverse;

  for (auto P : NewValues)
    NewValuesReverse[P.second] = P.first;

  Annotator.addAlternativeAliasBases(NewValuesReverse);

  create(Body);

  Annotator.resetAlternativeAliasBases();
  // Restore the original values.
  ValueMap = ValueMapCopy;
  IDToValue = IDToValueCopy;

  Builder.SetInsertPoint(&*AfterLoop);
  removeSubFuncFromDomTree((*LoopBody).getParent()->getParent(), DT);

  for (const Loop *L : Loops)
    OutsideLoopIterations.erase(L);

  isl_ast_node_free(For);
  isl_ast_expr_free(Iterator);
  isl_id_free(IteratorID);
}

void IslNodeBuilder::createFor(__isl_take isl_ast_node *For) {
  bool Vector = PollyVectorizerChoice == VECTORIZER_POLLY;

  if (Vector && IslAstInfo::isInnermostParallel(For) &&
      !IslAstInfo::isReductionParallel(For)) {
    int VectorWidth = getNumberOfIterations(For);
    if (1 < VectorWidth && VectorWidth <= 16) {
      createForVector(For, VectorWidth);
      return;
    }
  }

  if (IslAstInfo::isExecutedInParallel(For)) {
    createForParallel(For);
    return;
  }
  createForSequential(For, false);
}

void IslNodeBuilder::createIf(__isl_take isl_ast_node *If) {
  isl_ast_expr *Cond = isl_ast_node_if_get_cond(If);

  Function *F = Builder.GetInsertBlock()->getParent();
  LLVMContext &Context = F->getContext();

  BasicBlock *CondBB = SplitBlock(Builder.GetInsertBlock(),
                                  &*Builder.GetInsertPoint(), &DT, &LI);
  CondBB->setName("polly.cond");
  BasicBlock *MergeBB = SplitBlock(CondBB, &CondBB->front(), &DT, &LI);
  MergeBB->setName("polly.merge");
  BasicBlock *ThenBB = BasicBlock::Create(Context, "polly.then", F);
  BasicBlock *ElseBB = BasicBlock::Create(Context, "polly.else", F);

  DT.addNewBlock(ThenBB, CondBB);
  DT.addNewBlock(ElseBB, CondBB);
  DT.changeImmediateDominator(MergeBB, CondBB);

  Loop *L = LI.getLoopFor(CondBB);
  if (L) {
    L->addBasicBlockToLoop(ThenBB, LI);
    L->addBasicBlockToLoop(ElseBB, LI);
  }

  CondBB->getTerminator()->eraseFromParent();

  Builder.SetInsertPoint(CondBB);
  Value *Predicate = ExprBuilder.create(Cond);
  Builder.CreateCondBr(Predicate, ThenBB, ElseBB);
  Builder.SetInsertPoint(ThenBB);
  Builder.CreateBr(MergeBB);
  Builder.SetInsertPoint(ElseBB);
  Builder.CreateBr(MergeBB);
  Builder.SetInsertPoint(&ThenBB->front());

  create(isl_ast_node_if_get_then(If));

  Builder.SetInsertPoint(&ElseBB->front());

  if (isl_ast_node_if_has_else(If))
    create(isl_ast_node_if_get_else(If));

  Builder.SetInsertPoint(&MergeBB->front());

  isl_ast_node_free(If);
}

__isl_give isl_id_to_ast_expr *
IslNodeBuilder::createNewAccesses(ScopStmt *Stmt,
                                  __isl_keep isl_ast_node *Node) {
  isl_id_to_ast_expr *NewAccesses =
      isl_id_to_ast_expr_alloc(Stmt->getParent()->getIslCtx(), 0);

  auto *Build = IslAstInfo::getBuild(Node);
  assert(Build && "Could not obtain isl_ast_build from user node");
  Stmt->setAstBuild(Build);

  for (auto *MA : *Stmt) {
    if (!MA->hasNewAccessRelation()) {
      if (PollyGenerateExpressions) {
        if (!MA->isAffine())
          continue;
        if (MA->getLatestScopArrayInfo()->getBasePtrOriginSAI())
          continue;

        auto *BasePtr =
            dyn_cast<Instruction>(MA->getLatestScopArrayInfo()->getBasePtr());
        if (BasePtr && Stmt->getParent()->getRegion().contains(BasePtr))
          continue;
      } else {
        continue;
      }
    }
    assert(MA->isAffine() &&
           "Only affine memory accesses can be code generated");

    auto Schedule = isl_ast_build_get_schedule(Build);

#ifndef NDEBUG
    auto Dom = Stmt->getDomain();
    auto SchedDom = isl_set_from_union_set(
        isl_union_map_domain(isl_union_map_copy(Schedule)));
    auto AccDom = isl_map_domain(MA->getAccessRelation());
    Dom = isl_set_intersect_params(Dom, Stmt->getParent()->getContext());
    SchedDom =
        isl_set_intersect_params(SchedDom, Stmt->getParent()->getContext());
    assert(isl_set_is_subset(SchedDom, AccDom) &&
           "Access relation not defined on full schedule domain");
    assert(isl_set_is_subset(Dom, AccDom) &&
           "Access relation not defined on full domain");
    isl_set_free(AccDom);
    isl_set_free(SchedDom);
    isl_set_free(Dom);
#endif

    auto PWAccRel = MA->applyScheduleToAccessRelation(Schedule);

    auto AccessExpr = isl_ast_build_access_from_pw_multi_aff(Build, PWAccRel);
    NewAccesses = isl_id_to_ast_expr_set(NewAccesses, MA->getId(), AccessExpr);
  }

  return NewAccesses;
}

void IslNodeBuilder::createSubstitutions(__isl_take isl_ast_expr *Expr,
                                         ScopStmt *Stmt, LoopToScevMapT &LTS) {
  assert(isl_ast_expr_get_type(Expr) == isl_ast_expr_op &&
         "Expression of type 'op' expected");
  assert(isl_ast_expr_get_op_type(Expr) == isl_ast_op_call &&
         "Opertation of type 'call' expected");
  for (int i = 0; i < isl_ast_expr_get_op_n_arg(Expr) - 1; ++i) {
    isl_ast_expr *SubExpr;
    Value *V;

    SubExpr = isl_ast_expr_get_op_arg(Expr, i + 1);
    V = ExprBuilder.create(SubExpr);
    ScalarEvolution *SE = Stmt->getParent()->getSE();
    LTS[Stmt->getLoopForDimension(i)] = SE->getUnknown(V);
  }

  isl_ast_expr_free(Expr);
}

void IslNodeBuilder::createSubstitutionsVector(
    __isl_take isl_ast_expr *Expr, ScopStmt *Stmt,
    std::vector<LoopToScevMapT> &VLTS, std::vector<Value *> &IVS,
    __isl_take isl_id *IteratorID) {
  int i = 0;

  Value *OldValue = IDToValue[IteratorID];
  for (Value *IV : IVS) {
    IDToValue[IteratorID] = IV;
    createSubstitutions(isl_ast_expr_copy(Expr), Stmt, VLTS[i]);
    i++;
  }

  IDToValue[IteratorID] = OldValue;
  isl_id_free(IteratorID);
  isl_ast_expr_free(Expr);
}

void IslNodeBuilder::generateCopyStmt(
    ScopStmt *Stmt, __isl_keep isl_id_to_ast_expr *NewAccesses) {
  assert(Stmt->size() == 2);
  auto ReadAccess = Stmt->begin();
  auto WriteAccess = ReadAccess++;
  assert((*ReadAccess)->isRead() && (*WriteAccess)->isMustWrite());
  assert((*ReadAccess)->getElementType() == (*WriteAccess)->getElementType() &&
         "Accesses use the same data type");
  assert((*ReadAccess)->isArrayKind() && (*WriteAccess)->isArrayKind());
  auto *AccessExpr =
      isl_id_to_ast_expr_get(NewAccesses, (*ReadAccess)->getId());
  auto *LoadValue = ExprBuilder.create(AccessExpr);
  AccessExpr = isl_id_to_ast_expr_get(NewAccesses, (*WriteAccess)->getId());
  auto *StoreAddr = ExprBuilder.createAccessAddress(AccessExpr);
  Builder.CreateStore(LoadValue, StoreAddr);
}

void IslNodeBuilder::createUser(__isl_take isl_ast_node *User) {
  LoopToScevMapT LTS;
  isl_id *Id;
  ScopStmt *Stmt;

  isl_ast_expr *Expr = isl_ast_node_user_get_expr(User);
  isl_ast_expr *StmtExpr = isl_ast_expr_get_op_arg(Expr, 0);
  Id = isl_ast_expr_get_id(StmtExpr);
  isl_ast_expr_free(StmtExpr);

  LTS.insert(OutsideLoopIterations.begin(), OutsideLoopIterations.end());

  Stmt = (ScopStmt *)isl_id_get_user(Id);
  auto *NewAccesses = createNewAccesses(Stmt, User);
  if (Stmt->isCopyStmt()) {
    generateCopyStmt(Stmt, NewAccesses);
    isl_ast_expr_free(Expr);
  } else {
    createSubstitutions(Expr, Stmt, LTS);

    if (Stmt->isBlockStmt())
      BlockGen.copyStmt(*Stmt, LTS, NewAccesses);
    else
      RegionGen.copyStmt(*Stmt, LTS, NewAccesses);
  }

  isl_id_to_ast_expr_free(NewAccesses);
  isl_ast_node_free(User);
  isl_id_free(Id);
}

void IslNodeBuilder::createBlock(__isl_take isl_ast_node *Block) {
  isl_ast_node_list *List = isl_ast_node_block_get_children(Block);

  for (int i = 0; i < isl_ast_node_list_n_ast_node(List); ++i)
    create(isl_ast_node_list_get_ast_node(List, i));

  isl_ast_node_free(Block);
  isl_ast_node_list_free(List);
}

void IslNodeBuilder::create(__isl_take isl_ast_node *Node) {
  switch (isl_ast_node_get_type(Node)) {
  case isl_ast_node_error:
    llvm_unreachable("code generation error");
  case isl_ast_node_mark:
    createMark(Node);
    return;
  case isl_ast_node_for:
    createFor(Node);
    return;
  case isl_ast_node_if:
    createIf(Node);
    return;
  case isl_ast_node_user:
    createUser(Node);
    return;
  case isl_ast_node_block:
    createBlock(Node);
    return;
  }

  llvm_unreachable("Unknown isl_ast_node type");
}

bool IslNodeBuilder::materializeValue(isl_id *Id) {
  // If the Id is already mapped, skip it.
  if (!IDToValue.count(Id)) {
    auto *ParamSCEV = (const SCEV *)isl_id_get_user(Id);
    Value *V = nullptr;

    // Parameters could refere to invariant loads that need to be
    // preloaded before we can generate code for the parameter. Thus,
    // check if any value refered to in ParamSCEV is an invariant load
    // and if so make sure its equivalence class is preloaded.
    SetVector<Value *> Values;
    findValues(ParamSCEV, SE, Values);
    for (auto *Val : Values) {

      // Check if the value is an instruction in a dead block within the SCoP
      // and if so do not code generate it.
      if (auto *Inst = dyn_cast<Instruction>(Val)) {
        if (S.contains(Inst)) {
          bool IsDead = true;

          // Check for "undef" loads first, then if there is a statement for
          // the parent of Inst and lastly if the parent of Inst has an empty
          // domain. In the first and last case the instruction is dead but if
          // there is a statement or the domain is not empty Inst is not dead.
          auto MemInst = MemAccInst::dyn_cast(Inst);
          auto Address = MemInst ? MemInst.getPointerOperand() : nullptr;
          if (Address && SE.getUnknown(UndefValue::get(Address->getType())) ==
                             SE.getPointerBase(SE.getSCEV(Address))) {
          } else if (S.getStmtFor(Inst)) {
            IsDead = false;
          } else {
            auto *Domain = S.getDomainConditions(Inst->getParent());
            IsDead = isl_set_is_empty(Domain);
            isl_set_free(Domain);
          }

          if (IsDead) {
            V = UndefValue::get(ParamSCEV->getType());
            break;
          }
        }
      }

      if (auto *IAClass = S.lookupInvariantEquivClass(Val)) {

        // Check if this invariant access class is empty, hence if we never
        // actually added a loads instruction to it. In that case it has no
        // (meaningful) users and we should not try to code generate it.
        if (IAClass->InvariantAccesses.empty())
          V = UndefValue::get(ParamSCEV->getType());

        if (!preloadInvariantEquivClass(*IAClass)) {
          isl_id_free(Id);
          return false;
        }
      }
    }

    V = V ? V : generateSCEV(ParamSCEV);
    IDToValue[Id] = V;
  }

  isl_id_free(Id);
  return true;
}

bool IslNodeBuilder::materializeParameters(isl_set *Set, bool All) {
  for (unsigned i = 0, e = isl_set_dim(Set, isl_dim_param); i < e; ++i) {
    if (!All && !isl_set_involves_dims(Set, isl_dim_param, i, 1))
      continue;
    isl_id *Id = isl_set_get_dim_id(Set, isl_dim_param, i);
    if (!materializeValue(Id))
      return false;
  }
  return true;
}

/// Add the number of dimensions in @p BS to @p U.
static isl_stat countTotalDims(__isl_take isl_basic_set *BS, void *U) {
  unsigned *NumTotalDim = static_cast<unsigned *>(U);
  *NumTotalDim += isl_basic_set_total_dim(BS);
  isl_basic_set_free(BS);
  return isl_stat_ok;
}

Value *IslNodeBuilder::preloadUnconditionally(isl_set *AccessRange,
                                              isl_ast_build *Build,
                                              Instruction *AccInst) {

  // TODO: This check could be performed in the ScopInfo already.
  unsigned NumTotalDim = 0;
  isl_set_foreach_basic_set(AccessRange, countTotalDims, &NumTotalDim);
  if (NumTotalDim > MaxDimensionsInAccessRange) {
    isl_set_free(AccessRange);
    return nullptr;
  }

  isl_pw_multi_aff *PWAccRel = isl_pw_multi_aff_from_set(AccessRange);
  isl_ast_expr *Access =
      isl_ast_build_access_from_pw_multi_aff(Build, PWAccRel);
  auto *Address = isl_ast_expr_address_of(Access);
  auto *AddressValue = ExprBuilder.create(Address);
  Value *PreloadVal;

  // Correct the type as the SAI might have a different type than the user
  // expects, especially if the base pointer is a struct.
  Type *Ty = AccInst->getType();

  auto *Ptr = AddressValue;
  auto Name = Ptr->getName();
  Ptr = Builder.CreatePointerCast(Ptr, Ty->getPointerTo(), Name + ".cast");
  PreloadVal = Builder.CreateLoad(Ptr, Name + ".load");
  if (LoadInst *PreloadInst = dyn_cast<LoadInst>(PreloadVal))
    PreloadInst->setAlignment(dyn_cast<LoadInst>(AccInst)->getAlignment());

  // TODO: This is only a hot fix for SCoP sequences that use the same load
  //       instruction contained and hoisted by one of the SCoPs.
  if (SE.isSCEVable(Ty))
    SE.forgetValue(AccInst);

  return PreloadVal;
}

Value *IslNodeBuilder::preloadInvariantLoad(const MemoryAccess &MA,
                                            isl_set *Domain) {

  isl_set *AccessRange = isl_map_range(MA.getAddressFunction());
  AccessRange = isl_set_gist_params(AccessRange, S.getContext());

  if (!materializeParameters(AccessRange, false)) {
    isl_set_free(AccessRange);
    isl_set_free(Domain);
    return nullptr;
  }

  auto *Build = isl_ast_build_from_context(isl_set_universe(S.getParamSpace()));
  isl_set *Universe = isl_set_universe(isl_set_get_space(Domain));
  bool AlwaysExecuted = isl_set_is_equal(Domain, Universe);
  isl_set_free(Universe);

  Instruction *AccInst = MA.getAccessInstruction();
  Type *AccInstTy = AccInst->getType();

  Value *PreloadVal = nullptr;
  if (AlwaysExecuted) {
    PreloadVal = preloadUnconditionally(AccessRange, Build, AccInst);
    isl_ast_build_free(Build);
    isl_set_free(Domain);
    return PreloadVal;
  }

  if (!materializeParameters(Domain, false)) {
    isl_ast_build_free(Build);
    isl_set_free(AccessRange);
    isl_set_free(Domain);
    return nullptr;
  }

  isl_ast_expr *DomainCond = isl_ast_build_expr_from_set(Build, Domain);
  Domain = nullptr;

  ExprBuilder.setTrackOverflow(true);
  Value *Cond = ExprBuilder.create(DomainCond);
  Value *OverflowHappened = Builder.CreateNot(ExprBuilder.getOverflowState(),
                                              "polly.preload.cond.overflown");
  Cond = Builder.CreateAnd(Cond, OverflowHappened, "polly.preload.cond.result");
  ExprBuilder.setTrackOverflow(false);

  if (!Cond->getType()->isIntegerTy(1))
    Cond = Builder.CreateIsNotNull(Cond);

  BasicBlock *CondBB = SplitBlock(Builder.GetInsertBlock(),
                                  &*Builder.GetInsertPoint(), &DT, &LI);
  CondBB->setName("polly.preload.cond");

  BasicBlock *MergeBB = SplitBlock(CondBB, &CondBB->front(), &DT, &LI);
  MergeBB->setName("polly.preload.merge");

  Function *F = Builder.GetInsertBlock()->getParent();
  LLVMContext &Context = F->getContext();
  BasicBlock *ExecBB = BasicBlock::Create(Context, "polly.preload.exec", F);

  DT.addNewBlock(ExecBB, CondBB);
  if (Loop *L = LI.getLoopFor(CondBB))
    L->addBasicBlockToLoop(ExecBB, LI);

  auto *CondBBTerminator = CondBB->getTerminator();
  Builder.SetInsertPoint(CondBBTerminator);
  Builder.CreateCondBr(Cond, ExecBB, MergeBB);
  CondBBTerminator->eraseFromParent();

  Builder.SetInsertPoint(ExecBB);
  Builder.CreateBr(MergeBB);

  Builder.SetInsertPoint(ExecBB->getTerminator());
  Value *PreAccInst = preloadUnconditionally(AccessRange, Build, AccInst);
  Builder.SetInsertPoint(MergeBB->getTerminator());
  auto *MergePHI = Builder.CreatePHI(
      AccInstTy, 2, "polly.preload." + AccInst->getName() + ".merge");
  PreloadVal = MergePHI;

  if (!PreAccInst) {
    PreloadVal = nullptr;
    PreAccInst = UndefValue::get(AccInstTy);
  }

  MergePHI->addIncoming(PreAccInst, ExecBB);
  MergePHI->addIncoming(Constant::getNullValue(AccInstTy), CondBB);

  isl_ast_build_free(Build);
  return PreloadVal;
}

bool IslNodeBuilder::preloadInvariantEquivClass(
    InvariantEquivClassTy &IAClass) {
  // For an equivalence class of invariant loads we pre-load the representing
  // element with the unified execution context. However, we have to map all
  // elements of the class to the one preloaded load as they are referenced
  // during the code generation and therefor need to be mapped.
  const MemoryAccessList &MAs = IAClass.InvariantAccesses;
  if (MAs.empty())
    return true;

  MemoryAccess *MA = MAs.front();
  assert(MA->isArrayKind() && MA->isRead());

  // If the access function was already mapped, the preload of this equivalence
  // class was triggered earlier already and doesn't need to be done again.
  if (ValueMap.count(MA->getAccessInstruction()))
    return true;

  // Check for recursion which can be caused by additional constraints, e.g.,
  // non-finite loop constraints. In such a case we have to bail out and insert
  // a "false" runtime check that will cause the original code to be executed.
  auto PtrId = std::make_pair(IAClass.IdentifyingPointer, IAClass.AccessType);
  if (!PreloadedPtrs.insert(PtrId).second)
    return false;

  // The execution context of the IAClass.
  isl_set *&ExecutionCtx = IAClass.ExecutionContext;

  // If the base pointer of this class is dependent on another one we have to
  // make sure it was preloaded already.
  auto *SAI = MA->getScopArrayInfo();
  if (auto *BaseIAClass = S.lookupInvariantEquivClass(SAI->getBasePtr())) {
    if (!preloadInvariantEquivClass(*BaseIAClass))
      return false;

    // After we preloaded the BaseIAClass we adjusted the BaseExecutionCtx and
    // we need to refine the ExecutionCtx.
    isl_set *BaseExecutionCtx = isl_set_copy(BaseIAClass->ExecutionContext);
    ExecutionCtx = isl_set_intersect(ExecutionCtx, BaseExecutionCtx);
  }

  // If the size of a dimension is dependent on another class, make sure it is
  // preloaded.
  for (unsigned i = 1, e = SAI->getNumberOfDimensions(); i < e; ++i) {
    const SCEV *Dim = SAI->getDimensionSize(i);
    SetVector<Value *> Values;
    findValues(Dim, SE, Values);
    for (auto *Val : Values) {
      if (auto *BaseIAClass = S.lookupInvariantEquivClass(Val)) {
        if (!preloadInvariantEquivClass(*BaseIAClass))
          return false;

        // After we preloaded the BaseIAClass we adjusted the BaseExecutionCtx
        // and we need to refine the ExecutionCtx.
        isl_set *BaseExecutionCtx = isl_set_copy(BaseIAClass->ExecutionContext);
        ExecutionCtx = isl_set_intersect(ExecutionCtx, BaseExecutionCtx);
      }
    }
  }

  Instruction *AccInst = MA->getAccessInstruction();
  Type *AccInstTy = AccInst->getType();

  Value *PreloadVal = preloadInvariantLoad(*MA, isl_set_copy(ExecutionCtx));
  if (!PreloadVal)
    return false;

  for (const MemoryAccess *MA : MAs) {
    Instruction *MAAccInst = MA->getAccessInstruction();
    assert(PreloadVal->getType() == MAAccInst->getType());
    ValueMap[MAAccInst] = PreloadVal;
  }

  if (SE.isSCEVable(AccInstTy)) {
    isl_id *ParamId = S.getIdForParam(SE.getSCEV(AccInst));
    if (ParamId)
      IDToValue[ParamId] = PreloadVal;
    isl_id_free(ParamId);
  }

  BasicBlock *EntryBB = &Builder.GetInsertBlock()->getParent()->getEntryBlock();
  auto *Alloca = new AllocaInst(AccInstTy, AccInst->getName() + ".preload.s2a");
  Alloca->insertBefore(&*EntryBB->getFirstInsertionPt());
  Builder.CreateStore(PreloadVal, Alloca);

  for (auto *DerivedSAI : SAI->getDerivedSAIs()) {
    Value *BasePtr = DerivedSAI->getBasePtr();

    for (const MemoryAccess *MA : MAs) {
      // As the derived SAI information is quite coarse, any load from the
      // current SAI could be the base pointer of the derived SAI, however we
      // should only change the base pointer of the derived SAI if we actually
      // preloaded it.
      if (BasePtr == MA->getBaseAddr()) {
        assert(BasePtr->getType() == PreloadVal->getType());
        DerivedSAI->setBasePtr(PreloadVal);
      }

      // For scalar derived SAIs we remap the alloca used for the derived value.
      if (BasePtr == MA->getAccessInstruction())
        ScalarMap[DerivedSAI] = Alloca;
    }
  }

  for (const MemoryAccess *MA : MAs) {

    Instruction *MAAccInst = MA->getAccessInstruction();
    // Use the escape system to get the correct value to users outside the SCoP.
    BlockGenerator::EscapeUserVectorTy EscapeUsers;
    for (auto *U : MAAccInst->users())
      if (Instruction *UI = dyn_cast<Instruction>(U))
        if (!S.contains(UI))
          EscapeUsers.push_back(UI);

    if (EscapeUsers.empty())
      continue;

    EscapeMap[MA->getAccessInstruction()] =
        std::make_pair(Alloca, std::move(EscapeUsers));
  }

  return true;
}

void IslNodeBuilder::allocateNewArrays() {
  for (auto &SAI : S.arrays()) {
    if (SAI->getBasePtr())
      continue;

    assert(SAI->getNumberOfDimensions() > 0 && SAI->getDimensionSize(0) &&
           "The size of the outermost dimension is used to declare newly "
           "created arrays that require memory allocation.");

    Type *NewArrayType = nullptr;
    for (int i = SAI->getNumberOfDimensions() - 1; i >= 0; i--) {
      auto *DimSize = SAI->getDimensionSize(i);
      unsigned UnsignedDimSize = static_cast<const SCEVConstant *>(DimSize)
                                     ->getAPInt()
                                     .getLimitedValue();

      if (!NewArrayType)
        NewArrayType = SAI->getElementType();

      NewArrayType = ArrayType::get(NewArrayType, UnsignedDimSize);
    }

    auto InstIt =
        Builder.GetInsertBlock()->getParent()->getEntryBlock().getTerminator();
    auto *CreatedArray = new AllocaInst(NewArrayType, SAI->getName(), &*InstIt);
    CreatedArray->setAlignment(PollyTargetFirstLevelCacheLineSize);
    SAI->setBasePtr(CreatedArray);
  }
}

bool IslNodeBuilder::preloadInvariantLoads() {

  auto &InvariantEquivClasses = S.getInvariantAccesses();
  if (InvariantEquivClasses.empty())
    return true;

  BasicBlock *PreLoadBB = SplitBlock(Builder.GetInsertBlock(),
                                     &*Builder.GetInsertPoint(), &DT, &LI);
  PreLoadBB->setName("polly.preload.begin");
  Builder.SetInsertPoint(&PreLoadBB->front());

  for (auto &IAClass : InvariantEquivClasses)
    if (!preloadInvariantEquivClass(IAClass))
      return false;

  return true;
}

void IslNodeBuilder::addParameters(__isl_take isl_set *Context) {

  // Materialize values for the parameters of the SCoP.
  materializeParameters(Context, /* all */ true);

  // Generate values for the current loop iteration for all surrounding loops.
  //
  // We may also reference loops outside of the scop which do not contain the
  // scop itself, but as the number of such scops may be arbitrarily large we do
  // not generate code for them here, but only at the point of code generation
  // where these values are needed.
  Loop *L = LI.getLoopFor(S.getEntry());

  while (L != nullptr && S.contains(L))
    L = L->getParentLoop();

  while (L != nullptr) {
    const SCEV *OuterLIV = SE.getAddRecExpr(SE.getUnknown(Builder.getInt64(0)),
                                            SE.getUnknown(Builder.getInt64(1)),
                                            L, SCEV::FlagAnyWrap);
    Value *V = generateSCEV(OuterLIV);
    OutsideLoopIterations[L] = SE.getUnknown(V);
    L = L->getParentLoop();
  }

  isl_set_free(Context);
}

Value *IslNodeBuilder::generateSCEV(const SCEV *Expr) {
  /// We pass the insert location of our Builder, as Polly ensures during IR
  /// generation that there is always a valid CFG into which instructions are
  /// inserted. As a result, the insertpoint is known to be always followed by a
  /// terminator instruction. This means the insert point may be specified by a
  /// terminator instruction, but it can never point to an ->end() iterator
  /// which does not have a corresponding instruction. Hence, dereferencing
  /// the insertpoint to obtain an instruction is known to be save.
  ///
  /// We also do not need to update the Builder here, as new instructions are
  /// always inserted _before_ the given InsertLocation. As a result, the
  /// insert location remains valid.
  assert(Builder.GetInsertBlock()->end() != Builder.GetInsertPoint() &&
         "Insert location points after last valid instruction");
  Instruction *InsertLocation = &*Builder.GetInsertPoint();
  return expandCodeFor(S, SE, DL, "polly", Expr, Expr->getType(),
                       InsertLocation, &ValueMap,
                       StartBlock->getSinglePredecessor());
}

/// The AST expression we generate to perform the run-time check assumes
/// computations on integer types of infinite size. As we only use 64-bit
/// arithmetic we check for overflows, in case of which we set the result
/// of this run-time check to false to be cosnservatively correct,
Value *IslNodeBuilder::createRTC(isl_ast_expr *Condition) {
  auto ExprBuilder = getExprBuilder();
  ExprBuilder.setTrackOverflow(true);
  Value *RTC = ExprBuilder.create(Condition);
  if (!RTC->getType()->isIntegerTy(1))
    RTC = Builder.CreateIsNotNull(RTC);
  Value *OverflowHappened =
      Builder.CreateNot(ExprBuilder.getOverflowState(), "polly.rtc.overflown");

  if (PollyGenerateRTCPrint) {
    auto *F = Builder.GetInsertBlock()->getParent();
    RuntimeDebugBuilder::createCPUPrinter(
        Builder,
        "F: " + F->getName().str() + " R: " + S.getRegion().getNameStr() +
            " __RTC: ",
        RTC, " Overflow: ", OverflowHappened, "\n");
  }

  RTC = Builder.CreateAnd(RTC, OverflowHappened, "polly.rtc.result");
  ExprBuilder.setTrackOverflow(false);

  if (!isa<ConstantInt>(RTC))
    VersionedScops++;

  return RTC;
}
