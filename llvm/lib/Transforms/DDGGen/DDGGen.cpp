//===DDGGen.cpp - an LLVM Pass to generate Data Dependency Graph for CGRAs"===//
// Author: Shail Dave
// Last Modified on: April 15, 2018
//
//===----------------------------------------------------------------------===//
//
// This file detects critical loops annoted with label CGRA and generates
// DDG for coarse-grained reconfigurable array accelerators
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "ddg-gen"

#include "DFG.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "llvm/Analysis/GlobalsModRef.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/ScalarEvolutionAliasAnalysis.h"

#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Use.h"
#include "llvm/IR/Value.h"

#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"
#include <string>
#include <iostream>
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/Dominators.h"

#include "llvm/Transforms/Utils/LCSSA.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/Transforms/Utils/SSAUpdater.h"

#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace llvm;

unsigned int NodeID = 0;
unsigned int EdgeID = 0;
unsigned int ArrID = 0;
unsigned int TotalLoops = 0;

std::ofstream liveoutNodefile;
std::ofstream liveoutEdgefile;
std::ofstream liveInNodefile;
std::ofstream liveInEdgefile;

Module *M;

// Variable indicating whether the loop TC can be determined statically
// If true, unsafe operations (stores and live-outs) need to be padded with SelectInst
// If not padded, it affects data integrity due to execution of kernel and epilogue
bool dynamicTC = true;
BranchInst *loopExitBranch;
Instruction *loopExitCond;

// Counters for global Variable Declarations for live-in
int gPtrNo = 0;
int gVarNo = 0;
int dummyStoreVarNo = 0;

// Map containing instruction to be assigned to global variable/pointer/array
// Instruction is the one that defined live-in value
// Instruction is mapped to corresponding load instruction
std::map<llvm::Value*, std::string> map_instn_defn_livein_to_load;

std::map<Instruction*, std::string> map_instn_defn_liveout_to_load;

std::map<std::string, unsigned> map_livein_to_alignment;
std::map<std::string, unsigned> map_liveout_to_alignment;

std::map< BasicBlock*, BasicBlock* > map_liveout_destnBB_newBB;
std::map< Instruction*, Instruction*> map_replace_liveout_use;

//Map containing idxprom nodes and predecessor
std::map<int,int> idxprom_nodeID_pred_map;
std::map<Instruction*,unsigned> idxprom_nodeID_load_alignment_map;

// Returns the loop hint metadata node with the given name (for example,
// "llvm.loop.CGRA.enable").  If no such metadata node exists, then nullptr is
// returned.
static const MDNode *GetCGRAMetadata(const Loop *L, StringRef Name) {
  MDNode *LoopID = L->getLoopID();
  if (!LoopID)
  return nullptr;

  // First operand should refer to the loop id itself.
  assert(LoopID->getNumOperands() > 0 && "requires at least one operand");
  assert(LoopID->getOperand(0) == LoopID && "invalid loop id");

  for (unsigned i = 1, e = LoopID->getNumOperands(); i < e; ++i) {
    const MDNode *MD = dyn_cast<MDNode>(LoopID->getOperand(i));
    if (!MD)
    continue;

    const MDString *S = dyn_cast<MDString>(MD->getOperand(0));
    if (!S)
    continue;

    if (Name.equals(S->getString()))
    return MD;
  }
  return nullptr;
}

// Returns true if the loop has an CGRA(disable) pragma.
static bool HasCGRADisablePragma(const Loop *L) {
  return GetCGRAMetadata(L, "llvm.loop.CGRA.disable");
}

// Returns true if the loop has an CGRA(enable) pragma.
//static bool HasUnrollFullPragma(const Loop *L) {
static bool HasCGRAEnablePragma(const Loop *L) {
  return GetCGRAMetadata(L, "llvm.loop.CGRA.enable");
}

// Set Loop already provided hint for acceleration
static void SetLoopAlreadyIterated(Loop *L) {
  MDNode *LoopID = L->getLoopID();
  if (!LoopID) return;

  // First remove any existing loop CGRA metadata.
  SmallVector<Metadata *, 4> MDs;
  // Reserve first location for self reference to the LoopID metadata node.
  MDs.push_back(nullptr);

  if (LoopID) {
    for (unsigned i = 1, ie = LoopID->getNumOperands(); i < ie; ++i) {
      bool IsIteratedMetadata = false;
      MDNode *MD = dyn_cast<MDNode>(LoopID->getOperand(i));
      if (MD) {
        const MDString *S = dyn_cast<MDString>(MD->getOperand(0));
        IsIteratedMetadata = S && S->getString().startswith("llvm.loop.CGRA.");
      }
      if (!IsIteratedMetadata) MDs.push_back(LoopID->getOperand(i));
    }
  }

  // Add CGRA(disable) metadata to disable future unrolling.
  LLVMContext &Context = L->getHeader()->getContext();
  SmallVector<Metadata *, 1> DisableOperands;
  DisableOperands.push_back(MDString::get(Context, "llvm.loop.CGRA.disable"));
  MDNode *DisableNode = MDNode::get(Context, DisableOperands);
  MDs.push_back(DisableNode);

  MDNode *NewLoopID = MDNode::get(Context, MDs);
  // Set operand 0 to refer to the loop id itself.
  NewLoopID->replaceOperandWith(0, NewLoopID);
  L->setLoopID(NewLoopID);
}

namespace llvm
{
  struct DDGGen: public LoopPass
  {
    static char ID;        // Pass identification, replacement for typeid
    DDGGen() :
    LoopPass(ID)
    {

    }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      //getLoopAnalysisUsage(AU);

      AU.setPreservesCFG();

      AU.addRequired<DominatorTreeWrapperPass>();
      AU.addRequired<LoopInfoWrapperPass>();
      AU.addRequired<ScalarEvolutionWrapperPass>();
      AU.addRequired<SCEVAAWrapperPass>();
      AU.addPreservedID(LoopSimplifyID);
      AU.addPreserved<AAResultsWrapperPass>();
      AU.addPreserved<BasicAAWrapperPass>();
      AU.addPreserved<GlobalsAAWrapperPass>();

      // This is needed to perform LCSSA verification inside LPPassManager
      AU.addRequired<LCSSAVerificationPass>();
      AU.addPreserved<LCSSAVerificationPass>();

    }

    static void pushIVUsers(
      Instruction *Def,
      SmallPtrSet<Instruction*,16> &Simplified,
      SmallVectorImpl< std::pair<Instruction*,Instruction*> > &SimpleIVUsers) {

      for (User *U : Def->users()) {
        Instruction *UI = cast<Instruction>(U);

        // Avoid infinite or exponential worklist processing.
        // Also ensure unique worklist users.
        // If Def is a LoopPhi, it may not be in the Simplified set, so check for
        // self edges first.
        if (UI != Def && Simplified.insert(UI).second)
          SimpleIVUsers.push_back(std::make_pair(UI, Def));
      }
    }

    void resetGlobalVariables()
    {
      NodeID = 0;
      EdgeID = 0;
      ArrID = 0;

      map_instn_defn_livein_to_load.clear();
      map_instn_defn_liveout_to_load.clear();
      map_livein_to_alignment.clear();
      map_liveout_to_alignment.clear();
      map_liveout_destnBB_newBB.clear();
      map_replace_liveout_use.clear();
      idxprom_nodeID_pred_map.clear();
      idxprom_nodeID_load_alignment_map.clear();
    }

    bool isTCknownAtLoopEntry(std::vector<BasicBlock *> bbs)
    {
      Value *val1 = loopExitCond->getOperand(0);
      Instruction *Op1 = dyn_cast<Instruction>(val1);
      Instruction *Op2 = dyn_cast<Instruction>(loopExitCond->getOperand(1));
      if((Op1) && (Op2))
      {
        if(( std::find(bbs.begin(), bbs.end(), Op1->getParent()) != bbs.end()) &&
          ( std::find(bbs.begin(), bbs.end(), Op2->getParent()) != bbs.end()))
          return false;
      }
      else if(Op1)
      {
        Instruction *pred_ins_Op1 = dyn_cast<Instruction>(Op1->getOperand(0));
        Instruction *pred_ins_Op2 = dyn_cast<Instruction>(Op1->getOperand(1));
        if((pred_ins_Op1) && (pred_ins_Op2))
        {
          if(( std::find(bbs.begin(), bbs.end(), pred_ins_Op1->getParent()) != bbs.end()) &&
            ( std::find(bbs.begin(), bbs.end(), pred_ins_Op2->getParent()) != bbs.end()))
            return false;
        }
      }
      else if(Op2)
      {
        Instruction *pred_ins_Op1 = dyn_cast<Instruction>(Op2->getOperand(0));
        Instruction *pred_ins_Op2 = dyn_cast<Instruction>(Op2->getOperand(1));
        if((pred_ins_Op1) && (pred_ins_Op2))
        {
          if(( std::find(bbs.begin(), bbs.end(), pred_ins_Op1->getParent()) != bbs.end()) &&
            ( std::find(bbs.begin(), bbs.end(), pred_ins_Op2->getParent()) != bbs.end()))
            return false;
        }
      }

      return true;
    }

    void calculateTCDynamically(PHINode *IV, std::vector<BasicBlock *> bbs, BasicBlock *Preheader)
    {
      // get initial value of loop IV
      Value *startValue;
      Instruction *incOp;

      for(unsigned ii=0; ii<IV->getNumOperands(); ii++)
      {
        Value *val = IV->getOperand(ii);
        Instruction *Op = dyn_cast<Instruction>(val);
        if(Op)
        {
          if(std::find(bbs.begin(), bbs.end(), Op->getParent()) != bbs.end())
          {
            incOp = Op;
            continue;
          }
          startValue = Op;
        }
        else
          startValue = val; // constant value
      }
      Value *endValue = loopExitCond->getOperand(1);
      Value *cmpOp1 = loopExitCond->getOperand(0);

      std::vector<Value *> operands;
      Value *storeVal;
      switch (incOp->getOpcode())
      {
        // Need to fix for other increment operations e.g. sub, mul, shift
        case Instruction::Add:
        // TC can be TC+1 if instruction is le instead of lt
          operands.push_back(endValue);
          operands.push_back(startValue);
          TerminatorInst *TI = Preheader->getTerminator();
          IRBuilder<> builder(TI); //building the cloned instruction

          Value *sub = builder.CreateSub(endValue, startValue);
          Value *div = builder.CreateSDiv(sub, incOp->getOperand(1));
          Instruction *divInst = dyn_cast<Instruction>(div);
          storeVal = div;
          if(cmpOp1 == dyn_cast<Value>(IV))
          {
            Value *ConstInt1 = ConstantInt::get(Type::getInt32Ty(M->getContext()), 1);
            storeVal = builder.CreateAdd(div, ConstInt1);
          }
          Constant * tcPtr = M->getOrInsertGlobal("dynamicTCVal", divInst->getType());
          StoreInst *newStoreInstn = new StoreInst(storeVal,tcPtr,TI);
          break;
        Default:
          errs() << "Action Required to Compute Dynamic TC!\n";
          break;
      }
    }

    static PHINode *getInductionVariable(Loop *L, ScalarEvolution *SE, DominatorTree *DT) {
      unsigned cnt=0;
      if (L->getLoopLatch() == nullptr || L->getLoopPredecessor() == nullptr)
        return nullptr;
      for(cnt=0; cnt<2; cnt++)
      {
        for (BasicBlock::iterator I = L->getHeader()->begin(); isa<PHINode>(I); ++I) {
          PHINode *PhiVar = cast<PHINode>(I);
          Type *PhiTy = PhiVar->getType();
          if (!PhiTy->isIntegerTy() && !PhiTy->isFloatingPointTy() &&
          !PhiTy->isPointerTy())
          return nullptr;
          const SCEVAddRecExpr *AddRec =
          dyn_cast<SCEVAddRecExpr>(SE->getSCEV(PhiVar));
          if (!AddRec || !AddRec->isAffine())
          continue;
          const SCEV *Step = AddRec->getStepRecurrence(*SE);
          if (!isa<SCEVConstant>(Step))
          continue;
          Instruction *Op1 = dyn_cast<Instruction>(loopExitCond->getOperand(0));

          // Found the induction variable.
          // FIXME: Handle loops with more than one induction variable. Note that,
          // currently, legality makes sure we have only one induction variable.
          if(cnt==0)
          {
            if(Op1 == PhiVar)
              return PhiVar;

            // get operands of Phi
            Instruction *pred_Op1 = dyn_cast<Instruction>(I->getOperand(0));
            Instruction *pred_Op2 = dyn_cast<Instruction>(I->getOperand(1));
            if((Op1 == pred_Op1) || (Op1 == pred_Op2))
              return PhiVar;
          }
          else if(cnt==1)
          {
            if(DT->dominates(PhiVar,Op1))
              return PhiVar;
          }
        }
      }
      return nullptr;
    }


    void checkIV(PHINode *CurrIV, ScalarEvolution *SE, DominatorTree *DT)
    {
      if (!SE->isSCEVable(CurrIV->getType()))
        return;

      // Instructions processed by checkIV for CurrIV.
      SmallPtrSet<Instruction*,16> Simplified;

      // Use-def pairs if IV users waiting to be processed for CurrIV.
      SmallVector<std::pair<Instruction*, Instruction*>, 8> SimpleIVUsers;

      // Push users of the current LoopPhi. In rare cases, pushIVUsers may be
      // called multiple times for the same LoopPhi. This is the proper thing to
      // do for loop header phis that use each other.
      pushIVUsers(CurrIV, Simplified, SimpleIVUsers);

      while (!SimpleIVUsers.empty()) {

        std::pair<Instruction*, Instruction*> UseOper =
        SimpleIVUsers.pop_back_val();
        Instruction *UseInst = UseOper.first;

        // Bypass back edges to avoid extra work.
        //if (UseInst == CurrIV) continue;
        if (!SE->isSCEVable(UseInst->getType()))
          continue; //return false;
      }
    }

    unsigned calculateLoopTC(Loop *L, ScalarEvolution *SE)
    {
      //Generate Loop Trip Count
      // If there are multiple exiting blocks but one of them is the latch, use the
      // latch for the trip count estimation. Otherwise insist on a single exiting
      // block for the trip count estimation.
      unsigned TripCount = 0;
      BasicBlock *ExitingBlockTemp = L->getLoopLatch();
      if (!ExitingBlockTemp || !L->isLoopExiting(ExitingBlockTemp))
      ExitingBlockTemp = L->getExitingBlock();
      if (ExitingBlockTemp) {
        TripCount = SE->getSmallConstantTripCount(L, ExitingBlockTemp);
      }

      return TripCount;
    }

    //distance between 2 instructions, either 0 or 1 at this level
    //this function checks if both instructions are within one basic block
    //if so, we just need to find their  order within the block
    //If not, we need to traverse basic blocks to find the order of their basicblocks
    int getDistance(Instruction *ins1, BasicBlock* bb1, Instruction *ins2, BasicBlock* bb2, std::vector<BasicBlock *> bbs, BasicBlock* loopLatch)
    {
      //both within the same basic blocks, find the order in BB
      if (bb1->getName() == bb2->getName())
      {
        for (BasicBlock::iterator BI = bb1->begin(); BI != bb1->end(); ++BI)
        {
          //instruction 1 is visited before ins2-intra iteration dependency
          if (BI->isIdenticalTo(ins1))
          {
            return 0;
          }
          //instruction 2 is visited before instruction 1-inter-iteration dependency
          if (BI->isIdenticalTo(ins2))
          {
            return 1;
          }
        }
        //should never reach here
        return -1;
      }
      //2 different blocks
      else
      {
        //if bb1 is looplatch, because looplatch is the last basic block, dependency is loopcarried dependency and distance = 1
        if (bb1->getName()==loopLatch->getName())
        {
          return 1;
        }
        //set of basicblocks to visit
        std::vector<BasicBlock*> to_visit;
        //set of already visited basicblocks
        std::vector<BasicBlock*> visited;
        //we start from bb1
        to_visit.push_back(bb1);
        //traverse loop CFG

        while(to_visit.size() > 0)
        {
          BasicBlock* it = to_visit[0];
          to_visit.erase(to_visit.begin());
          visited.push_back(it);
          //what are the set of next basicblocks
          for (succ_iterator pblocks = llvm::succ_begin(it); pblocks != llvm::succ_end(it); pblocks++)
          {
            //if BB is not within loop BB, ignore it
            if (!contains(*pblocks, bbs))
            {
              continue;
            }
            //if we have reached bb2, it is dependency inside 1 iteration
            if (bb2->getName() == (*pblocks)->getName())
            {
              return 0;
            }
            //we do not traverse beyond loop latch, if we dont reach the second block, it is inter iteration
            //we should not stop here though because there might be more blocks in to_visit set that has not traversed yet
            if ((*pblocks)->getName() == loopLatch->getName())
            {
              continue;
            }
            //is this bb already visited? dont add it to traverse list
            if (contains(*pblocks, visited))
            {
              continue;
            }
            //is it already added to traverse list?
            if (contains(*pblocks, to_visit))
            {
              continue;
            }
            //add it to traverse list
            to_visit.push_back(*pblocks);
          }
        }

        //if we are here, it is inter-iteration dependency
        return 1;
      }
    }

    // find the last cmp instruction within a basic block
    Instruction* Find_Last_CMP_Instruction(BasicBlock* bb)
    {
      for (BasicBlock::reverse_iterator TBI = bb->rbegin(); TBI != bb->rend(); ++TBI)
      {
        if (TBI->getOpcode() == Instruction::ICmp || TBI->getOpcode() == Instruction::FCmp)
        {
          return &*TBI;
        }
      }
      return NULL;
    }

    SelectInst* convertPHItoSel(Instruction *BI, Instruction *insLoc, BasicBlock * LoopExitBlk)
    {
      PHINode* phiInst = dyn_cast<PHINode>(BI);
      Value *op1 = phiInst->getIncomingValue(0);
      Value *op2 = phiInst->getIncomingValue(1);
      Value *cond;
      for(BasicBlock::iterator BI_temp = LoopExitBlk->begin(); BI_temp !=LoopExitBlk->end(); ++BI_temp)
      {
        if(BI_temp->getOpcode() != Instruction::Br)
          continue;

        BranchInst* tempBrInst = dyn_cast<llvm::BranchInst>(BI_temp);
        if(tempBrInst->isUnconditional()) break;
        brInst = tempBrInst;
        cond = tempBrInst->getCondition();
        SelectInst* newSelectInst;
        newSelectInst = newSelectInst->Create(cond,op1,op2,"",insLoc);
        BI->eraseFromParent();
        return newSelectInst;
      }
    }

    StoreInst* getLiveOutToBeStored(Instruction *BI, std::vector<BasicBlock *> bbs, std::vector<BasicBlock *> LoopLatchExitBlks)
    {
      StoreInst * ptr;
      // Reject two common cases fast: instructions with no uses (like stores)
      // and instructions with one use that is in the same block as this.
      if (BI->use_empty())
        return NULL;

      ptr = NULL;
      for(auto U : BI->users()){  // U is of type User*
        if(auto Inst = dyn_cast<Instruction>(U)){
          // an instruction uses V
          // Ensure that the use is outside of the BBs of loop
          if ( std::find(bbs.begin(), bbs.end(), Inst->getParent()) != bbs.end() )
            continue;

          if(std::find(LoopLatchExitBlks.begin(), LoopLatchExitBlks.end(), Inst->getParent()) == LoopLatchExitBlks.end())
            continue;
          //delete instance of the store
          if(Inst->getOpcode() == Instruction::Store)
          {
            //get getPointerOperand() from storeInst class
            ptr = (dyn_cast<StoreInst>(Inst));
            break;
          }
          else
            ptr = getLiveOutToBeStored(Inst, bbs, LoopLatchExitBlks);
          if(ptr != NULL) break;
        }
      }
      if(ptr != NULL)
      {
        GlobalValue* G = dyn_cast<llvm::GlobalValue>(ptr);
        if(G == NULL)
        {
          ptr = NULL; // If not a global pointer
        }
      }
      return ptr;
    }

    void getLoopExitBranch(BasicBlock *LoopExitBlk)
    {
      for (BasicBlock::reverse_iterator TBI = LoopExitBlk->rbegin(); TBI != LoopExitBlk->rend(); ++TBI)
      {
        if(TBI->getOpcode() != Instruction::Br)
          continue;

        BranchInst* tempBrInst = dyn_cast<llvm::BranchInst>(&*TBI);
        if(tempBrInst->isUnconditional())
          break;

        loopExitBranch = tempBrInst;
        loopExitCond = dyn_cast<llvm::Instruction>(tempBrInst->getCondition());
      }
    }

    NODE * updateExitConditionNode(DFG *myDFG, NODE *exitCond, std::vector<BasicBlock *> bbs)
    {
      NODE *phiNode = new NODE(select, 1, NodeID++, "", NULL);
      myDFG->insert_Node(phiNode);
      NODE *constInt;

      BranchInst *brInst = loopExitBranch;
      if(brInst->isUnconditional())
      {
        errs() << "Should not reach here!\n";
        return NULL;
      }
      BasicBlock *bb1 = brInst->getSuccessor(0);
      if ( std::find(bbs.begin(), bbs.end(), bb1) != bbs.end())
      {
        constInt = new NODE(constant, 1, NodeID++, "ConstInt1", NULL);
      }
      else
      {
        constInt = new NODE(constant, 1, NodeID++, "ConstInt0", NULL);
      }

      myDFG->insert_Node(constInt);

      myDFG->make_Arc(exitCond, phiNode, EdgeID++, 1, TrueDep, 0);
      myDFG->make_Arc(constInt, phiNode, EdgeID++, 0, TrueDep, 1);

      return phiNode;
    }

    NODE * padUnsafeLiveOutOperationsWithSelects(DFG *myDFG, NODE *unsafeOp, NODE *exitCond, std::vector<BasicBlock *> bbs)
    {
      NODE *selOp = new NODE(cond_select, 1, NodeID++, "", NULL);
      myDFG->insert_Node(selOp);

      /* Due to ISA challenges for select instruction,
      a select operation cannot store its output value in registers.
      Mapping is simplified and does not consider such constraints.
      Therefore, we cannot pad with a single select operation with recurrence
      Instead, route (add with 0) the output to select,
      forming Loop-carried dependence with a delay of the cycle as 2 instead of 1 */

      NODE *forwardValue = new NODE(add, 1, NodeID++, "", NULL);
      myDFG->insert_Node(forwardValue);
      NODE *const0 = new NODE(constant, 1, NodeID++, "ConstInt0", NULL);
      myDFG->insert_Node(const0);

      unsigned selectSelf_opOrder;
      unsigned selectInput_opOrder;
      BranchInst *brInst = loopExitBranch;
      if(brInst->isUnconditional())
      {
        errs() << "Should not reach here!\n";
        return NULL;
      }
      BasicBlock *bb1 = brInst->getSuccessor(0);
      if ( std::find(bbs.begin(), bbs.end(), bb1) != bbs.end())
      {
        selectInput_opOrder = 0;
        selectSelf_opOrder = 1;
      }
      else
      {
        selectInput_opOrder = 1;
        selectSelf_opOrder = 0;
      }

      myDFG->make_Arc(unsafeOp, selOp, EdgeID++, 0, TrueDep, selectInput_opOrder);
      myDFG->make_Arc(forwardValue, selOp, EdgeID++, 1, TrueDep, selectSelf_opOrder);
      myDFG->make_Arc(exitCond, selOp, EdgeID++, 0, PredDep, 2);

      myDFG->make_Arc(selOp, forwardValue, EdgeID++, 0, TrueDep, 0);
      myDFG->make_Arc(const0, forwardValue, EdgeID++, 0, TrueDep, 1);

      return forwardValue;
    }

    /* Need to control loop exit, for loops with statically unknown tripcounts
       (dynamicTC as well as tripcounts known at run-time before loop-entry)

       This is done by generating loop exit control operation.
       If true, CGRA controller (finishes and) exits the execution of kernel.
       Then, after finishing kernel, it will execute Epilogue.

       Pads Loop Exit Condition with an exit node.
       Exit Node Informs CGRA controller to quit, whenever true.
       For a branch Instruction, We should exit if --
       1. condition is true, loop exit BB is successors(0)
       2. condition is false, loop exit BB is successors(1)

       For exit node, there are 2 inputs.
       1) IP0 = CMP and IP1 = ConstInt0 or
       2) IP0 = ConstInt1 and IP1 = CMP.

       To determine, exit condition, we can form Truth table as

                              |   IP0   IP1   Exit?
        CMP is IP0            |   1     0     Yes
        If true, then exit    |   0     0
        -----------------------------------------------
        CMP is IP1            |   1     1
        If false, then exit   |   1     0     Yes
    */
    NODE * generateLoopExitNode(DFG *myDFG, NODE *exitCond, std::vector<BasicBlock *> bbs)
    {
      NODE *exitOp = new NODE(loopctrl, 1, NodeID++, "", NULL);
      myDFG->insert_Node(exitOp);

      NODE *constInt;

      unsigned exitOp_Order;
      unsigned const_opOrder;
      BranchInst *brInst = loopExitBranch;
      if(brInst->isUnconditional())
      {
        errs() << "Should not reach here!\n";
        return NULL;
      }
      BasicBlock *bb1 = brInst->getSuccessor(0);
      if ( std::find(bbs.begin(), bbs.end(), bb1) != bbs.end())
      {
        exitOp_Order = 1;
        const_opOrder = 0;
        constInt = new NODE(constant, 1, NodeID++, "ConstInt1", NULL);
      }
      else
      {
        exitOp_Order = 0;
        const_opOrder = 1;
        constInt = new NODE(constant, 1, NodeID++, "ConstInt0", NULL);
      }

      myDFG->insert_Node(constInt);
      myDFG->make_Arc(exitCond, exitOp, EdgeID++, 0, TrueDep, exitOp_Order);
      myDFG->make_Arc(constInt, exitOp, EdgeID++, 0, TrueDep, const_opOrder);

      return exitOp;
    }

    bool Update_LiveOut_Variables(Instruction *BI, DFG* myDFG, std::vector<BasicBlock *> bbs, std::vector<BasicBlock *> LoopLatchExitBlks)
    {
      bool IRChanged = false;
      std::string storeVariable;
      bool loadInserted = false;
      StoreInst * newStore;
      Value *ptr;
      unsigned alignment = 0;

      //if((BI->getOpcode() == Instruction::Store) && dynamicTC)
      //  padUnsafeStoreOperationsWithSelects(BI);

      // Reject two common cases fast: instructions with no uses (like stores)
      // and instructions with one use that is in the same block as this.
      if (BI->use_empty())
        return IRChanged;

      bool usedOutside = false;
      for(auto U : BI->users()){  // U is of type User*

        if(auto Inst = dyn_cast<Instruction>(U)){
          // an instruction uses Value
          // Ensure that the use is outside of the BBs of loop
          if ( std::find(bbs.begin(), bbs.end(), Inst->getParent()) == bbs.end() )
            usedOutside = true;
        }
      }

      if (!usedOutside) return IRChanged;
      std::map<Instruction *, std::string>::iterator it;
      it = map_instn_defn_liveout_to_load.find(BI);
      if (it != map_instn_defn_liveout_to_load.end() )
        return IRChanged;

      NODE *node;

      newStore = getLiveOutToBeStored(BI, bbs, LoopLatchExitBlks);
      if(newStore == NULL)
      {
        std::string gPtrName;
        Value *v = dyn_cast<Value>(BI);
        Type* T = v->getType();
        GlobalVariable *gPtr;
        alignment = 4;

        if(T->isIntegerTy() || T->isFloatingPointTy())
        {
          gPtrName = "gVar" + std::to_string(++gVarNo);
          // Create new global pointer
          gPtr = new GlobalVariable(*M, T, false, GlobalValue::CommonLinkage, 0, gPtrName);
          Constant *constVal = Constant::getNullValue(T);
          gPtr->setInitializer(constVal);
          alignment = (T->getPrimitiveSizeInBits())/8;
        }
        else if(T->isPointerTy())
        {
          gPtrName = "gPtr" + std::to_string(++gPtrNo);
          PointerType* PointerTy_0 = dyn_cast<PointerType>(T);// PointerType::get(T, 0);
          gPtr = new GlobalVariable(*M, T, false, GlobalValue::CommonLinkage, 0, gPtrName);
          ConstantPointerNull* const_ptr_2 = ConstantPointerNull::get(PointerTy_0);
          gPtr->setInitializer(const_ptr_2);
          alignment = T->getPointerAddressSpace();
          if(alignment < 4) alignment = 4;
        }
        if(alignment == 0)
          errs() << "Cannot find size of the memory access for live-out variable\n";
        gPtr->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);
        node = new NODE(constant, 1, ((NodeID++)+100), gPtrName, gPtr);
        node->setAlignment(alignment);
        myDFG->insert_Node(node);
        ptr = gPtr;
        storeVariable = gPtrName;
      }
      // Find the store node, if not found track it using successors in the next block
      else
      {
        ptr = newStore->getPointerOperand();
        if(ptr == NULL) return false;
        IRChanged = true;
        storeVariable = ptr->getName().str();
        node = myDFG->get_Node(ptr);
        std::vector<NODE*> setofNodes = myDFG->getSetOfVertices();
        for(unsigned int i=0; i < setofNodes.size(); i++)
        {
          if(setofNodes[i]->get_Name() == storeVariable)
          node = setofNodes[i];
        }
        if(node == NULL) {
          node = new NODE(constant, 1, ((NodeID++)+100), storeVariable, ptr);
          myDFG->insert_Node(node);
        }
      }

      map_instn_defn_liveout_to_load[BI] = storeVariable;
      map_liveout_to_alignment[storeVariable] = alignment;
      NODE *outputNode = myDFG->get_Node(BI);

      if(dynamicTC)
      {
        NODE *exitCond = myDFG->get_Node(dyn_cast<Instruction>(loopExitBranch->getCondition()));

        //TODO: If loop exit condition is at top of loop BBs, avoid phi below
        exitCond = updateExitConditionNode(myDFG, exitCond, bbs);

        NODE *loopExitNode = generateLoopExitNode(myDFG, exitCond, bbs);

        if(outputNode == exitCond)
          outputNode = loopExitNode;

        outputNode = padUnsafeLiveOutOperationsWithSelects(myDFG, outputNode, exitCond, bbs);
      }

      myDFG->make_Arc(outputNode, node, EdgeID++, 0, LiveOutDataDep, 0);

      unsigned int CGRA_ConstantID = node->get_ID();
      unsigned int CGRA_storeAddID = ((NodeID++)+100);
      unsigned int CGRA_storeDataID = ((NodeID++)+100);

      //Generate file for live-out
      liveoutNodefile << outputNode->get_ID() << "\t" << outputNode->get_Instruction() << "\t" << outputNode->get_Name() << "\t" << 0 << "\n";
      liveoutNodefile << CGRA_ConstantID << "\t" << constant << "\t" << storeVariable << "\t" << 0 << "\n";
      liveoutNodefile << CGRA_storeAddID << "\t" << st_add << "\t" << "st_add_" + storeVariable << "\t" << alignment << "\n";
      liveoutNodefile << CGRA_storeDataID << "\t" << st_data << "\t" << "st_data_" + storeVariable << "\t" << 0 << "\n";

      liveoutEdgefile << CGRA_storeAddID << "\t" << CGRA_storeDataID << "\t0\tSRE\t0\n";
      liveoutEdgefile << outputNode->get_ID() << "\t" << CGRA_storeDataID << "\t0\tTRU\t0\n";
      liveoutEdgefile << CGRA_ConstantID << "\t" << CGRA_storeDataID << "\t0\tTRU\t1\n";

      // delete stores and modify corresponding accesses if no store
      LoadInst* loadGlobal;
      LoadInst* loadGlobalNonPhi;

      // See if need to load and modify the operands of the successors
      for(auto U : BI->users()){  // U is of type User*
        if(auto Inst = dyn_cast<Instruction>(U))
        {
          // an instruction uses V
          // Ensure that the use is outside of the BBs of loop
          if ( std::find(bbs.begin(), bbs.end(), Inst->getParent()) != bbs.end() )
            continue;

          //delete instance of the store
          if((Inst->getOpcode() == Instruction::Store) &&
             (dyn_cast<llvm::GlobalValue>(Inst)))
          {
            Inst->eraseFromParent();
            break;
          }
          else
          {
              if(Inst->getOpcode() == Instruction::PHI)
              {
                // Create new BB between LoopLatchExit and the block where PHI is located
                // Insert load instruction in newly created BB

                // First check if newBB was already inserted.
                // If not create a newBB and insert between a loop block (may be looplatch)
                // and the block containing the Phi
                // TODO: Due to single loop exit block, currently assumed that
                // the sourceBB from loop for the destination phi node would be same.
                BasicBlock *newBB, *destinationBB;
                destinationBB = Inst->getParent();

                std::map<BasicBlock *, BasicBlock *>::iterator it;
                it = map_liveout_destnBB_newBB.find(destinationBB);
                if (it != map_liveout_destnBB_newBB.end() )
                {
                  newBB = it->second;
                }
                else
                {
                  newBB = newBB->Create(destinationBB->getContext(), "", destinationBB->getParent(), nullptr);

                  BasicBlock *sourceBB = BI->getParent();
                  // Find out the BB from which the live-out value is coming from
                  PHINode *phiInst = dyn_cast<PHINode>(Inst);
                  for(unsigned i=0; i < phiInst->getNumIncomingValues(); i++)
                  {
                    if(dyn_cast<Instruction>(phiInst->getIncomingValue(i)) == BI)
                    {
                      sourceBB = phiInst->getIncomingBlock(i);
                      break;
                    }
                  }
                  newBB->moveAfter(sourceBB);

                  // Need to update input BB to newBB for remining PHI nodes, if any!
                  sourceBB->replaceSuccessorsPhiUsesWith(newBB);

                  //update newBB as successor of sourceBB
                  for(BasicBlock::iterator BI_temp = sourceBB->begin(); BI_temp !=sourceBB->end(); ++BI_temp)
                  {
                    if(BI_temp->getOpcode() != Instruction::Br)
                    continue;

                    BranchInst* tempBrInst = dyn_cast<llvm::BranchInst>(BI_temp);
                    for(unsigned ii=0; ii < tempBrInst->getNumSuccessors(); ii++)
                    {
                      if(tempBrInst->getSuccessor(ii) == destinationBB)
                      tempBrInst->setSuccessor(ii, newBB);
                    }
                  }

                  // insert branch to destinationBB
                  BranchInst *newBranchInst;
                  newBranchInst->Create(destinationBB, newBB);

                  map_liveout_destnBB_newBB[destinationBB] = newBB;
                }

                TerminatorInst *TI = newBB->getTerminator();
                loadGlobal = new LoadInst(ptr, "", TI);
                map_replace_liveout_use[Inst] = loadGlobal;
              }
              else
              {
                // Make all instructions that referred to BI now refer to loadGlobal as their source.
                if(!loadInserted)
                {
                  loadGlobalNonPhi = new LoadInst(ptr, storeVariable, Inst); //previously ptr->getName()
                  loadInserted = true;
                }
                map_replace_liveout_use[Inst] = loadGlobalNonPhi;
              }
          }
          IRChanged = true;
        }
      }

      // replace use with load instruction
      std::map<Instruction*, Instruction*>::iterator itt;
      for(itt = map_replace_liveout_use.begin(); itt != map_replace_liveout_use.end(); itt++)
      {
        (itt->first)->replaceUsesOfWith(BI,dyn_cast<Value>(itt->second));
      }

      return IRChanged;
    }

    bool Update_LiveIn_Variables(Instruction *BI, DFG* myDFG, std::vector<BasicBlock *> bbs, std::vector<BasicBlock *> LoopLatchExitBlks, BasicBlock *Preheader)
    {
      bool IRChanged = false;
      std::string storeVariable;

      for(Use &U : BI->operands()){  // U is of type Use &
        Value *v = U.get();
        Argument *arg = dyn_cast<Argument>(v);
        auto Inst = dyn_cast<Instruction>(v);
        if(Inst){
          // an instruction defines V
          // Ensure that the definition is outside of the BBs of loop
          if ( std::find(bbs.begin(), bbs.end(), Inst->getParent()) != bbs.end() )
            continue;
        }
        else if(arg)
        {
          Function *parentFunc = arg->getParent();
          if (parentFunc)
          {
            Inst = Preheader->getTerminator();
          }
          else
            continue;
        }
        else
          continue;

        GlobalValue* G = dyn_cast<llvm::GlobalValue>(v);
        if(G != NULL)
        {
          continue;
        }

        std::string gPtrName;
        TerminatorInst *TI = Inst->getParent()->getTerminator();
        std::map<Value *, std::string>::iterator it;
        it = map_instn_defn_livein_to_load.find(v);
        if (it != map_instn_defn_livein_to_load.end() )
          continue;

        llvm::Type* T = v->getType();
        GlobalVariable *gPtr;
        unsigned alignment = 4;

        if(T->isIntegerTy() || T->isFloatingPointTy())
        {
          gPtrName = "gVar" + std::to_string(++gVarNo);
          // Create new global pointer
          gPtr = new GlobalVariable(*M, T, false, GlobalValue::CommonLinkage, 0, gPtrName);
          Constant *constVal = Constant::getNullValue(T);
          gPtr->setInitializer(constVal);
          alignment = (T->getPrimitiveSizeInBits())/8;
        }
        else if(T->isPointerTy())
        {
          gPtrName = "gPtr" + std::to_string(++gPtrNo);
          PointerType* PointerTy_0 = dyn_cast<PointerType>(T);// PointerType::get(T, 0);
          gPtr = new GlobalVariable(*M, T, false, GlobalValue::CommonLinkage, 0, gPtrName);
          ConstantPointerNull* const_ptr_2 = ConstantPointerNull::get(PointerTy_0);
          gPtr->setInitializer(const_ptr_2);
          /* Assumption is that Pointers will occupy 4 bytes for 32-bit system */
          alignment = T->getPointerAddressSpace();
          if(alignment < 4) alignment = 4;
        }
        if(alignment == 0)
          errs() << "Cannot find size of the memory access for live-in variable\n";
        gPtr->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);

        // Store Value to Pointer
        StoreInst *newStoreInstn = new StoreInst(v,gPtr,TI);

        // Load value back from Global Pointer Inside Loop Block
        map_instn_defn_livein_to_load[v] = gPtrName;
        map_livein_to_alignment[gPtrName] = alignment;
      }
      return IRChanged;
    }

    //make a new node from instruction BI and adds it to DFG
    bool Add_Node(Instruction *BI, DFG* myDFG, BasicBlock* loopHeader)
    {
      NODE* node, *node2;
      bool retVal = false;
      std::string name="";
      unsigned alignment;

      switch (BI->getOpcode())
      {
        // Terminator Instructions
        case Instruction::Ret:
          errs() << "\n\nReturn Detected!\n\n";
          return false;
        case Instruction::Br:
          retVal = true;
          return retVal;
        case Instruction::Switch:
          errs() << "\n\nSwitch Detected!\n\n";
          return false;
        case Instruction::IndirectBr:
          errs() << "\n\nIndirectBr Detected!\n\n";
          return false;
        case Instruction::Invoke:
          retVal = false;
          errs() << "\n\nInvoke Detected!\n\n";
          return retVal;
        case Instruction::Resume:
          retVal = false;
          errs() << "\n\nResume Detected!\n\n";
          return retVal;
        case Instruction::CatchSwitch:
          retVal = false;
          errs() << "\n\ncatchswitch Detected!\n\n";
          return retVal;
        case Instruction::CatchRet:
          retVal = false;
          errs() << "\n\ncatchret Detected!\n\n";
          return retVal;
        case Instruction::CleanupRet:
          retVal = false;
          errs() << "\n\ncleanupret Detected!\n\n";
          return retVal;
        case Instruction::Unreachable:
          retVal = false;
          errs() << "\n\nUnreachable Detected!\n\n";
          return retVal;

        // Standard binary operators...
        case Instruction::Add:
        case Instruction::FAdd:
          node = new NODE(add, 1, NodeID++, BI->getName().str(), BI);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::Sub:
        case Instruction::FSub:
          node = new NODE(sub, 1, NodeID++, BI->getName().str(), BI);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::Mul:
        case Instruction::FMul:
          node = new NODE(mult, 1, NodeID++, BI->getName().str(), BI);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::UDiv:
        case Instruction::SDiv:
        case Instruction::FDiv:
          node = new NODE(div, 1, NodeID++, BI->getName().str(), BI);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::URem:
        case Instruction::SRem:
        case Instruction::FRem:
          node = new NODE(rem, 1, NodeID++, BI->getName().str(), BI);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;

        // Bitwise Binary Operations
        case Instruction::Shl:
          node = new NODE(shiftl, 1, NodeID++, BI->getName().str(), BI);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::LShr:
          node = new NODE(shiftr_logical, 1, NodeID++, BI->getName().str(), BI);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::AShr:
          node = new NODE(shiftr, 1, NodeID++, BI->getName().str(), BI);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::And:
          node = new NODE(andop, 1, NodeID++, BI->getName().str(), BI);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::Or:
          node = new NODE(orop, 1, NodeID++, BI->getName().str(), BI);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::Xor:
          node = new NODE(xorop, 1, NodeID++, BI->getName().str(), BI);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;

        // Memory instructions...
        case Instruction::Alloca:
          retVal = true;
          return retVal;
        case Instruction::Load:
          /* Check Type */
          node = new NODE(ld_add, 1, NodeID++, BI->getName().str(), BI);
          node2 = new NODE(ld_data, 1, NodeID++, BI->getName().str(), BI);
          node->set_Load_Data_Bus_Read(node2);
          node2->set_Load_Address_Generator(node);
          LoadInst *tempLoadInst;
          tempLoadInst = dyn_cast<LoadInst>(BI);
          alignment = tempLoadInst->getAlignment();
          node->setAlignment(alignment);
          myDFG->insert_Node(node);
          myDFG->insert_Node(node2);
          for(unsigned ii=0; ii<BI->getNumOperands(); ii++)
          {
            GetElementPtrInst *temp = dyn_cast<GetElementPtrInst>(BI->getOperand(ii));
            if(temp)
            {
              idxprom_nodeID_load_alignment_map[temp]=alignment;
              break;
            }
          }
          retVal = true;
          return retVal;
        case Instruction::Store:
          node = new NODE(st_add, 1, NodeID++, BI->getName().str(), BI);
          node2 = new NODE(st_data, 1, NodeID++, BI->getName().str(), BI);
          node->set_Store_Data_Bus_Write(node2);
          node2->set_Store_Address_Generator(node);
          StoreInst *tempStoreInst;
          tempStoreInst = dyn_cast<StoreInst>(BI);
          alignment = tempStoreInst->getAlignment();
          node->setAlignment(alignment);
          myDFG->insert_Node(node);
          myDFG->insert_Node(node2);
          for(unsigned ii=0; ii<BI->getNumOperands(); ii++)
          {
            GetElementPtrInst *temp = dyn_cast<GetElementPtrInst>(BI->getOperand(ii));
            if(temp)
            {
              idxprom_nodeID_load_alignment_map[temp]=alignment;
              break;
            }
          }
          retVal = true;
          return retVal;
        case Instruction::AtomicCmpXchg:
          retVal = false;
          errs() << "\n\nAtomicCmpXchg Detected!\n\n";
          return retVal;
        case Instruction::AtomicRMW:
          retVal = false;
          errs() << "\n\nAtomicRMW Detected!\n\n";
          return retVal;
        case Instruction::Fence:
          retVal = false;
          errs() << "\n\nFence Detected!\n\n";
          return retVal;
        case Instruction::GetElementPtr:
          node = new NODE(add, 1, NodeID++, BI->getName().str(),BI);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;

        // Convert instructions...
        case Instruction::Trunc:
        case Instruction::ZExt:
          node = new NODE(andop, 1, NodeID++, BI->getName().str(), BI);
          myDFG->insert_Node(node);
          return true;
        case Instruction::SExt:
          node = new NODE(sext, 1, NodeID++, BI->getName().str(), BI);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::FPTrunc:
        case Instruction::FPExt:
        case Instruction::FPToUI:
        case Instruction::FPToSI:
        case Instruction::UIToFP:
        case Instruction::SIToFP:
        case Instruction::IntToPtr:
        case Instruction::PtrToInt:
        case Instruction::BitCast:
          retVal = false;
          errs() << "\n\nConvert Instruction Detected!\n\n";
          return retVal;
        // Other instructions...
        case Instruction::ICmp:
        case Instruction::FCmp:
          llvm::Instruction_Operation op;
          if (cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::FCMP_OEQ
          || cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::FCMP_UEQ
          || cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::ICMP_EQ)
          {
            op = cmpEQ;
          }
          else if (cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::FCMP_ONE
          || cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::FCMP_UNE
          || cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::ICMP_NE)
          {
            op = cmpNEQ;
          }
          else if (cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::FCMP_OGT
          || cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::FCMP_UGT
          || cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::ICMP_SGT)
          {
            op = cmpSGT;
          }
          else if (cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::ICMP_UGT)
          {
            op = cmpUGT;
          }
          else if (cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::FCMP_OGE
          || cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::FCMP_UGE
          || cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::ICMP_SGE)
          {
            op = cmpSGEQ;
          }
          else if (cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::ICMP_UGE)
          {
            op = cmpUGEQ;
          }

          else if (cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::FCMP_OLT
          || cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::FCMP_ULT
          || cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::ICMP_SLT)
          {
            op = cmpSLT;
          }
          else if (cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::ICMP_ULT)
          {
            op = cmpULT;
          }
          else if (cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::FCMP_OLE
          || cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::FCMP_ULE
          || cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::ICMP_SLE)
          {
            op = cmpSLEQ;
          }
          else if (cast<CmpInst>(BI)->getPredicate() == llvm::CmpInst::ICMP_ULE)
          {
            op = cmpULEQ;
          }
          node = new NODE(op, 1, NodeID++, BI->getName().str(), BI);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::PHI:
        //TODO Optimize Phi's here
        /* Phi's are generated to select inputs from multiple blocks.
        We can treat a Phi node (in loopHeader block) as a special case;
        select live-in value for the first time, and recurrent value otherwise
        If Phi is not in loopHeader block, treat it as a select instruction
        */
        {
          node = new NODE(select, 1, NodeID++, BI->getName().str(), BI);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        }
        case Instruction::Select:
          node = new NODE(cond_select, 1, NodeID++, BI->getName().str(), BI);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;

        // Vector Operations
        case Instruction::ExtractElement:
        case Instruction::InsertElement:
        case Instruction::ShuffleVector:
          retVal = false;
          errs() << "\n\nVector Instruction Detected!\n\n";
          return retVal;

        // Aggregate Operations
        case Instruction::ExtractValue:
        case Instruction::InsertValue:
          retVal = false;
          errs() << "\n\nAggregate Instruction Detected!\n\n";
          return retVal;

        // Other Operations
        case Instruction::Call:
          retVal = false;
          errs() << "\n\nCall Detected!\n\n";
          return retVal;
        case Instruction::VAArg:
          retVal = false;
          errs() << "\n\nva_arg Detected!\n\n";
          return retVal;
        case Instruction::LandingPad:
          retVal = false;
          errs() << "\n\nLandingPad Detected!\n\n";
          return retVal;
        case Instruction::CatchPad:
          retVal = false;
          errs() << "\n\nCatchPad Detected!\n\n";
          return retVal;
        case Instruction::CleanupPad:
          retVal = false;
          errs() << "\n\nCleanupPad Detected!\n\n";
          return retVal;
      }
      errs() << "\n\nUndefined Instruction Detected\n\n";
      return retVal;
    }

    //return true if set contains current basicblock
    bool contains(BasicBlock* current, std::vector<BasicBlock*> set)
    {
      for (std::vector<BasicBlock*>::const_iterator it = set.begin() ; it != set.end(); ++it)
      {
        if (current->getName().str() == (*it)->getName().str())
          return true;
      }
      return false;
    }

    Instruction* condInst;
    BranchInst* brInst;

    BasicBlock * SearchCommonParentForBranching(BasicBlock *bb1, BasicBlock *bb2, std::vector<BasicBlock *> bbs)
    {
      BasicBlock* retBlk = NULL;

      if(bb1->getSinglePredecessor() == bb2->getSinglePredecessor())
      {
        BasicBlock *tempBB = bb1->getSinglePredecessor();
        for(BasicBlock::iterator BI_temp = tempBB->begin(); BI_temp !=tempBB->end(); ++BI_temp)
        {
          if(BI_temp->getOpcode() != Instruction::Br)
            continue;

          BranchInst* tempBrInst = dyn_cast<llvm::BranchInst>(BI_temp);
          if(tempBrInst->isUnconditional()) break;
          brInst = tempBrInst;
          condInst = dyn_cast<llvm::Instruction>(tempBrInst->getCondition());
          retBlk = bb1->getSinglePredecessor();
        }
      }
      else if(bb1->getSinglePredecessor() == bb2)
      {
        BasicBlock *tempBB = bb2;
        for(BasicBlock::iterator BI_temp = tempBB->begin(); BI_temp !=tempBB->end(); ++BI_temp)
        {
          if(BI_temp->getOpcode() != Instruction::Br)
            continue;

          BranchInst* tempBrInst = dyn_cast<llvm::BranchInst>(BI_temp);
          if(tempBrInst->isUnconditional()) break;
          brInst = tempBrInst;
          condInst = dyn_cast<llvm::Instruction>(tempBrInst->getCondition());
          retBlk = bb2;
        }
      }
      else if(bb1 == bb2->getSinglePredecessor())
      {
        BasicBlock *tempBB = bb1;
        for(BasicBlock::iterator BI_temp = tempBB->begin(); BI_temp !=tempBB->end(); ++BI_temp)
        {
          if(BI_temp->getOpcode() != Instruction::Br)
            continue;

          BranchInst* tempBrInst = dyn_cast<llvm::BranchInst>(BI_temp);
          if(tempBrInst->isUnconditional()) break;
          brInst = tempBrInst;
          condInst = dyn_cast<llvm::Instruction>(tempBrInst->getCondition());
          retBlk = bb1;
        }
      }
      else
      {
        bool found = false;
        for (unsigned int ii = 0; ii < bbs.size(); ii++)
        {
          if(found) break;
          for (BasicBlock::iterator BBI = bbs[ii]->begin(); BBI != bbs[ii]->end(); ++BBI)
          {
            if(BBI->getOpcode() != Instruction::Br)
              continue;
            BranchInst * tempBrInst = (dyn_cast<llvm::BranchInst>(BBI));
            if(tempBrInst->isUnconditional()) break;
            if( (tempBrInst->getSuccessor(0) == bb1) &&
                (tempBrInst->getSuccessor(1) == bb2))
            {
              brInst = tempBrInst;
              condInst = dyn_cast<llvm::Instruction>(tempBrInst->getCondition());
              retBlk = bbs[ii];
              found = true;
              break;
            }
          }
        }
      }
      return retBlk;
    }

    BranchInst * getConditionalBranch(BasicBlock *bb)
    {
      BranchInst* tempBrInst;
      for(BasicBlock::iterator BI_temp = bb->begin(); BI_temp !=bb->end(); ++BI_temp)
      {
        if(BI_temp->getOpcode() != Instruction::Br)
          continue;

        tempBrInst = dyn_cast<llvm::BranchInst>(BI_temp);
        if(tempBrInst->isUnconditional()) return NULL;
        return tempBrInst;
      }
      return tempBrInst;
    }

    std::vector<BasicBlock *> ifBlocks;
    std::vector<BasicBlock *> elseBlocks;
    std::vector<BasicBlock *> condBlocks;
    std::vector<BranchInst *> condSet;
    std::vector<BasicBlock *> succBlocks;

    void collectAllBranchInfo(std::vector<BasicBlock *> bbs, BasicBlock* loopLatch)
    {
      for (unsigned int ii = 0; ii < bbs.size(); ii++)
      {
        if(bbs[ii] == loopLatch) continue;
        if(bbs[ii]->getSingleSuccessor() != NULL) continue;
        std::vector<BasicBlock*> bbSucc;
        for (succ_iterator SI = succ_begin(bbs[ii]), SE = succ_end(bbs[ii]); SI != SE; ++SI)
          bbSucc.push_back(*SI);
        std::vector<BasicBlock*>::iterator it = find(bbSucc.begin(), bbSucc.end(), bbs[ii]);
        if (it != bbSucc.end())
            continue;

        for (BasicBlock::iterator BBI = bbs[ii]->begin(); BBI != bbs[ii]->end(); ++BBI)
        {
          if(BBI->getOpcode() != Instruction::Br)
            continue;
          BranchInst * tempBrInst = (dyn_cast<llvm::BranchInst>(BBI));
          if(tempBrInst->isUnconditional()) break;

          BasicBlock *succ1 = tempBrInst->getSuccessor(0);
          BasicBlock *succ2 = tempBrInst->getSuccessor(1);
          BasicBlock *commonSuccessor = SearchCommonSuccessorForBranching(bbs[ii],succ1,succ2,loopLatch);
          assert((commonSuccessor != NULL) && "Error while finding common successor");

          ifBlocks.push_back(succ1);
          elseBlocks.push_back(succ2);
          condBlocks.push_back(bbs[ii]);
          condSet.push_back(tempBrInst);
          succBlocks.push_back(commonSuccessor);
          break;
        }
      }
      /*
      for(unsigned int ii = 0; ii < condBlocks.size(); ii++)
      {
        std::cout << ii << "  " << condBlocks[ii]->begin()->getOpcodeName() << "\t" <<
          ifBlocks[ii]->begin()->getOpcodeName() << "\t" << elseBlocks[ii]->begin()->getOpcodeName() <<
          "\t" << succBlocks[ii]->begin()->getOpcodeName() << "\n";
      } */
    }

    void updateStoresInConditionalBBs(BasicBlock *loopLatch, BasicBlock *loopHeader, std::vector<BasicBlock *> bbs, DFG* myDFG)
    {
      // Global array and variable names for which
      // there is at least a store in conditional path
      std::vector<std::string> storeVarNames;
      std::vector<int> varFound;

      for(unsigned int ii = 0; ii < condBlocks.size(); ii++)
      {
        std::vector<bool> varLoaded;
        std::vector<Instruction*> variableValue;
        std::vector<Value*> variablePtr;
        std::vector<unsigned> variableAlignment;
        std::vector<std::string> storeArrNames;
        std::vector<int> arrayFound;
        std::vector<Instruction *> arrayIndex;

        for(unsigned int it = 0; it < 2; it++)
        {
          BasicBlock *current;
          if(it == 0)
            current = ifBlocks[ii];
          else
            current = elseBlocks[ii];

          if(current == succBlocks[ii]) continue;

          while(current != loopLatch)
          {
            if(current == succBlocks[ii]) break;
            for (BasicBlock::iterator BBI = current->begin(); BBI != current->end(); ++BBI)
            {
              if(BBI->getOpcode() == Instruction::Br)
              {
                BranchInst * tempBrInst = (dyn_cast<llvm::BranchInst>(BBI));
                if(tempBrInst->isUnconditional())
                  current = current->getSingleSuccessor();
                else
                  current = SearchCommonSuccessorForBranching(current,tempBrInst->getSuccessor(0), tempBrInst->getSuccessor(1),loopLatch);
                break;
              }
              else if(BBI->getOpcode() == Instruction::Store)
              {
                StoreInst *stInst = (dyn_cast<llvm::StoreInst>(BBI));
                // TODO
                Instruction *ptrOperand = (dyn_cast<Instruction>(stInst->getPointerOperand()));
                if(ptrOperand != NULL)
                {
                  GetElementPtrInst *ptrInst = dyn_cast<GetElementPtrInst>(stInst->getPointerOperand());
                  std::string varName = ptrInst->getPointerOperand()->getName().str();
                  if(varName.empty())
                    varName = myDFG->get_Node(stInst->getPointerOperand())->get_Name();

                  std::vector<std::string>::iterator itt = std::find(storeVarNames.begin(), storeVarNames.end(), varName);
                  if(itt == storeVarNames.end())
                  {
                    storeVarNames.push_back(varName);
                    varFound.push_back(it);
                    variablePtr.push_back(stInst->getPointerOperand());
                    variableAlignment.push_back(stInst->getAlignment());
                  }
                  else
                  {
                    varFound[std::distance(storeVarNames.begin(), itt)] = 2;
                  }
                }
                else
                {
                  std::string varName = stInst->getPointerOperand()->getName().str();
                  if(varName.empty()) varName = myDFG->get_Node(stInst->getPointerOperand())->get_Name();
                  assert( !varName.empty() && "Variable does not have name! Debug!\n");
                  std::vector<std::string>::iterator itt = std::find(storeVarNames.begin(), storeVarNames.end(), varName);
                  if(itt == storeVarNames.end())
                  {
                    storeVarNames.push_back(varName);
                    varFound.push_back(it);
                    variablePtr.push_back(stInst->getPointerOperand());
                    variableAlignment.push_back(stInst->getAlignment());
                  }
                  else
                    varFound[std::distance(storeVarNames.begin(), itt)] = 2;
                }
              }
            }
          }
        }

        // Iterate for each variable or array
        for(unsigned int it = 0; it < storeVarNames.size(); it++)
        {
          bool presentInBothBlks = (varFound[it] == 2);
          Value *op1, *op2;
          StoreInst *stInst;

          for(unsigned int itt = 0; itt < 2; itt++)
          {
            BasicBlock *current;
            if(itt == 0)
              current = ifBlocks[ii];
            else
              current = elseBlocks[ii];

            if(current == succBlocks[ii])
              continue;

            while(current != loopLatch)
            {
              if(current == succBlocks[ii]) break;
              for (BasicBlock::iterator BBI = current->begin(); BBI != current->end(); ++BBI)
              {
                if(BBI->getOpcode() == Instruction::Br)
                {
                  BranchInst * tempBrInst = (dyn_cast<llvm::BranchInst>(BBI));
                  if(tempBrInst->isUnconditional())
                    current = current->getSingleSuccessor();
                  else
                    current = SearchCommonSuccessorForBranching(current,tempBrInst->getSuccessor(0), tempBrInst->getSuccessor(1),loopLatch);
                  break;
                }
                else if(BBI->getOpcode() == Instruction::Store)
                {
                  stInst = (dyn_cast<llvm::StoreInst>(BBI));
                  std::string stInstName = stInst->getPointerOperand()->getName().str();
                  if(stInstName.empty())
                    stInstName = myDFG->get_Node(stInst->getPointerOperand())->get_Name();
                  if(stInstName.compare(storeVarNames[it]) != 0)
                    continue;
                  if(!presentInBothBlks)
                  {
                    if(itt == 0)
                    {
                      op1 = stInst->getPointerOperand();
                      op2 = stInst;
                    }
                    else
                    {
                      op1 = stInst;
                      op2 = stInst->getPointerOperand();
                    }
                  }
                  else
                  {
                    if(itt == 0)
                      op1 = stInst->getPointerOperand();
                    else
                      op2 = stInst->getPointerOperand();
                  }
                }
              }
            }
          }

          NODE *node1 = myDFG->get_Node(stInst); //store_address node
          NODE *node2;                          //store_data node
          NODE *selectNode = new NODE(cond_select, 1, NodeID++, "", NULL);
          NODE *constInt;
          myDFG->insert_Node(selectNode);

          if(!presentInBothBlks)
          {
            std::string dummyStoreName;
            Value *v = stInst->getValueOperand();
            Type* T = v->getType();
            GlobalVariable *dummyStore;
            unsigned alignment = 4;

            dummyStoreName = "dmy_CGRAst_" + std::to_string(++dummyStoreVarNo);
            // Create new dummy variable
            dummyStore = new GlobalVariable(*M, T, false, GlobalValue::CommonLinkage, 0, dummyStoreName);
            constInt = new NODE(constant, 1, NodeID++, dummyStoreName, NULL);

            if(T->isIntegerTy() || T->isFloatingPointTy())
            {
              Constant *constVal = Constant::getNullValue(T);
              dummyStore->setInitializer(constVal);
              alignment = (T->getPrimitiveSizeInBits())/8;
            }
            else if(T->isPointerTy())
            {
              PointerType* PointerTy_0 = dyn_cast<PointerType>(T);// PointerType::get(T, 0);
              ConstantPointerNull* const_ptr_2 = ConstantPointerNull::get(PointerTy_0);
              dummyStore->setInitializer(const_ptr_2);
              alignment = T->getPointerAddressSpace();
              if(alignment < 4) alignment = 4;
            }
            dummyStore->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);
            myDFG->insert_Node(constInt);
          }

          NODE *input1 = myDFG->get_Node(op1);
          NODE *input2 = myDFG->get_Node(op2);
          NODE *condNode = myDFG->get_Node(Find_Last_CMP_Instruction(condBlocks[ii]));
          unsigned selectInput_opOrder;
          unsigned selectSelf_opOrder;
          ARC *arc1;

          if(node1->is_Store_Data_Bus_Write())
          {
            node2 = node1;
            node1 = node2->get_Related_Node();
          }
          else
            node2 = node1->get_Related_Node();

          if(op1 == stInst)
          {
            selectInput_opOrder = 1;
            selectSelf_opOrder = 0;
            myDFG->make_Arc(input2, selectNode, EdgeID++, 0, TrueDep, selectInput_opOrder);
            myDFG->make_Arc(constInt, selectNode, EdgeID++, 0, TrueDep, selectSelf_opOrder);
            arc1 = myDFG->get_Arc(input2, node1);
            myDFG->Remove_Arc(arc1);
          }
          else if(op2 == stInst)
          {
            selectInput_opOrder = 0;
            selectSelf_opOrder = 1;
            myDFG->make_Arc(input1, selectNode, EdgeID++, 0, TrueDep, selectInput_opOrder);
            myDFG->make_Arc(constInt, selectNode, EdgeID++, 0, TrueDep, selectSelf_opOrder);
            arc1 = myDFG->get_Arc(input1, node1);
            myDFG->Remove_Arc(arc1);
          }
          else
          {
            //"Store In Both Paths! Not Implemented\n";
            myDFG->make_Arc(input1, selectNode, EdgeID++, 0, TrueDep, 0);
            myDFG->make_Arc(input2, selectNode, EdgeID++, 0, TrueDep, 1);
            arc1 = myDFG->get_Arc(input1, node1);
            myDFG->Remove_Arc(arc1);
            arc1 = myDFG->get_Arc(input2, node1);
            myDFG->Remove_Arc(arc1);
          }

          myDFG->make_Arc(condNode, selectNode, EdgeID++, 0, PredDep, 2);
          myDFG->make_Arc(selectNode, node1, EdgeID++, 0, TrueDep, 0);
        }
      }
    }

    BasicBlock * SearchCommonSuccessorForBranching(BasicBlock *bb, BasicBlock *bb1, BasicBlock *bb2, BasicBlock *loopLatch)
    {
      BasicBlock* retBlk = NULL;
      if((bb1 == NULL) || (bb2 == NULL)) return retBlk;

      BasicBlock* succ1 = bb1->getSingleSuccessor();
      BasicBlock* succ2 = bb2->getSingleSuccessor();
      if((succ1 == succ2) && (succ1 != NULL))
      {
        retBlk = succ1;
      }
      else if(succ1 == bb2)
      {
        retBlk = bb2;
      }
      else if(bb1 == succ2)
      {
        retBlk = bb1;
      }
      else
      {
        if((succ1 == NULL) && (bb1 != loopLatch))
        {
          for (BasicBlock::iterator BBI = bb1->begin(); BBI != bb1->end(); ++BBI)
          {
            if(BBI->getOpcode() != Instruction::Br)
              continue;
            BranchInst * tempBrInst = (dyn_cast<llvm::BranchInst>(BBI));
            if(tempBrInst->isUnconditional()) break;
            succ1 = SearchCommonSuccessorForBranching(bb1,tempBrInst->getSuccessor(0), tempBrInst->getSuccessor(1),loopLatch);
            break;
          }
        }

        if((succ2 == NULL) && (bb2 != loopLatch))
        {
          for (BasicBlock::iterator BBI = bb2->begin(); BBI != bb2->end(); ++BBI)
          {
            if(BBI->getOpcode() != Instruction::Br)
              continue;
            BranchInst * tempBrInst = (dyn_cast<llvm::BranchInst>(BBI));
            if(tempBrInst->isUnconditional()) break;
            succ2 = SearchCommonSuccessorForBranching(bb2,tempBrInst->getSuccessor(0), tempBrInst->getSuccessor(1),loopLatch);
            break;
          }
        }

        if(succ1 == succ2)
          retBlk = succ1;
        else if(bb1 == succ2)
          retBlk = succ2;
        else if(bb1->getSingleSuccessor() == succ2)
          retBlk = succ2;
        else if(bb2 == succ1)
          retBlk = succ1;
        else if(bb2->getSingleSuccessor() == succ1)
            retBlk = succ1;
        else
          retBlk = NULL;
      }
      return retBlk;
    }

    bool Update_Data_Dependencies(Instruction *BI, DFG* myDFG, std::vector<BasicBlock *> bbs, BasicBlock* loopLatch, BasicBlock* loopHeader)
    {
      NODE* node, *node1, *node2, *node3, *node4;
      ARC* arc1;
      DataDepType dep = TrueDep;
      if (BI->getOpcode() == Instruction::Trunc)
      {
        Type* T = (dyn_cast<TruncInst>(BI))->getDestTy();
        unsigned mask = (1 << T->getPrimitiveSizeInBits()) - 1;
        node1 = myDFG->get_Node(BI);
        node2 = new NODE(constant, 1, NodeID++, "ConstInt"+std::to_string(mask), NULL);
        myDFG->insert_Node(node2);
        myDFG->make_Arc(node2, node1, EdgeID++, 0, TrueDep, 1);
      }

      if((BI->getOpcode() == Instruction::SExt))
      {
        node1 = myDFG->get_Node(BI);
        Value *v = dyn_cast<Value>(BI->getOperand(0));
        Type* T = v->getType();
        unsigned mask = T->getPrimitiveSizeInBits();
        node2 = new NODE(constant, 1, NodeID++, "ConstInt"+std::to_string(mask), NULL);
        myDFG->insert_Node(node2);
        myDFG->make_Arc(node2, node1, EdgeID++, 0, TrueDep, 1);
      }

      if (BI->getOpcode() == Instruction::ZExt)
      {
        Value *v = dyn_cast<Value>(BI->getOperand(0));
        Type* T = v->getType();
        unsigned mask = (1 << T->getPrimitiveSizeInBits()) - 1;
        node1 = myDFG->get_Node(BI);
        node2 = new NODE(constant, 1, NodeID++, "ConstInt"+std::to_string(mask), NULL);
        myDFG->insert_Node(node2);
        myDFG->make_Arc(node2, node1, EdgeID++, 0, TrueDep, 1);
      }

      for (unsigned int j = 0; j < BI->getNumOperands(); j++)
      {
        dep = TrueDep;
        //constant values can be immediate
        //if it is greater than immediate field; instruction generation should treat it as nonrecurring value
        if (BI->getOperand(j)->getValueID() == llvm::Value::ConstantIntVal)
        {
          std::ostringstream os;
          int constVal = 0;
          if(dyn_cast<llvm::ConstantInt>((BI)->getOperand(j))->getBitWidth() > 1)
            constVal = dyn_cast<llvm::ConstantInt>((BI)->getOperand(j))->getSExtValue();
          else
            constVal = dyn_cast<llvm::ConstantInt>((BI)->getOperand(j))->getZExtValue();
          os << constVal;
          std::string name = "ConstInt" + os.str();
          if (myDFG->get_Node((BI)->getOperand(j)) == NULL)
          {
            node1 = new NODE(constant, 1, NodeID++, name, (BI)->getOperand(j));
            myDFG->insert_Node(node1);
          }
          else
            node1 = myDFG->get_Node((BI)->getOperand(j));
          node2 = myDFG->get_Node(BI);
          if(BI->getOpcode() != Instruction::Store)
            myDFG->make_Arc(node1, node2, EdgeID++, 0, dep,j);
          continue;
        }
        else if (BI->getOperand(j)->getValueID() == llvm::Value::ConstantFPVal)
        {
          //for now we ignore FP constants
          std::ostringstream os;
          os << cast<ConstantFP>((BI)->getOperand(j))->getValueAPF().convertToFloat();
          std::string name = "ConstFP" + os.str();
          if (myDFG->get_Node((BI)->getOperand(j)) == NULL)
          {
            node1 = new NODE(constant, 1, NodeID++, name, (BI)->getOperand(j));
            myDFG->insert_Node(node1);
          }
          else
            node1 = myDFG->get_Node((BI)->getOperand(j));
          node2 = myDFG->get_Node(BI);
          if(BI->getOpcode() != Instruction::Store)
            myDFG->make_Arc(node1, node2, EdgeID++, 0, dep,j);
          continue;
        }
        else if (BI->getOperand(j)->getValueID() == llvm::Value::BasicBlockVal)
        {
          errs() << "\n\nBasicBlockVal Detected!\n\n";
          continue;
        }
        else if ((BI->getOperand(j)->getValueID() >= llvm::Value::InstructionVal) ||
                 (BI->getOperand(j)->getValueID() == llvm::Value::GlobalVariableVal) ||
                 (BI->getOperand(j)->getValueID() == llvm::Value::ArgumentVal))
        {
          if(BI->getOperand(j)->getValueID() == llvm::Value::GlobalVariableVal)
          {
            if(BI->getOpcode() != Instruction::Load && BI->getOpcode() != Instruction::Store)
              continue;
          }

          std::string name;
          name = cast<Instruction>((BI)->getOperand(j))->getName().str();
          int distance = 0;

          //if the node has not been created, create it, else get distance
          if (myDFG->get_Node((BI)->getOperand(j)) == NULL)
          {
            Value *defInst = BI->getOperand(j);
            std::map<Value *, std::string>::iterator it;
            it = map_instn_defn_livein_to_load.find(defInst);
            if ((it != map_instn_defn_livein_to_load.end()) ||
                (BI->getOperand(j)->getValueID() == llvm::Value::GlobalVariableVal))
            {
              std::string ptrName;
              unsigned alignment = 4;
              if (it != map_instn_defn_livein_to_load.end())
              {
                ptrName = it->second;
                alignment = map_livein_to_alignment[ptrName];
              }
              else
              {
                ptrName = name;
              }

              node1 = new NODE(constant, 1, NodeID++, ptrName, (BI)->getOperand(j));
              node1->setAlignment(alignment);
              myDFG->insert_Node(node1);
              node2 = myDFG->get_Node(BI);
              dep = LiveInDataDep;
              unsigned int CGRA_loadAddID = ((NodeID++)+100);
              unsigned int CGRA_loadDataID = ((NodeID++)+100);
              // Update LiveIn information
              liveInNodefile << node2->get_ID() << "\t" << node2->get_Instruction() << "\t" << node2->get_Name() << "\n";
              liveInNodefile << node1->get_ID() << "\t" << constant << "\t" << ptrName << "\n";
              liveInNodefile << CGRA_loadAddID << "\t" << ld_add << "\t" << "ld_add_" + ptrName << "\n";
              liveInNodefile << CGRA_loadDataID << "\t" << ld_data << "\t" << "ld_data_" + ptrName << "\n";

              liveInEdgefile << CGRA_loadAddID << "\t" << CGRA_loadDataID << "\t0\tLRE\t0\n";
              liveInEdgefile << CGRA_loadDataID << "\t" << node2->get_ID() << "\t0\tTRU\t" << j << "\n";
              liveInEdgefile << node1->get_ID() << "\t" << CGRA_loadAddID << "\t0\tTRU\t0\n";
            }
            else
            {
              std::cout << "Cannot detect defining instruction for the live-in\n\n";
            }
          }
          else
          {
            distance = getDistance(cast<Instruction>((BI)->getOperand(j)),
            cast<Instruction>((BI)->getOperand(j))->getParent(), BI, BI->getParent(), bbs,loopLatch);
          }

          //if the operand instruction corresponds to a load operation
          if (cast<Instruction>((BI)->getOperand(j))->getOpcode() == Instruction::Load)
          {
            node1 = myDFG->get_Node_Mem_Data(cast<Instruction>((BI)->getOperand(j)));
            if (node1 == NULL)
            {
              node1 = myDFG->get_Node((BI)->getOperand(j));
            }
          }
          else
          {
            node1 = myDFG->get_Node((BI)->getOperand(j));
          }

          //if its a store instruction
          if (BI->getOpcode() == Instruction::Store)
          {
            if((dyn_cast<StoreInst>(BI))->getPointerOperand() == BI->getOperand(j))
            {
              node2 = myDFG->get_Node_Mem_Add(BI);
              if(((dyn_cast<StoreInst>(BI))->getValueOperand()->getValueID() == llvm::Value::ConstantIntVal) || ((dyn_cast<StoreInst>(BI))->getValueOperand()->getValueID() == llvm::Value::ConstantFPVal)) {
                node3 = myDFG->get_Node((dyn_cast<StoreInst>(BI))->getValueOperand());
                node4 = myDFG->get_Node_Mem_Data(BI);
                myDFG->make_Arc(node3, node4, EdgeID++, 0, TrueDep,0);
              }
            }
            else
            {
              node2 = myDFG->get_Node_Mem_Data(BI);
              if((dyn_cast<StoreInst>(BI))->getPointerOperand()->getValueID() == llvm::Value::ConstantIntVal) {
                node3 = myDFG->get_Node((dyn_cast<StoreInst>(BI))->getPointerOperand());
                node4 = myDFG->get_Node_Mem_Add(BI);
                myDFG->make_Arc(node3, node4, EdgeID++, 0, TrueDep,0);
              }
            }
          }
          else if (BI->getOpcode() == Instruction::Load)
          {
            node2 = myDFG->get_Node_Mem_Add(BI);
          }
          else
          {
            node2 = myDFG->get_Node(BI);
          }
          myDFG->make_Arc(node1, node2, EdgeID++, distance,dep,j);
        }
      }

      if(BI->getOpcode() == Instruction::Select)
      {
        //change OperandOrder To 1
        node1 = myDFG->get_Node(BI);
        node = myDFG->get_Node((dyn_cast<SelectInst>(BI))->getCondition());
        arc1 = myDFG->get_Arc(node, node1);
        arc1->SetOperandOrder(2);
        arc1->Set_Dependency_Type(PredDep);
        node = myDFG->get_Node((dyn_cast<SelectInst>(BI))->getTrueValue());
        arc1 = myDFG->get_Arc(node, node1);
        arc1->SetOperandOrder(0);
        node = myDFG->get_Node((dyn_cast<SelectInst>(BI))->getFalseValue());
        arc1 = myDFG->get_Arc(node, node1);
        arc1->SetOperandOrder(1);
      }

      /* Shail says:
      It is assumed that array address calculation should be independent of indvars or i, in general.
      getelementptr in IR shows how we can obtain base address and offsets. (LLVM Reference Manual)
      In obtained IR, it is always seen that 1st operand is the base address, 2nd is constant 0 (i.e. offset)
      And 3rd operand is the offset variable i.e. indsvars or i.
      Thus, we should eliminate constant0  and do 1st operand + (4 * 3rd Operand)
      */
      if(BI->getOpcode() == Instruction::GetElementPtr)
      {
        ArrID++;
        std::ostringstream arrid;
        arrid << ArrID;
        for(unsigned int i=0; i< BI->getNumOperands(); ++i)
        {
          //Fix Predecessor Nodes With idxprom
          node1 = myDFG->get_Node(BI);
          std::string operandname = BI->getOperand(i)->getName().str();
          //Add Node If Unnamed Private
          if(!operandname.empty())
          {
            std::string str = BI->getOperand(i)->getName().str();
            node2 = new NODE(constant, 1, NodeID++, str , NULL);
            myDFG->insert_Node(node2);
            myDFG->make_Arc(node2, node1, EdgeID++, 0, TrueDep, 0);
          }
          else
            node2 = myDFG->get_Node(BI->getOperand(i));

          if(node2->is_Load_Address_Generator())
            node2 = node2->get_Related_Node();

          if(node2->get_Instruction() == mult)
          {
            std::vector<NODE*> prevNodes = node2->Get_Prev_Nodes();
            for(unsigned int n=0; n<prevNodes.size(); n++)
            {
              if(prevNodes[n]->get_Name().find("ConstInt") != std::string::npos)
              {
                arc1 = myDFG->get_Arc(prevNodes[n], node2);
                if(arc1 != NULL)
                  arc1->SetOperandOrder(1);
              }
              else
              {
                arc1 = myDFG->get_Arc(prevNodes[n], node2);
                if(arc1 != NULL)
                  arc1->SetOperandOrder(0);
              }
            }
          }

          //Fix Insertion Of idxprom, if not there
          if(i==2)
          {
            node1 = myDFG->get_Node(BI);
            node = node2;

            //If already aligned through sext in IR with %idxprom, skip
            if(node->get_Instruction() == mult)
            {
              //change OperandOrder To 1
              arc1 = myDFG->get_Arc(node, node1);
              if(arc1 != NULL)
                arc1->SetOperandOrder(1);
              break;
            }

            if(idxprom_nodeID_pred_map.find(node->get_ID()) != idxprom_nodeID_pred_map.end())
            {
              arc1 = myDFG->get_Arc(node, node1);
              myDFG->Remove_Arc(arc1);
              node3 = myDFG->get_Node(idxprom_nodeID_pred_map[node->get_ID()]);
              myDFG->make_Arc(node, node3, EdgeID++, 0, TrueDep, 0);
              myDFG->make_Arc(node3, node1, EdgeID++, 0, TrueDep, 1);
            }
            else
            {
              node3 = new NODE(mult, 1, NodeID++, "idxprom"+arrid.str() , NULL);
              unsigned alignment = idxprom_nodeID_load_alignment_map[BI];
              node4 = new NODE(constant, 1, NodeID++, "ConstInt"+std::to_string(alignment), NULL);
              myDFG->insert_Node(node3);
              myDFG->insert_Node(node4);
              arc1 = myDFG->get_Arc(node, node1);
              myDFG->Remove_Arc(arc1);
              myDFG->make_Arc(node, node3, EdgeID++, 0, TrueDep, 0);
              myDFG->make_Arc(node4, node3, EdgeID++, 0, TrueDep, 1);
              myDFG->make_Arc(node3, node1, EdgeID++, 0, TrueDep, 1);
              idxprom_nodeID_pred_map.insert(std::pair<int,int>(node->get_ID(),node3->get_ID()));
            }
          }

          //Remove Unnecessary ConstInt0 Node
          if (node2->get_Name() == "ConstInt0")
          {
            node1 = myDFG->get_Node(BI);
            arc1 = myDFG->get_Arc(node2, node1);
            myDFG->Remove_Arc(arc1);
          }
        }
      }

      //Fix Successor Operand Order
      if((BI->getOpcode() == Instruction::Load) || (BI->getOpcode() == Instruction::Store))
      {
        node1 = myDFG->get_Node(BI);
        for(unsigned int i=0; i< BI->getNumOperands(); ++i)
        {
          Instruction *priorInst = (dyn_cast<Instruction>(BI->getOperand(i)));
          if(priorInst == NULL) continue;
          if(priorInst->getOpcode() == Instruction::GetElementPtr)
          {
            node2 = myDFG->get_Node(BI->getOperand(i));
            arc1 = myDFG->get_Arc(node2,node1);
            if(arc1 != NULL)
              arc1->SetOperandOrder(0);
          }
        }
      }

      if(BI->getOpcode() == Instruction::PHI)
      {
        node1 = myDFG->get_Node(BI);

        //If phi node is at top of the loop - it's not due to if-then-else
        //so avoid acting on phi node to supply predicate
        //This is because phi and selects have separate opcodes in backend
        //inputs selected through Phi is statically determined.
        if(BI->getParent() != loopHeader)
        {
          std::vector<BasicBlock*> bbList;
          std::vector<Instruction*> operandList;
          //Populate lists with the infromation from phi node
          for(unsigned int ii=0; ii < dyn_cast<PHINode>(BI)->getNumIncomingValues(); ii++)
          {
            bbList.push_back(dyn_cast<PHINode>(BI)->getIncomingBlock(ii));
            operandList.push_back(dyn_cast<Instruction>(BI->getOperand(ii)));
          }

          std::vector<NODE*> succofPhi;
          std::vector<int> succofPhiDistance;
          std::vector<DataDepType> succofPhiDepType;
          std::vector<int> succofPhiOpOrder;
          for(unsigned int ii=0; ii< BI->getNumOperands(); ++ii)
          {
            node2 = myDFG->get_Node(BI->getOperand(ii));
            arc1 = myDFG->get_Arc(node2,node1);
            if(arc1 != NULL)
              myDFG->Remove_Arc(arc1);
          }
          std::vector<ARC*> outGoingArcs = myDFG->getSetOfArcs();
          for(unsigned int ii = 0; ii < outGoingArcs.size(); ii++)
          {
            if(outGoingArcs[ii]->get_From_Node() == node1)
            {
              succofPhi.push_back(outGoingArcs[ii]->get_To_Node());
              succofPhiDistance.push_back(outGoingArcs[ii]->Get_Inter_Iteration_Distance());
              succofPhiDepType.push_back(outGoingArcs[ii]->Get_Dependency_Type());
              succofPhiOpOrder.push_back(outGoingArcs[ii]->GetOperandOrder());
            }
          }
          myDFG->delete_Node(node1);

          while(bbList.size() > 1)
          {
            for(unsigned int ii=0; ii < bbList.size(); ii++)
            {
              for(unsigned int jj=0; jj < bbList.size(); jj++)
              {
                std::string operandName;
                if(operandList[jj] != NULL) operandName = operandList[jj]->getOpcodeName();
              }
              for(unsigned int jj=0; jj < bbList.size(); jj++)
              {
                if(ii == jj) continue;
                bool skip = false;
                //Check that all successors are already operated.
                //Otherwise skip such pair (ii,jj)
                BasicBlock *succII = bbList[ii]->getSingleSuccessor();
                BasicBlock *succJJ = bbList[jj]->getSingleSuccessor();
                if(succII == NULL || succJJ == NULL)
                {
                  std::vector<BasicBlock*>::iterator it;
                  for (succ_iterator SI = succ_begin(bbList[ii]), SE = succ_end(bbList[ii]); SI != SE; ++SI)
                  {
                    it = find(bbList.begin(), bbList.end(), (*SI));
                    if (it != bbList.end()) {
                      skip = true;
                      break;
                    }
                  }

                  for (succ_iterator SI = succ_begin(bbList[jj]), SE = succ_end(bbList[jj]); SI != SE; ++SI)
                  {
                    it = find(bbList.begin(), bbList.end(), (*SI));
                    if (it != bbList.end()) {
                      skip = true;
                      break;
                    }
                  }

                  if((bbList[ii]->getSinglePredecessor() == bbList[jj]) ||
                     (bbList[jj]->getSinglePredecessor() == bbList[ii]))
                    skip = false;
                }

                if(skip) continue;
                BasicBlock *commonParent = SearchCommonParentForBranching(bbList[ii],bbList[jj],bbs);
                if(commonParent == NULL) continue;
                bbList.erase(bbList.begin()+ii);
                bbList.insert(bbList.begin()+ii,commonParent);
                bbList.erase(bbList.begin()+jj);

                if(dyn_cast<PHINode>(BI)->getNumIncomingValues() > 2)
                  node3 = new NODE(cond_select, 1, NodeID++, brInst->getName().str(), brInst);
                else
                  node3 = new NODE(cond_select, 1, NodeID++, brInst->getName().str(), BI);
                myDFG->insert_Node(node3);
                node2 = myDFG->get_Node(operandList[ii]);
                if(node2->is_Load_Address_Generator()) node2 = node2->get_Related_Node();
                myDFG->make_Arc(node2, node3, EdgeID++, 0, TrueDep, 0);
                node2 = myDFG->get_Node(operandList[jj]);
                if(node2->is_Load_Address_Generator()) node2 = node2->get_Related_Node();
                myDFG->make_Arc(node2, node3, EdgeID++, 0, TrueDep, 1);
                node2 = myDFG->get_Node(condInst);
                myDFG->make_Arc(node2, node3, EdgeID++, 0, PredDep, 2);

                if(dyn_cast<PHINode>(BI)->getNumIncomingValues() <= 2)
                {
                  for(unsigned int kk = 0; kk < succofPhi.size(); kk++)
                  {
                    myDFG->make_Arc(node3, succofPhi[kk], EdgeID++, succofPhiDistance[kk],
                          succofPhiDepType[kk], succofPhiOpOrder[kk]);
                  }
                }

                operandList.erase(operandList.begin()+ii);
                operandList.insert(operandList.begin()+ii,brInst);
                operandList.erase(operandList.begin()+jj);
                break;
              }
              if(bbList.size() == 1) break;
            }
          }

          BasicBlock *tempBB1 = dyn_cast<PHINode>(BI)->getIncomingBlock(0)->getSinglePredecessor();
          BasicBlock *tempBB2 = dyn_cast<PHINode>(BI)->getIncomingBlock(1)->getSinglePredecessor();
          if(tempBB1 == tempBB2)
          {
            BasicBlock *tempBB = tempBB1;
            for(BasicBlock::iterator BI_temp = tempBB->begin(); BI_temp !=tempBB->end(); ++BI_temp)
            {
              if(BI_temp->getOpcode() != Instruction::Br)
                continue;

              node2 = myDFG->get_Node((dyn_cast<llvm::BranchInst>(BI_temp))->getCondition());
            }
          }
          else
          {
            bool found = false;
            for (unsigned int ii = 0; ii < bbs.size(); ii++)
            {
              if(found) break;
              for (BasicBlock::iterator BBI = bbs[ii]->begin(); BBI != bbs[ii]->end(); ++BBI)
              {
                if(BBI->getOpcode() != Instruction::Br)
                  continue;
                BranchInst * tempInst = (dyn_cast<llvm::BranchInst>(BBI));
                if( (tempInst->getSuccessor(0) == (dyn_cast<Instruction>(BI->getOperand(0)))->getParent()) &&
                    (tempInst->getSuccessor(1) == (dyn_cast<Instruction>(BI->getOperand(1)))->getParent()))
                {
                  node2 = myDFG->get_Node(tempInst->getCondition());
                  found = true;
                  break;
                }
              }
            }
          }
          myDFG->make_Arc(node2, node3, EdgeID++, 0, PredDep, 2);

        }
      }

      return true;
    }

    bool canExecuteOnCGRA(Loop *L, std::vector<BasicBlock *> bbs, BasicBlock * Preheader, BasicBlock *ExitBlk)
    {
      bool retVal = true;
      unsigned depth = L->getLoopDepth();

      std::vector<Loop *> subLoops = L->getSubLoops();
      unsigned subLoopsize = subLoops.size();

      if(subLoopsize > 0)
      {
        std::cout << "1) Nested Loop with Depth " << depth << ". ";
        std::cout << "The loop contains " << subLoopsize << " SubLoops.\n";
        retVal = false;
      }

      SmallVector<BasicBlock*, 8> ExitingBlocks;
      L->getExitingBlocks(ExitingBlocks);
      if(ExitingBlocks.size() != 1)
      {
        std::cout << "2) The loop has " << ExitingBlocks.size() << " exits.\n";
        retVal = false;
      }

      unsigned totalInstructions=0;
      unsigned brInstructions = 0;
      unsigned functionCalls = 0;
      unsigned floatignPtInstructions = 0;
      unsigned vectorInstructions = 0;
      for (unsigned i = 0; i < (unsigned) bbs.size(); i++)
      {
        totalInstructions += bbs[i]->size();
        for (BasicBlock::iterator BI = bbs[i]->begin(); BI != bbs[i]->end(); ++BI)
        {
          if(BI->getOpcode() == Instruction::Br)
          {
            BranchInst* tempBrInst = dyn_cast<llvm::BranchInst>(BI);
            if(tempBrInst->isConditional())
              brInstructions++;
          }
          else if(BI->getOpcode() == Instruction::Call)
          {
            //TODO: Check function call can be inlined
            //Check if call to a library function can be separated from user defined function
            functionCalls++;
          }
          else if((BI->getOpcode() == Instruction::FAdd) ||
                  (BI->getOpcode() == Instruction::FSub) ||
                  (BI->getOpcode() == Instruction::FMul) ||
                  (BI->getOpcode() == Instruction::FDiv) ||
                  (BI->getOpcode() == Instruction::FRem) ||
                  (BI->getOpcode() == Instruction::FPTrunc))
                  // and FPCMP and more ...
          {
            floatignPtInstructions++;
          }
          else if((BI->getOpcode() == Instruction::ExtractElement) ||
                  (BI->getOpcode() == Instruction::InsertElement) ||
                  (BI->getOpcode() == Instruction::ShuffleVector))
          {
            vectorInstructions++;
          }
        }
      }

      // It is possible that vector instructions are in Preheader or in Exiting Block(s)
      for (BasicBlock::iterator BI = Preheader->begin(); BI != Preheader->end(); ++BI)
      {
        if((BI->getOpcode() == Instruction::ExtractElement) ||
                (BI->getOpcode() == Instruction::InsertElement) ||
                (BI->getOpcode() == Instruction::ShuffleVector))
        {
          vectorInstructions++;
        }
      }

      for (BasicBlock::iterator BI = ExitBlk->begin(); BI != ExitBlk->end(); ++BI)
      {
        if((BI->getOpcode() == Instruction::ExtractElement) ||
                (BI->getOpcode() == Instruction::InsertElement) ||
                (BI->getOpcode() == Instruction::ShuffleVector))
        {
          vectorInstructions++;
        }
      }

      if(totalInstructions > 30)
      {
        std::cout << "3) Complex Loop with More Instructions. Total Instructions:  " << totalInstructions << "\n";
        retVal = false;
      }

      if(brInstructions > 2)
      {
        std::cout << "4) Loop with Complex Control Flow. Total branches are: " << brInstructions << "\n";
        retVal = false;
      }

      if(functionCalls > 0)
      {
        std::cout << "5) Loop with total " << functionCalls << " Function Calls.\n";
        retVal = false;
      }

      if(floatignPtInstructions > 0)
      {
        std::cout << "6) Loop with " << floatignPtInstructions << " floating point operations.\n";
        retVal = false;
      }

      if(vectorInstructions > 0)
      {
        std::cout << "7) Loop with " << vectorInstructions << " vector instructions.\n";
        retVal = false;
      }

      if(retVal == false)
      {
        std::cout << "Cannot execute this loop on CGRA.\n";
        std::cout << "Compiling for execution on CPU.\n";
      }

      return retVal;
    }

    virtual bool runOnLoop(Loop *L, LPPassManager &LPM)
    {
      std::vector<BasicBlock *> bbs = L->getBlocks();
      bool retVal = false;
      ScalarEvolution * SE = &getAnalysis<ScalarEvolutionWrapperPass>().getSE();
      DominatorTree * DT = &getAnalysis<DominatorTreeWrapperPass>().getDomTree();

      if (HasCGRADisablePragma(L))
        return false;

      bool HasPragma = HasCGRAEnablePragma(L);
      if(!HasPragma)
        return false;

      // We can only remove the loop if there is a preheader that we can
      // branch from after removing it.
      BasicBlock *Preheader = L->getLoopPreheader();

      if (!Preheader) {
        return false;
      }

      // If LoopSimplify form is not available, stay out of trouble.
      if (!L->hasDedicatedExits()) {
        return false;
      }

      SmallVector<BasicBlock *, 4> ExitBlocks;
      L->getExitingBlocks(ExitBlocks);

      // We require that the loop only have a single exit block.  Otherwise, we'd
      // be in the situation of needing to be able to solve statically which exit
      // block will be branched to, or trying to preserve the branching logic in
      // a loop invariant manner.
      BasicBlock *ExitBlk = L->getUniqueExitBlock();
      if (!ExitBlk) {
        return false;
      }

      std::cout << "Loop has \tDepth=" << L->getLoopDepth() << "\tNumber of BBs=" << bbs.size() << " With one exiting branch\n";

      if(!canExecuteOnCGRA(L,bbs,Preheader,ExitBlk))
      {
        SetLoopAlreadyIterated(L);
        return false;
      }

      TotalLoops++;

      resetGlobalVariables();

      DFG *myDFG = new DFG();

      std::vector<Loop *> subLoops = L->getSubLoops();
      SmallVector<BasicBlock*, 8> ExitingBlocks;
      L->getExitingBlocks(ExitingBlocks);

      std::ostringstream osLoopID;
      osLoopID << TotalLoops;
      std::string directoryPath = "mkdir -p ./CGRAExec/L" + osLoopID.str();
      system(directoryPath.c_str());

      std::string newPath = "./CGRAExec/L" + osLoopID.str() + "/livein_node.txt";
      liveInNodefile.open(newPath.c_str());
      newPath = "./CGRAExec/L" + osLoopID.str() + "/livein_edge.txt";
      liveInEdgefile.open(newPath.c_str());

      for (int i = 0; i < (int) bbs.size(); i++)
      {
        for (BasicBlock::iterator BI = bbs[i]->begin(); BI != bbs[i]->end(); ++BI)
        {
          if (!Add_Node(&(*BI), myDFG, L->getHeader()))
            return false;
        }
      }

      for (int i = 0; i < (int) bbs.size(); i++)
      {
        for (BasicBlock::iterator BI = bbs[i]->begin(); BI != bbs[i]->end(); ++BI)
        {
          if (myDFG->get_Node(&(*BI)) != NULL)
          {
            SmallVector<BasicBlock *, 10> ExitBlks;
            std::vector<BasicBlock *> LoopExitBlks;
            L->getExitBlocks(ExitBlks);
            for(unsigned int itt = 0; itt < ExitBlks.size(); itt++)
            {
              LoopExitBlks.push_back(ExitBlks[itt]);
            }
            M = L->getLoopPreheader()->getParent()->getParent();
            retVal |= Update_LiveIn_Variables(&(*BI), myDFG, bbs, LoopExitBlks, Preheader);
          }
        }
      }

      for (int i = 0; i < (int) bbs.size(); i++)
      {
        for (BasicBlock::iterator BI = bbs[i]->begin(); BI != bbs[i]->end(); ++BI)
        {
          if (myDFG->get_Node(&(*BI)) != NULL)
          {
            if (!Update_Data_Dependencies(&(*BI), myDFG, bbs,L->getLoopLatch(), L->getHeader() ))
              return false;
          }
        }
      }

      liveInNodefile.close();
      liveInEdgefile.close();

      newPath = "./CGRAExec/L" + osLoopID.str() + "/liveout_node.txt";
      liveoutNodefile.open(newPath.c_str());
      newPath = "./CGRAExec/L" + osLoopID.str() + "/liveout_edge.txt";
      liveoutEdgefile.open(newPath.c_str());

      unsigned TripCount = calculateLoopTC(L, SE);
      if(TripCount != 0)
        dynamicTC = false;

      //Write TripCount in file
      std::ofstream lpTCfile;
      std::string tcfilename = "./CGRAExec/L" + osLoopID.str() + "/loop_iterations.txt";
      lpTCfile.open(tcfilename.c_str());
      lpTCfile << TripCount;
      lpTCfile.close();

      if(dynamicTC)
      {
        // TODO: If there are multiple exiting blocks, how to find the condition variable for dynamic TC loops
        // May be a hierarchy of select operations can help
        // In such case, need to exit even if a single condition is true, ensuring we exit from loop
        getLoopExitBranch(ExitBlocks[0]);

        // Need to determine if trip-count is known at run-time
        // If yes, generate corresponding variables and calculate TC
        // If not, pad unsafe operations with selection instructions
        if(isTCknownAtLoopEntry(bbs))
        {
          PHINode *indsvar = getInductionVariable(L,SE,DT);
          calculateTCDynamically(indsvar, bbs, Preheader);
          dynamicTC = false;
        }
      }

      for (int i = 0; i < (int) bbs.size(); i++)
      {
        for (BasicBlock::iterator BI = bbs[i]->begin(); BI != bbs[i]->end(); ++BI)
        {
          if (myDFG->get_Node(&(*BI)) != NULL)
          {
            SmallVector<BasicBlock *, 10> ExitBlks;
            std::vector<BasicBlock *> LoopExitBlks;
            L->getExitBlocks(ExitBlks);
            for(unsigned int itt = 0; itt < ExitBlks.size(); itt++)
            {
              LoopExitBlks.push_back(ExitBlks[itt]);
            }
            retVal |= Update_LiveOut_Variables(&(*BI), myDFG, bbs, LoopExitBlks);
          }
        }
      }

      liveoutNodefile.close();
      liveoutEdgefile.close();

      collectAllBranchInfo(bbs,L->getLoopLatch());
      updateStoresInConditionalBBs(L->getLoopLatch(),L->getHeader(),bbs, myDFG);

      std::ostringstream osNodeID;
      osNodeID << NodeID;
      myDFG->Dump_Loop("./CGRAExec/L" + osLoopID.str() + "/loop" + osNodeID.str());
      directoryPath = "loop_" + osNodeID.str();
      myDFG->Dot_Print_DFG(directoryPath.c_str());
      directoryPath = directoryPath + "DFG.dot";
      newPath = "./CGRAExec/L" + osLoopID.str() + "/" + directoryPath;
      std::rename(directoryPath.c_str(), newPath.c_str());

      delete myDFG;return true; //retVal;
    }
  };
}

char DDGGen::ID = 0;
static RegisterPass<DDGGen> X("DDGGen", "Loop Pass to Generate DDG for Loop Acceleration Over CGRA", false, true);
