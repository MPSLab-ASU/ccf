//===CondDDGGen.cpp - an LLVM Pass to generate Data Dependency Graph for Loops with conditionals for CGRAs"===//
// Author: Shail Dave, Mahesh Balasubramanian
// Last Modified on: April 15, 2018
//		   : Apri  15, 2019
//
// Author: Vinh Ta
// Last Modified on: March 25, 2022
// Update: Added awareness of loop control and live out nodes for scheduling constraint.
// Update: Added support for struct in C
//   NOTE: currently only support packed structs and up to 5 indices in GEP insts
//   TODO: Add support for non-packed structs (find padding information) and generalize indices in GEP
//===----------------------------------------------------------------------===//
//
// This file detects critical loops with conditionals annoted with label CGRA and generates
// DDG for coarse-grained reconfigurable array accelerators
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "cddg-gen"

#include "DFG.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "llvm/Analysis/GlobalsModRef.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/ScalarEvolutionAliasAnalysis.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/IR/Value.h"

//Edits by Arkan
#include "llvm/IR/IRBuilder.h"

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

#define DEBUG   1 
//DEBUG flag for update store
#define DEBUG1  1
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
std::map<std::string, Datatype> map_livein_to_datatype; 
std::map<std::string, unsigned> map_liveout_to_alignment;

std::map< BasicBlock*, BasicBlock* > map_liveout_destnBB_newBB;
std::map< Instruction*, Instruction*> map_replace_liveout_use;

std::vector<BasicBlock *> ifBlocks;
std::vector<BasicBlock *> elseBlocks;
std::vector<BasicBlock *> condBlocks;
std::vector<BranchInst *> condSet;
std::vector<BasicBlock *> succBlocks;

//Map containing idxprom nodes and predecessor
std::map<int,int> idxprom_nodeID_pred_map;
std::map<Instruction*,unsigned> idxprom_nodeID_load_alignment_map;

// Added by Vinh TA
// Support for struct, mainly used by getelementptr
// Map containing struct name and its size
std::map<std::string, unsigned> map_struct_size;
// Map containing struct name and its member sizes in declaration order
std::map<std::string, std::vector<int>> map_struct_member_size;
// Map from operand ID to node holding its address
std::map<int, NODE*> map_operand_node;

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
  struct CondDDGGen: public LoopPass
  {
    static char ID;        // Pass identification, replacement for typeid
    Function *hook; 
    CondDDGGen() :
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
      map_livein_to_datatype.clear();
      map_liveout_to_alignment.clear();
      map_liveout_destnBB_newBB.clear();
      map_replace_liveout_use.clear();
      idxprom_nodeID_pred_map.clear();
      idxprom_nodeID_load_alignment_map.clear();

      map_struct_size.clear();
      map_struct_member_size.clear();
      map_operand_node.clear();

      ifBlocks.clear();
      elseBlocks.clear();
      condBlocks.clear();
      condSet.clear();
      succBlocks.clear();
    }

    bool isTCknownAtLoopEntry(std::vector<BasicBlock *> bbs)
    {
      if(DEBUG)
	     errs() << "inside isTCknownatLoopEntry\n";

      Value *val1 = loopExitCond->getOperand(0);
      if(DEBUG) errs() << "val: " << val1->getName() << "\n";
      Instruction *Op1 = dyn_cast<Instruction>(val1);
      Instruction *Op2 = dyn_cast<Instruction>(loopExitCond->getOperand(1));
      //if(DEBUG) errs() << "op1: " << *Op1 << "\top2:" << *Op2 << "\n";
      //if(DEBUG) exit(1);
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
      if(DEBUG) errs() << "Calculating trip count\n";
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
          //TerminatorInst *TI = Preheader->getTerminator();
	  auto *TI = Preheader->getTerminator();
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

      // Vinh's COMSAT technique does not require TC!
      errs() << "calculateLoopTC bypassed!\n";
      return 1;
      
      unsigned TripCount = 0;
      BasicBlock *ExitingBlockTemp = L->getLoopLatch();
      if (!ExitingBlockTemp || (ExitingBlockTemp && !L->isLoopExiting(ExitingBlockTemp)))
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
      if (bb1 == bb2)
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
        if (bb1 == loopLatch)
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
            if (bb2 == (*pblocks))
            {
              return 0;
            }
            //we do not traverse beyond loop latch, if we dont reach the second block, it is inter iteration
            //we should not stop here though because there might be more blocks in to_visit set that has not traversed yet
            if ((*pblocks) == loopLatch)
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
      //errs() << "inside liveout to be stored\n";
      StoreInst * ptr;
      // Reject two common cases fast: instructions with no uses (like stores)
      // and instructions with one use that is in the same block as this.
      if (BI->use_empty())
        return NULL;

      //errs() << "passed null\n";
      ptr = NULL;

      for(auto U : BI->users()){  // U is of type User*
        if(auto Inst = dyn_cast<Instruction>(U)){
          //  errs() << "Liveout store: " << *Inst << "\n";
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
          {
            //  errs() << "inside else\n";
            //exit(1);
            continue; 
            ptr = getLiveOutToBeStored(Inst, bbs, LoopLatchExitBlks);
          }
          if(ptr != NULL) break;
        }
      }
      // errs() << "passed for loop\n"; 
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
      NODE *phiNode = new NODE(cgra_select, 1, NodeID++, "", NULL);
      Datatype dt = int32; 
      phiNode->setDatatype(dt);
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
        llvm::Datatype dt = int32;
        constInt->setDatatype(dt);
      }
      else
      {
        constInt = new NODE(constant, 1, NodeID++, "ConstInt0", NULL);
        llvm::Datatype dt = int32;
        constInt->setDatatype(dt);
      }

      myDFG->insert_Node(constInt);

      myDFG->make_Arc(exitCond, phiNode, EdgeID++, 1, TrueDep, 0);
      myDFG->make_Arc(constInt, phiNode, EdgeID++, 0, TrueDep, 1);

      return phiNode;
    }

    NODE * padUnsafeLiveOutOperationsWithSelects(DFG *myDFG, NODE *unsafeOp, NODE *exitCond, std::vector<BasicBlock *> bbs)
    {
      Datatype dt = unsafeOp->getDatatype();
      if(DEBUG) errs() << "\n\nAre we padding LiveOuts?\n\n" ; 
      NODE *selOp = new NODE(cond_select, 1, NodeID++, "", NULL);
      selOp->setDatatype(dt);
      myDFG->insert_Node(selOp);
      if(DEBUG) errs() << "\n\nUnsafe op node: " << selOp->get_ID() << "\n\n"; 

      /* Due to ISA challenges for select instruction,
         a select operation cannot store its output value in registers.
         Mapping is simplified and does not consider such constraints.
         Therefore, we cannot pad with a single select operation with recurrence
         Instead, route (add with 0) the output to select,
         forming Loop-carried dependence with a delay of the cycle as 2 instead of 1 */

      NODE *forwardValue = new NODE(add, 1, NodeID++, "", NULL);
      forwardValue->setDatatype(dt);
      myDFG->insert_Node(forwardValue);
      NODE *const0 = new NODE(constant, 1, NodeID++, "ConstInt0", NULL);
      const0->setDatatype((Datatype) int32);
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


      if(unsafeOp->is_Load_Address_Generator())
          unsafeOp = unsafeOp->get_Related_Node();
      myDFG->make_Arc(unsafeOp, selOp, EdgeID++, 0, TrueDep, selectInput_opOrder);
    
      if(forwardValue->is_Load_Address_Generator())
          forwardValue = forwardValue->get_Related_Node();
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
      Datatype dt = exitCond->getDatatype(); 
      NODE *exitOp = new NODE(loopctrl, 1, NodeID++, "", NULL);
      exitOp->setDatatype(dt);
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
        constInt->setDatatype((Datatype) int32);
      }
      else
      {
        exitOp_Order = 0;
        const_opOrder = 1;
        constInt = new NODE(constant, 1, NodeID++, "ConstInt0", NULL);
        constInt->setDatatype((Datatype) int32);
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
      {
        return IRChanged;
      }

      if(DEBUG)
      {
        errs() << "we are in liveout update\n";
        errs() << "Ins: " << *BI << "\n";
      }

      NODE *node;
      //errs() << "before new store\n";
      newStore = getLiveOutToBeStored(BI, bbs, LoopLatchExitBlks);
      //errs() << "after get liveout store\n"; 
      if(newStore == NULL)
      {
        std::string gPtrName;
        Value *v = dyn_cast<Value>(BI);
        Type* T = v->getType();
        GlobalVariable *gPtr;
        alignment = 4;
        llvm::Datatype dt; 

        if(T->isIntegerTy() || T->isFloatingPointTy())
        {
          gPtrName = "gVar" + std::to_string(++gVarNo);
          // Create new global pointer
          gPtr = new GlobalVariable(*M, T, false, GlobalValue::CommonLinkage, 0, gPtrName);
          Constant *constVal = Constant::getNullValue(T);
          gPtr->setInitializer(constVal);
          if(T->getPrimitiveSizeInBits() % 8 == 0)
            alignment = (T->getPrimitiveSizeInBits())/8;
          else
            alignment = T->getPrimitiveSizeInBits() %8;

          dt = get_Datatype(T, alignment); 
          // IntegerTy()  == 11 means it is arbitrary bit width integers.
          // LLVM classifies char as IntegerTy with bit width 1. 
          // so if the Integer type is divisble by 8 then we can classify them as int or floats
          // This else statement says use char as Primitive size. Change this if you face any errors.
          // Char should be 1-byte aligned and short should be 2-byte aligned.
        }
        else if(T->isPointerTy())
        { 
          gPtrName = "gPtr" + std::to_string(++gPtrNo);
          //PointerType* PointerTy_0 = dyn_cast<PointerType>(T);// PointerType::get(T, 0);
          // find the element type
          PointerType* PT = dyn_cast<PointerType>(T);
	  
          int bit_width;
          if(PT != nullptr)
          { 
            if(PT->getPrimitiveSizeInBits() % 8 == 0)
              bit_width = (PT->getElementType()->getPrimitiveSizeInBits())/8;
            else
              bit_width = PT->getElementType()->getPrimitiveSizeInBits() %8;

	    if(bit_width == 4)
            {
              if(PT->getElementType()->isFloatingPointTy())
                dt = float32;
              else
                dt = int32;  
            }
            else if(bit_width == 8)
              dt = float64;
            else if(bit_width < 4)
              dt = character;
          }
          else
          {
            if(T->getPrimitiveSizeInBits() % 8 == 0)
              bit_width = (T->getPrimitiveSizeInBits())/8;
            else
              bit_width = T->getPrimitiveSizeInBits() %8;
            if(bit_width == 4)
            {
              if(T->isIntegerTy())
                dt = int32;
              else
                dt = float32;
            }
            else if(bit_width == 8)
              dt = float64;
            else if(bit_width < 4)
              dt = character;
          }

          gPtr = new GlobalVariable(*M, T, false, GlobalValue::CommonLinkage, 0, gPtrName);
          ConstantPointerNull* const_ptr_2 = ConstantPointerNull::get(PT);
          gPtr->setInitializer(const_ptr_2);
          alignment = T->getPointerAddressSpace();
          if(alignment < 4) alignment = 4;
        }
        if(alignment == 0)
          errs() << "WARNING: Update_LiveOut_Variable: Cannot find size of the memory access for live-out variable\n";
	if(DEBUG) errs() << "  Aligment: " << alignment << "\n";
        gPtr->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);
        node = new NODE(constant, 1, ((NodeID++)+100), gPtrName, gPtr);
        node->setAlignment(alignment);
        node->setDatatype(dt);
        myDFG->insert_Node(node);
        ptr = gPtr;
        storeVariable = gPtrName;
      }
      // Find the store node, if not found track it using successors in the next block
      else
      {
        if(DEBUG)
          errs() << "else store in not null\n";
        ptr = newStore->getPointerOperand();
        Value *v = dyn_cast<Value>(BI);
        Type* T = v->getType();
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
          llvm::Datatype dt;  
          if(T->getPrimitiveSizeInBits() % 8 == 0)
            alignment = (T->getPrimitiveSizeInBits())/8;
          else
            alignment = T->getPrimitiveSizeInBits() %8;

          dt = get_Datatype(T, alignment);
          node->setDatatype(dt);
          myDFG->insert_Node(node);

        }
      }

      //errs() << "Passed if else\n";
      map_instn_defn_liveout_to_load[BI] = storeVariable;
      map_liveout_to_alignment[storeVariable] = alignment;
      NODE *outputNode = myDFG->get_Node(BI);
      if(outputNode->get_Instruction() == ld_add) outputNode = outputNode->get_Related_Node();

      // Hardware model does not support pred insts (cond_select nodes) to store data to register and hence cannot be used for liveout, following statements add a routing node from such nodes to support liveout data
      if(outputNode->get_Instruction() == cond_select){ 
	NODE* route_node = new NODE(add, 1, NodeID++, "route", NULL);
	route_node->setDatatype(outputNode->getDatatype());
	myDFG->insert_Node(route_node);
	myDFG->make_Arc(outputNode, route_node, EdgeID++, 0, TrueDep, 0);
	outputNode = route_node;
      }
      
      if(DEBUG) errs() << "  outputNode: " << outputNode->get_Name() << "\n";
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

      if(DEBUG) errs() << "  Make_arc: outputNode: " << outputNode->get_Name() << " -> node: " <<  node->get_Name() << "\n";
      myDFG->make_Arc(outputNode, node, EdgeID++, 0, LiveOutDataDep, 0);

      unsigned int CGRA_ConstantID = node->get_ID();
      unsigned int CGRA_storeAddID = ((NodeID++)+100);
      unsigned int CGRA_storeDataID = ((NodeID++)+100);
      unsigned int liveout_datatype = (unsigned int) (node->getDatatype()); 
      //Generate file for live-out
      liveoutNodefile << outputNode->get_ID() << "\t" << outputNode->get_Instruction() << "\t" << outputNode->get_Name() << "\t" << 0 << "\t" <<liveout_datatype << "\n";
      liveoutNodefile << CGRA_ConstantID << "\t" << constant << "\t" << storeVariable << "\t" << 0 << "\t" << liveout_datatype << "\n";
      liveoutNodefile << CGRA_storeAddID << "\t" << st_add << "\t" << "st_add_" + storeVariable << "\t" << alignment << "\t" << liveout_datatype << "\n";
      liveoutNodefile << CGRA_storeDataID << "\t" << st_data << "\t" << "st_data_" + storeVariable << "\t" << 0 << "\t" << liveout_datatype << "\n";

      liveoutEdgefile << CGRA_storeAddID << "\t" << CGRA_storeDataID << "\t0\tSRE\t0\n";
      liveoutEdgefile << outputNode->get_ID() << "\t" << CGRA_storeDataID << "\t0\tTRU\t0\n";
      liveoutEdgefile << CGRA_ConstantID << "\t" << CGRA_storeDataID << "\t0\tTRU\t1\n";

      // delete stores and modify corresponding accesses if no store
      LoadInst* loadGlobal;
      LoadInst* loadGlobalNonPhi;


      /*for(auto U : BI->users()){  // U is  f type User*
        if(auto Inst = dyn_cast<Instruction>(U))
        {
        if ( std::find(bbs.begin(), bbs.end(), Inst->getParent()) != bbs.end() )
        continue;
        errs() << "Inst: " << *Inst << "\n";
        }
        }*/ 
      //exit(1);

      BasicBlock* LoadInsertedBB, *currentBB;
      // See if need to load and modify the operands of the successors
      for(auto U : BI->users()){  // U is  f type User*
        if(auto Inst = dyn_cast<Instruction>(U))
        {
          currentBB = Inst->getParent();

          //this ensures that the load instructions are added to the 
          // users from different basicblocks. 
          if(LoadInsertedBB != currentBB)
            loadInserted = false;
          //else
          //  continue; 
          //errs() << "Inst: " << *Inst << "\n";
          //errs() << "gVarNo: " << gVarNo << "\n";
          // an instruction uses V
          // Ensure that the use is outside of the BBs of loop
          if ( std::find(bbs.begin(), bbs.end(), Inst->getParent()) != bbs.end() )
            continue;

          //delete instance of the store
          if((Inst->getOpcode() == Instruction::Store) &&
              (dyn_cast<llvm::GlobalValue>(Inst)))
          {

            //errs() << "gVarNo: " << gVarNo << "\t erase from Parent\n";
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
                if(DEBUG)
                  errs() << "New BB create else\n";
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

                //errs() << "sourceBB: \n";
                //for(BasicBlock::iterator mitt = sourceBB->begin(); mitt!=sourceBB->end(); ++mitt)
                // errs() << *mitt << "\n";
              }

              //TerminatorInst *TI = newBB->getTerminator();
	      auto *TI = newBB->getTerminator();
              //errs() << "term instr: " << *TI << "\n";
              loadGlobal = new LoadInst(Inst->getType(),ptr, "", TI);
              map_replace_liveout_use[Inst] = loadGlobal;

              //errs() << "newBB: \n"; 
              //for(BasicBlock::iterator mitt = newBB->begin(); mitt!=newBB->end(); ++mitt)
              // errs() << *mitt << "\n";

              //BasicBlock *sourceBB = BI->getParent();
              //errs() << "sourceBB: \n";
              //for(BasicBlock::iterator mitt = sourceBB->begin(); mitt!=sourceBB->end(); ++mitt)
              // errs() << *mitt << "\n";
            }
            else
            { 
              // Make all instructions that referred to BI now refer to loadGlobal as their source.
              // if load not inserted add load node in the destination BB. 
              // Usually the we add the load instruction of the loadGlobal just before the Inst.
              // But this causes the "Instruction Does not Dominate all uses!" error.
              // So we find the top first non phi node of the BB and insert the load. 
              if(!loadInserted)
              {
                if(DEBUG)
                  errs() << "if load not inserted!\n";
                BasicBlock *destinationBB = Inst->getParent();
                Instruction *FirstNonPhi = destinationBB->getFirstNonPHI();
		Type *T = ptr->getType();

		if(T->isPointerTy())
		{
		   //PointerType* PointerTy_0 = dyn_cast<PointerType>(T);// PointerType::get(T, 0);
                   // find the element type
                   PointerType* PT = dyn_cast<PointerType>(T);
                   if(DEBUG) errs() << "found array type\n";
                  loadGlobalNonPhi = new LoadInst(PT->getElementType(), ptr, storeVariable, FirstNonPhi);
		}
		else
                 loadGlobalNonPhi = new LoadInst(ptr->getType(),ptr, storeVariable, FirstNonPhi);
                loadInserted = true;
                LoadInsertedBB = destinationBB;  
              }
              map_replace_liveout_use[Inst] = loadGlobalNonPhi;
            }
          }
          IRChanged = true;
        }
      }

      if(DEBUG)
        errs() << "passed update liveout\n"; 
      // replace use with load instruction

      std::map<Instruction*, Instruction*>::iterator itt;
      for(itt = map_replace_liveout_use.begin(); itt != map_replace_liveout_use.end(); itt++)
      { 
        if(DEBUG)
          errs() << "inside updating load instructions\t" << *(itt->first) << "\t" << *(itt->second) << "\n";
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
        //TerminatorInst *TI = Inst->getParent()->getTerminator();
	auto *TI = Inst->getParent()->getTerminator();
        std::map<Value *, std::string>::iterator it;
        it = map_instn_defn_livein_to_load.find(v);
        if (it != map_instn_defn_livein_to_load.end() )
          continue;

        llvm::Type* T = v->getType();
        GlobalVariable *gPtr;
        unsigned alignment = 4;
        Datatype dt;  

        if(T->isIntegerTy() || T->isFloatingPointTy())
        {
          gPtrName = "gVar" + std::to_string(++gVarNo);
          // Create new global pointer
          gPtr = new GlobalVariable(*M, T, false, GlobalValue::CommonLinkage, 0, gPtrName);
          Constant *constVal = Constant::getNullValue(T);
          gPtr->setInitializer(constVal);

          if(T->getPrimitiveSizeInBits() % 8 == 0)
            alignment = (T->getPrimitiveSizeInBits())/8;
          else
            alignment = T->getPrimitiveSizeInBits() %8;

          dt = get_Datatype(T, alignment);
        }
        else if(T->isPointerTy())
        {
          gPtrName = "gPtr" + std::to_string(++gPtrNo);
          //PointerType* PointerTy_0 = dyn_cast<PointerType>(T);// PointerType::get(T, 0);

          PointerType* PT = dyn_cast<PointerType>(T);

          if(DEBUG)
            errs() << "after get seq ptr\n";

          int bit_width;
          if(PT != nullptr)
          {
            if(PT->getPrimitiveSizeInBits() % 8 == 0)
              bit_width = (PT->getElementType()->getPrimitiveSizeInBits())/8;
            else
              bit_width = PT->getElementType()->getPrimitiveSizeInBits() %8;
            if(DEBUG)
              errs() << "passed bit width\n";
            if(bit_width == 4)
            {
              if(PT->getElementType()->isFloatingPointTy())
                dt = float32;
              else
                dt = int32;
            }
            else if(bit_width == 8)
              dt = float64;
            else if(bit_width < 4)
              dt = character;
          }
          else
          {
            if(DEBUG)
            {
              errs() << "inside else of the update livein\n";
              errs() << "T->getPrimitiveSizeInBits(): " << T->getPrimitiveSizeInBits() << "\n";
            }
            if(T->getPrimitiveSizeInBits() % 8 == 0)
              bit_width = (T->getPrimitiveSizeInBits())/8;
            else
              bit_width = T->getPrimitiveSizeInBits() %8;
            if(bit_width == 4)
            {
              if(T->isIntegerTy())
                dt = int32;
              else
                dt = float32;
            }
            else if(bit_width == 8)
              dt = float64;
            else if(bit_width < 4)
              dt = character;
          }
          gPtr = new GlobalVariable(*M, T, false, GlobalValue::CommonLinkage, 0, gPtrName);
          ConstantPointerNull* const_ptr_2 = ConstantPointerNull::get(PT);
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
        //map_instn_defn_livein_to_load[v] = gPtrName; 
        map_instn_defn_livein_to_load[v] = gPtrName; 
        map_livein_to_alignment[gPtrName] = alignment;
        map_livein_to_datatype[gPtrName] = dt;
      }
      return IRChanged;
    }

    unsigned tempStructSize = 0;
    unsigned tempArraySize = 0;
    Datatype get_Datatype(Type* T, int bit_width)
    {
      llvm::Datatype dt;
      bool is_primitive = false;
      if(T->isIntegerTy())
      {
        if(bit_width == 1)
          dt = character;
        else if(bit_width == 2)
          dt = int16;
        else
          dt = int32;
	is_primitive = true;
      }
      else if(T->isFloatingPointTy())
      {
        if(T->isDoubleTy())
          dt = float64;
        else if(bit_width == 2)
          dt = float16;
        else
          dt = float32;
	is_primitive = true;
      }
      else if(T->isVectorTy()){
	errs() << "ERROR: Vectorization detected! Exitting.\n";
	exit(1);
      }
      else if(T->isArrayTy()){
	Type* element_type = T->getArrayElementType();
	Datatype element_dt = get_Datatype(element_type, element_type->getPrimitiveSizeInBits()/8);
	tempArraySize = DT_Size[element_dt] * T->getArrayNumElements();
	if(DEBUG) errs() << "\t    Current node is of type array - numElements: " << T->getArrayNumElements() << " - element size: " << DT_Size[element_dt] << "\n";
	dt = _array;
	is_primitive = true;
      }
      else if(T->isPointerTy()){
	if(DEBUG) errs() << "\t    Current node is of type pointer -> finding element type\n";
	Type* element_type = T->getPointerElementType();
	get_Datatype(element_type, element_type->getPrimitiveSizeInBits()/8);
	dt = int32;
	is_primitive = true;
      }

      // Added by Vinh TA: add support for struct
      // Populate maps:
      //   (struct name -> struct size)
      //   (struct name -> struct member sizes)
      
      if(T->isStructTy()){
	StructType* temp = dyn_cast<StructType>(T);
	if(!temp->isPacked())
	  errs() << "\nERROR: " << temp->getName().str() << " is not packed, make sure padding is disabled by adding #pragma pack(push, 1) before declaration\n";

	std::string structName = T->getStructName().str();
	unsigned structElements = T->getStructNumElements();
	errs() << "\tStruct type - name: " << structName << " - number of elements: " << structElements << "\n";

	for(std::map<std::string, unsigned>::iterator it = map_struct_size.begin(); it != map_struct_size.end(); ++it){
	  if(it->first.compare(structName) == 0){
	    errs() << "\t  Struct already added\n";
	    return int32;
	  }
	}
	
	unsigned structSize = 0;
	map_struct_size[structName] = structSize;
	
	for(int i=0; i<structElements; i++){
	  Type* structMember = T->getStructElementType(i);
	  Datatype memberDT = get_Datatype(structMember, structMember->getPrimitiveSizeInBits()/8);
	  unsigned memberSize;
	  if(memberDT == _struct){
	    memberSize = tempStructSize;
	    memberDT = int32;
	  } else if(memberDT == _array){
	    memberSize = tempArraySize;
	    memberDT = get_Datatype(structMember->getArrayElementType(), structMember->getArrayElementType()->getPrimitiveSizeInBits()/8);
	  } else {
	    memberSize = DT_Size[memberDT];
	  }
	  
	  errs() << "\t  Member " << i << " - dt: " << memberDT << " - size: " << memberSize << "\n";
	  structSize += memberSize;
	  map_struct_member_size[structName].push_back(memberSize);
	}
	map_struct_size[structName] = structSize;
	tempStructSize = structSize;
	return _struct;  // Differentiate struct DT to realize its size
      }

      if(!is_primitive){
	errs() << "\t  Cannot find primitive type! Defaulting to int32!\n";
	dt = int32;
      }
      return dt; 
    }

    //make a new node from instruction BI and adds it to DFG
    bool Add_Node(Instruction *BI, DFG* myDFG, BasicBlock* loopHeader)
    {
      NODE* node, *node2;
      bool retVal = false;
      std::string name="";
      unsigned alignment;
      Value *v = dyn_cast<Value>(BI);
      Type* T = v->getType();
      int bit_width = T->getPrimitiveSizeInBits()/8; 
      llvm::Datatype dt = get_Datatype(T, bit_width);
      if(dt == _struct){
	errs() << "DT returned _struct\n";
	dt = int32;
      }
      else if(dt == _array){
	errs() << "DT returned _array\n";
	dt = get_Datatype(T->getArrayElementType(), T->getArrayElementType()->getPrimitiveSizeInBits()/8);
      }
      

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
          {
            if(DEBUG) {
              errs() << "instr: " << *BI << "\n";
              errs() << "\n\nSwitch Detected!\n\n";
              errs() << "num_cases: " << cast<SwitchInst>(BI)->getNumCases() << "\n";
            }


            //errs() << "num_cases: " << (cast<SwitchInst>(BI)->getSuccessor())->getName().str() << "\n";

            // Mahesh says:
            // The getNumCases returns all the switch cases except default and the default is the 0th successor
            // in the getSuccessor. So get 1 to numcases+1 and then get default again. 
            int num_subblocks = cast<SwitchInst>(BI)->getNumCases(); 

            for(int i=1; i < num_subblocks+1; i++)
            {
              //errs() << "switch blocks " << (cast<SwitchInst>(BI)->getSuccessor(i)) << "\n";
              BasicBlock * bb = cast<SwitchInst>(BI)->getSuccessor(i); 
              for(BasicBlock::iterator it = bb->begin(); it!=bb->end(); ++it)
                if (!Add_Node(&(*it), myDFG, loopHeader))
                  return false;
              //errs() << "intrs: " << *it << "\n";
            } 
            if(DEBUG)
              errs() << "default: " << cast<SwitchInst>(BI)->getDefaultDest() << "\n";
            BasicBlock * bbs = cast<SwitchInst>(BI)->getDefaultDest(); 
            for(BasicBlock::iterator it = bbs->begin(); it!=bbs->end(); ++it)
              if (!Add_Node(&(*it), myDFG, loopHeader))
                return false;

            //errs() << "intrs: " << *it << "\n";

            return true; 
          }
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
          node->setDatatype(dt); 
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::Sub:
        case Instruction::FSub:
          node = new NODE(sub, 1, NodeID++, BI->getName().str(), BI);
          node->setDatatype(dt);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::Mul:
        case Instruction::FMul:
          node = new NODE(mult, 1, NodeID++, BI->getName().str(), BI);
          node->setDatatype(dt);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::UDiv:
        case Instruction::SDiv:
        case Instruction::FDiv:
          node = new NODE(division, 1, NodeID++, BI->getName().str(), BI);
          node->setDatatype(dt);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::URem:
        case Instruction::SRem:
        case Instruction::FRem:
          node = new NODE(rem, 1, NodeID++, BI->getName().str(), BI);
          node->setDatatype(dt);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;

          // Bitwise Binary Operations
        case Instruction::Shl:
          node = new NODE(shiftl, 1, NodeID++, BI->getName().str(), BI);
          node->setDatatype(dt);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::LShr:
          node = new NODE(shiftr_logical, 1, NodeID++, BI->getName().str(), BI);
          node->setDatatype(dt);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::AShr:
          node = new NODE(shiftr, 1, NodeID++, BI->getName().str(), BI);
          node->setDatatype(dt);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::And:
          node = new NODE(andop, 1, NodeID++, BI->getName().str(), BI);
          node->setDatatype(dt);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::Or:
          node = new NODE(orop, 1, NodeID++, BI->getName().str(), BI);
          node->setDatatype(dt);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::Xor:
          node = new NODE(xorop, 1, NodeID++, BI->getName().str(), BI);
          node->setDatatype(dt);
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
          node->setDatatype(dt);
          node2->setDatatype(dt);
          LoadInst *tempLoadInst;
          tempLoadInst = dyn_cast<LoadInst>(BI);
          alignment = tempLoadInst->getAlignment();

	  // Check if loading node targets a struct then adjust alignment to struct size
	  if(dyn_cast<Value>(BI)->getType()->isPointerTy()){
	    Type* BI_type = dyn_cast<Value>(BI)->getType();
	    Type* element_type = BI_type->getPointerElementType();
	    if(element_type->isStructTy()){
	      BI_type = element_type;
	      std::string struct_name = BI_type->getStructName().str();
	      if(map_struct_size.find(struct_name) == map_struct_size.end())
	        get_Datatype(BI_type, BI_type->getPrimitiveSizeInBits()/8);
	      alignment = map_struct_size[struct_name];
	      if(DEBUG) errs() << "\t  Loading struct - name: " << struct_name << " - alignment: " << alignment << "\n";
	    }
	  }
	  else if(dyn_cast<Value>(BI)->getType()->isStructTy()){
	    Type* BI_type = dyn_cast<Value>(BI)->getType();
	    std::string struct_name = BI_type->getStructName().str();
	    if(map_struct_size.find(struct_name) == map_struct_size.end())
	      get_Datatype(BI_type, BI_type->getPrimitiveSizeInBits()/8);
	    alignment = map_struct_size[struct_name];
	    if(DEBUG) errs() << "\t  Loading struct - name: " << struct_name << " - alignment: " << alignment << "\n";
	  }
	  
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
	  break;
        case Instruction::Store:
          {
            node = new NODE(st_add, 1, NodeID++, BI->getName().str(), BI);
            node2 = new NODE(st_data, 1, NodeID++, BI->getName().str(), BI);
            node->set_Store_Data_Bus_Write(node2);
            node2->set_Store_Address_Generator(node);
            StoreInst *tempStoreInst;
            tempStoreInst = dyn_cast<StoreInst>(BI);
            alignment = tempStoreInst->getAlignment();
            if(DEBUG1) errs() << "alignment for store: " << alignment << "\n";
            if(DEBUG1) errs() << "T: " << T << "\n";
            StoreInst* si = dyn_cast<StoreInst>(BI); 
            Value *vi = si->getPointerOperand(); 
            Type* Ti = vi->getType()->getPointerElementType();
            if(DEBUG1) errs() << "store name: " << node->get_ID() << "\n"; 
            dt = get_Datatype(Ti, alignment); 
            if(DEBUG1) errs() << "Datatype: " << dt << "\n";
            node->setAlignment(alignment);
            node->setDatatype(dt);
            node2->setDatatype(dt);
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
          }
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
          dt = int32; 
          node->setDatatype(dt);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;

          // Convert instructions...
        case Instruction::Trunc:
        case Instruction::ZExt:
          node = new NODE(andop, 1, NodeID++, BI->getName().str(), BI);
          node->setDatatype(dt);
          myDFG->insert_Node(node);
          return true;
        case Instruction::SExt:
          node = new NODE(sext, 1, NodeID++, BI->getName().str(), BI);
          node->setDatatype(dt);
          myDFG->insert_Node(node);
          retVal = true;
          return retVal;
        case Instruction::FPTrunc:
        case Instruction::FPExt:
          node = new NODE(andop, 1, NodeID++, BI->getName().str(), BI);
          node->setDatatype(dt);
          myDFG->insert_Node(node);
          return true;
        case Instruction::FPToUI:
        case Instruction::FPToSI:
          node = new NODE(add, 1, NodeID++, BI->getName().str(), BI); 
          node->setDatatype((Datatype) int32);
          myDFG->insert_Node(node);
          return true; 
        case Instruction::UIToFP:
        case Instruction::SIToFP:
          node = new NODE(add, 1, NodeID++, BI->getName().str(), BI);
          node->setDatatype((Datatype) float32);
          myDFG->insert_Node(node);
          return true;
        case Instruction::IntToPtr:
        case Instruction::PtrToInt:
          retVal = true; 
        case Instruction::BitCast:
          node = new NODE(add, 1, NodeID++, BI->getName().str(), BI);
          node->setDatatype(dt);
          myDFG->insert_Node(node);
          return true;
          // llvm document: It is always a no-op cast becasue
          // no bits change.
          return retVal;
          // Other instructions...
        case Instruction::ICmp:
        case Instruction::FCmp:
          if(dt == 0)
          {
            Value *v = dyn_cast<Value>(BI->getOperand(0));
            Type* T = v->getType();
            int width = T->getPrimitiveSizeInBits()/8; 

            //errs() << "width " << width << "\n";
            if(T->isIntegerTy())
            {
              //errs() << "cmp int type    " << T->getPrimitiveSizeInBits() << "\n";
              if(width == 1)
                dt = character;
              else if(width == 2)
                dt = int16;
              else
                dt = int32;
            }
            else if(T->isFloatingPointTy())
            {
              if(T->isDoubleTy())
                dt = float64;
              else if(width == 2)
                dt = float32;
              else
                dt = float16;
              //errs() << "cmp float    " << T->getPrimitiveSizeInBits() << "\n";
            } 

          } 
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
          node->setDatatype(dt);
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
            if(DEBUG) errs() << "We are here for PHI\n";
            node = new NODE(cgra_select, 1, NodeID++, BI->getName().str(), BI);
            node->setDatatype(dt);
            myDFG->insert_Node(node);
            retVal = true;
            return retVal;
          }
        case Instruction::Select:
          node = new NODE(cond_select, 1, NodeID++, BI->getName().str(), BI);
          node->setDatatype(dt);
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
        if (current == (*it))
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
        if(DEBUG)
          errs() << "in 1st if\n"; 
        BasicBlock *tempBB = bb1->getSinglePredecessor();
        if(DEBUG)
          if(tempBB == NULL)
            errs() <<  "tempBB is NULL\n";
        if(tempBB != NULL)
        {
          for(BasicBlock::iterator BI_temp = tempBB->begin(); BI_temp !=tempBB->end(); ++BI_temp)
          {
            if(BI_temp->getOpcode() != Instruction::Br)
              continue;
            if(DEBUG) errs() << "inside common parents: " << (*BI_temp) << "\n";
            BranchInst* tempBrInst = dyn_cast<llvm::BranchInst>(BI_temp);
            if(tempBrInst->isUnconditional()) break;
            brInst = tempBrInst;
            condInst = dyn_cast<llvm::Instruction>(tempBrInst->getCondition());
            retBlk = bb1->getSinglePredecessor();
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
              if(DEBUG) errs() << "inside common parents: " << (*BBI) << "\n";
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
      }
      else if(bb1->getSinglePredecessor() == bb2)
      {
        if(DEBUG)
          errs() << "in 2nd if-else\n";
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
        if(DEBUG)
          errs() << "in 3rd if-else\n";
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
        if(DEBUG)
          errs() << "in last else\n";     

        // errs() << "bbssize: " << bbs.size() << "\n";
        bool found = false;
        for (unsigned int ii = 0; ii < bbs.size(); ii++)
        {
          if(found) break;
          for (BasicBlock::iterator BBI = bbs[ii]->begin(); BBI != bbs[ii]->end(); ++BBI)
          {
            if(BBI->getOpcode() != Instruction::Br)
              continue;
            if(DEBUG) errs() << "inside common parents: " << (*BBI) << "\n";
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
        brInst = tempBrInst;
        condInst = dyn_cast<llvm::Instruction>(tempBrInst->getCondition());
        if(tempBrInst->isUnconditional()) return NULL;
        return tempBrInst;
      }
      return tempBrInst;
    }

    

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
          if(DEBUG1)
          { 
            errs() << "Br inst=" << *tempBrInst << "\n"; 
            if(succ2 != NULL)
            {
              errs() << "working in succ2\n";
              for(BasicBlock::iterator itt=succ2->begin(); itt!=succ2->end(); ++itt)
                errs() << *itt << "\n";
            }

          }
          BasicBlock *commonSuccessor = SearchCommonSuccessorForBranching(bbs[ii],succ1,succ2,loopLatch);
          assert((commonSuccessor != NULL) && "Error while finding common successor");

          ifBlocks.push_back(succ1);
          elseBlocks.push_back(succ2);
          condBlocks.push_back(bbs[ii]);
          condSet.push_back(tempBrInst);
          if(DEBUG1)
          {
            errs() << "succ blocks pushed\n";
            if(commonSuccessor == NULL)
              errs() << "Hi common sucessor is null\n";
            else
            {
              for(BasicBlock::iterator itt = commonSuccessor->begin(); itt != commonSuccessor->end(); ++itt)
                errs() << *itt << "\n";
            }
            errs() << "\n";  
          }
          succBlocks.push_back(commonSuccessor);
          break;
        }
      }
      if(DEBUG1)
      {
        errs() << "from collecting info\n";
        for(unsigned int ii=0; ii < ifBlocks.size(); ii++)  
        {
          errs() << "ii: " << ii << "\n";
          for(BasicBlock::iterator BBI = ifBlocks[ii]->begin(); BBI != ifBlocks[ii]->end(); ++BBI)
            errs() << "ifBlock ins: " << *BBI << "\n";
          errs() << "\n";
        }

        for(unsigned int ii=0; ii < elseBlocks.size(); ii++)  
        {
          errs() << "ii: " << ii << "\n";
          for(BasicBlock::iterator BBI = elseBlocks[ii]->begin(); BBI != elseBlocks[ii]->end(); ++BBI)
            errs() << "elseBlock ins: " << *BBI << "\n";
          errs() << "\n";
        }
        for(unsigned int ii=0; ii < condSet.size(); ii++)
          errs() << "condset: " << *(condSet[ii]) << "\n"; 
        //for(unsigned int ii=0; ii < succBlocks.size(); ii++)
        // {
        //   errs() << "ii: " << ii << "\n";
        //  for(BasicBlock::iterator BBI = succBlocks[ii]->begin(); BBI != succBlocks[ii]->end(); ++BBI)
        //    errs() << "succBlock ins: " << *BBI << "\n";
        //  errs() << "\n";
        //}
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
      //std::vector<std::string> storeVarNames;
      //std::vector<int> varFound;

      if(DEBUG1) errs() << "\nupdateStoresInConditionalBBs\ncondsize: " << condBlocks.size() << "\n";


      /*for(unsigned int ii = 0; ii < condBlocks.size(); ii++)
        {
        for(unsigned int it = 0; it < 2; it++)
        {
        BasicBlock *current;
        if(it == 0)
        current = ifBlocks[ii];
        else
        current = elseBlocks[ii];

        if(current == succBlocks[ii]) continue;

      //while(current != loopLatch)
      //{
      if(current == succBlocks[ii]) break;
      for (BasicBlock::iterator BBI = current->begin(); BBI != current->end(); ++BBI)
      {
      if(DEBUG1)
      errs() << "we are in store: " << *BBI << "\n";

      if(BBI->getOpcode() == Instruction::Br)
      {
      BranchInst * tempBrInst = (dyn_cast<llvm::BranchInst>(BBI));
      if(tempBrInst->isUnconditional())
      current = current->getSingleSuccessor();
      else
      {
      // current = SearchCommonSuccessorForBranching(current,tempBrInst->getSuccessor(0), tempBrInst->getSuccessor(1),loopLatch);
      current = loopLatch; 
      }
      break;
      }
      else if(BBI->getOpcode() == Instruction::Store)
      {
      if(DEBUG1) errs() << "inside storei\n";
      }
      }
      //}
      }
      }*/

      std::vector<BasicBlock*> unconditional_blocks;
      for(unsigned ii = 0; ii < condBlocks.size(); ii++){
	BasicBlock* current = ifBlocks[ii];
	for(auto block_it = current->begin(); block_it != current->end(); ++block_it){
	  if(block_it->getOpcode() == Instruction::Br){
	    BranchInst* BrInst = (dyn_cast<llvm::BranchInst>(block_it));
	    if(BrInst->isUnconditional()){
	      unconditional_blocks.push_back(current->getSingleSuccessor());
	      if(DEBUG1) errs() << "  Unconditional block pushed back: " << *(current->getSingleSuccessor()) << "\n";
	    }
	  }
	}
      }

      for(unsigned int ii = 0; ii < condBlocks.size(); ii++)
      {
        for(unsigned int it = 0; it < 2; it++)
        {
          BasicBlock *current;
          if(it == 0)
            current = ifBlocks[ii];
          else
            current = elseBlocks[ii];

          if(current == succBlocks[ii]) continue;

          //while(current != loopLatch)
          // {
          if(current == succBlocks[ii]) break;  
          for (BasicBlock::iterator BBI = current->begin(); BBI != current->end(); ++BBI)
          {
            if(DEBUG1)
              errs() << "break ins: " << *BBI << "\n";

            if(BBI->getOpcode() == Instruction::Br)
            {
              BranchInst * tempBrInst = (dyn_cast<llvm::BranchInst>(BBI));
              if(tempBrInst->isUnconditional())
                current = current->getSingleSuccessor();
              else
              {
                //current = SearchCommonSuccessorForBranching(current,tempBrInst->getSuccessor(0), tempBrInst->getSuccessor(1),loopLatch);
                current = loopLatch;
              }
              break;
            }
            else if(BBI->getOpcode() == Instruction::Store)
            {
              if(DEBUG1)
                errs() << "in store: " << *BBI << "\n";
              StoreInst *stInst = (dyn_cast<llvm::StoreInst>(BBI));
              // TODO
              Instruction *ptrOperand = (dyn_cast<Instruction>(stInst->getPointerOperand()));
              Instruction *valOperand = (dyn_cast<Instruction>(stInst->getValueOperand()));
              if(DEBUG1)
              {
                errs() << "ptroperand: " << *ptrOperand << "\n";
              }
              if(ptrOperand != nullptr)
              {
                GetElementPtrInst *ptrInst = dyn_cast<GetElementPtrInst>(stInst->getPointerOperand());
                std::string varName; 
                if(ptrInst != nullptr) 
                {
                  varName = ptrInst->getPointerOperand()->getName().str();
                  if(varName.empty()) varName = myDFG->get_Node(stInst->getPointerOperand())->get_Name();
                }
                else
                  varName = myDFG->get_Node(stInst->getPointerOperand())->get_Name();
                if(DEBUG1) errs() << "varName: " << varName << "\n";

                NODE* node4 = myDFG->get_Node(ptrOperand);
                if(DEBUG1) errs() << "passed node 4: " << node4->get_Name() << "\n";
                NODE* node5 = myDFG->get_Node(valOperand);
                if(DEBUG1) errs() << "passed node 5: " << node5->get_Name() << "\n";
                std::vector<NODE*> outGoingNodes = node4->Get_Next_Nodes();
		//std::vector<ARC*> outGoingArcs = myDFG->getSetOfArcs();


                NODE* node_ld; 
                NODE* in_node;
                bool found=false;
                for(unsigned int out_i = 0; out_i < outGoingNodes.size(); out_i++)
                {
                  //if(outGoingArcs[ii]->get_From_Node()->get_ID() == node4->get_ID() && outGoingArcs[ii]->get_To_Node()->get_Instruction() == ld_add)
		  if(outGoingNodes[out_i]->is_Load_Address_Generator()) 
                  {
                    found = true;
                    node_ld = outGoingNodes[out_i]; 
                  }
                }
                if(DEBUG1) errs() << "passed outgoing found: " << found << "\n";

		NODE* node3, *node_ld_data;
		
                if(!found)
                {
                  NODE* node = myDFG->get_Node_Mem_Data(cast<Instruction>(BBI));
		  std::vector<NODE*> from_node = node->Get_Prev_Nodes();
                  for(unsigned int ii = 0; ii < from_node.size(); ii++)
                  {
                    if(!from_node[ii]->is_Store_Address_Generator())
                    {
		      in_node = from_node[ii];
		      break;
                    }
                  }
		  assert((in_node != NULL) && "Fatal: store data node not found!\n");
		  if(DEBUG1) errs() << "in node: " << in_node->get_Name() << "\n";
                  if(DEBUG1) errs() << "dest node: " << node->get_Name() << "\n";
		  
                  NODE* node1 = new NODE(ld_add, 1, NodeID++, "", NULL);
                  NODE* node2 = new NODE(ld_data, 1, NodeID++, "", NULL);
                  node1->setDatatype(node->getDatatype());
                  node2->setDatatype(node->getDatatype());
                  unsigned alignment = stInst->getAlignment();
                  node1->setAlignment(alignment);
                  node1->set_Load_Data_Bus_Read(node2); 
                  node2->set_Load_Address_Generator(node1);
                  myDFG->insert_Node(node1);
                  myDFG->insert_Node(node2);
		  node_ld_data = node2;

                  node3 = new NODE(cond_select, 1, NodeID++, "", NULL); 
                  myDFG->insert_Node(node3);

                  if(DEBUG1) errs() << " not found inserting nodes: node1: " << node1->get_ID() << "\tnode2: " << node2->get_ID() << "\tnode3:" << node3->get_ID() << "\n";
                  ARC* arc;
                  if(in_node != NULL)
                  {
                    arc = myDFG->get_Arc(in_node, node);
                    if(arc != NULL)
                      myDFG->Remove_Arc(arc);
		    else errs() << "WARNING: in_node -> node arc not found!\n";
                  }
                  myDFG->make_Arc(node4, node1, EdgeID++, 0, TrueDep, 0);
                  //myDFG->make_Arc(node1, node2, EdgeID++, 0, TrueDep, 0);
                  if(DEBUG1) errs() << "if: " << it << "\n";
                  if(DEBUG1) errs() << "if ii: " << ii << "\n";
                  Instruction* cond; 
                  //if(ii == 0)
                  cond = dyn_cast<llvm::Instruction>(condSet[ii]->getCondition());
                  //else
                  //cond = dyn_cast<llvm::Instruction>(condSet[0]->getCondition());
                  BasicBlock *parent = BBI->getParent(); 
                  NODE* node6 = myDFG->get_Node(cond);  
                  if(DEBUG1) errs() << "if node6: " << node6->get_ID() << "\n"; 
                  if(it == 0) 
                  {
		    if(DEBUG1) errs() << "it = 0\n";
                    myDFG->make_Arc(node2, node3, EdgeID++, 0, TrueDep, 0);
                    if(in_node!=NULL)
                      myDFG->make_Arc(in_node, node3, EdgeID++, 0, TrueDep, 1);      
                    else
                      myDFG->make_Arc(node, node3, EdgeID++, 0, TrueDep, 1);
                    myDFG->make_Arc(node6, node3, EdgeID++, 0, PredDep, 2);
                  }
                  else
                  {
		    if(DEBUG1) errs() << "it != 0\n";
                    myDFG->make_Arc(node5, node3, EdgeID++, 0, TrueDep, 0);
                    myDFG->make_Arc(node2, node3, EdgeID++, 0, TrueDep, 1); 
                    myDFG->make_Arc(node6, node3, EdgeID++, 0, PredDep, 2);
                  }
                  node3->setDatatype(node->getDatatype()); 
                  myDFG->make_Arc(node3, node, EdgeID++, 0, TrueDep, 0);
                }
                else
                {
                  if(node5->is_Load_Address_Generator()) node5 = node5->get_Related_Node();
                  node3 = new NODE(cond_select, 1, NodeID++, "", NULL);
                  if(DEBUG1) errs() << "inserting nodes: node3: " << node3->get_Name() << "\n";
                  myDFG->insert_Node(node3);
                  node_ld_data = node_ld->get_Related_Node(); 
                  if(DEBUG1) errs() << "node_ld_data: " << node_ld_data->get_Name() << "\n";
                  NODE* node = myDFG->get_Node_Mem_Data(cast<Instruction>(BBI));
                  if(DEBUG1) errs() << "dest node: " << node->get_Name() << "\n";
                  ARC* arc = myDFG->get_Arc(node5, node);
                  if(arc != NULL)
                    myDFG->Remove_Arc(arc); 
                  if(DEBUG1) errs() << "else it: " << it << "\n";
                  if(DEBUG1) errs() << "else ii: " << ii << "\n";

		  std::vector<NODE*> from_node = node->Get_Prev_Nodes();
                  for(unsigned int ii = 0; ii < from_node.size(); ii++)
                  {
                    if(!from_node[ii]->is_Store_Address_Generator())
                    {
		      in_node = from_node[ii];
		      node5 = in_node;
		      break;
                    }
                  }
                  if(in_node != NULL)
                  {
		    if(DEBUG1) errs() << "in_node: " << in_node->get_Name() << "\n";
		    ARC* arc;
                    arc = myDFG->get_Arc(in_node, node);
                    if(arc != NULL)
                      myDFG->Remove_Arc(arc);
                  }
		  
                  Instruction* cond;
                  //if(it == 0)
                    cond = dyn_cast<llvm::Instruction>(condSet[ii]->getCondition());
                  //else
                  //  cond = dyn_cast<llvm::Instruction>(condSet[0]->getCondition());
                  NODE* node6 = myDFG->get_Node(cond);
                  if(DEBUG1) errs() << "else node6: " << node6->get_Name() << "\n";
                  if(it == 0)
                  {
                    myDFG->make_Arc(node_ld_data, node3, EdgeID++, 0, TrueDep, 0);
                    myDFG->make_Arc(node5, node3, EdgeID++, 0, TrueDep, 1); 
                    myDFG->make_Arc(node6, node3, EdgeID++, 0, PredDep, 2);
                  }
                  else
                  {
                    myDFG->make_Arc(node5, node3, EdgeID++, 0, TrueDep, 0);
                    myDFG->make_Arc(node_ld_data, node3, EdgeID++, 0, TrueDep, 1);
                    myDFG->make_Arc(node6, node3, EdgeID++, 0, PredDep, 2);
                  }
                  node3->setDatatype(node->getDatatype()); 
                  myDFG->make_Arc(node3, node, EdgeID++, 0, TrueDep, 0);
		}

		// Find predicating level by traversing from current st inst block to the first unconditional/loop latch/loop head block
		// First find bbs containing current store inst
		int level = 0;
		bool curr_if_block = false;
	        Instruction* current_block_ins = dyn_cast<llvm::Instruction>(ifBlocks[ii]->begin());
		{
		for(auto block_it = ifBlocks[ii]->begin(); block_it != ifBlocks[ii]->end(); ++block_it){
		  Instruction* block_ins = dyn_cast<llvm::Instruction>(block_it);
		  Instruction* curr_inst = dyn_cast<llvm::Instruction>(BBI);
		  if(block_ins == curr_inst) curr_if_block = true;
		}
		}
		if(!curr_if_block){
		  current_block_ins = dyn_cast<llvm::Instruction>(elseBlocks[ii]->begin());
		  bool block_found = false;
		  for(auto block_it = elseBlocks[ii]->begin(); block_it != elseBlocks[ii]->end(); ++block_it){
		    Instruction* block_ins = dyn_cast<llvm::Instruction>(block_it);
		    Instruction* curr_inst = dyn_cast<llvm::Instruction>(BBI);
		    if(block_ins == curr_inst) block_found = true;
		  }
		  if(!block_found){
		    errs() << "ERROR: failed to find current inst block!\n";
		    exit(1);
		  }
		}
		
		if(ii >= 1){ // current inst is in if block
		  // Find upper-level block (if or else) that points to current block using current_block_ins and if/elseBlocks[ii-1]
		  // Iterate until reaching unconditional/latch/head block
		  int block_i = ii;
		  bool stop = false;
		  do{
		    BasicBlock* parent_block_if = ifBlocks[block_i-1];
		    bool if_block_found = false;
		    for(auto if_BBI = parent_block_if->begin(); if_BBI != parent_block_if->end(); ++if_BBI){
		      BranchInst* brIns = dyn_cast<llvm::BranchInst>(if_BBI);
		      if(brIns != NULL){
			if(brIns->isUnconditional()) continue;
			Instruction* parent_br_succ;
			if(curr_if_block)
			  parent_br_succ = dyn_cast<llvm::Instruction>(brIns->getSuccessor(0)->begin());
			else
			  parent_br_succ = dyn_cast<llvm::Instruction>(brIns->getSuccessor(1)->begin());

			errs() << "  brins: " << *((Instruction*)brIns) << "\n";
			errs() << "  parent_br_succ: " << *parent_br_succ << " - current_block_ins: " << *current_block_ins << "\n";
			
			if(parent_br_succ == current_block_ins){
			  level++;
			  block_i--;
			  curr_if_block = true;
			  if_block_found = true;
			  current_block_ins = dyn_cast<llvm::Instruction>(parent_block_if->begin());

			  if(std::find(unconditional_blocks.begin(), unconditional_blocks.end(), parent_block_if) != unconditional_blocks.end() || \
			     parent_block_if == loopLatch)
			    stop = true;
			  
			  errs() << "    Found block in if! Level = " << level << " - block_i: " << block_i << " - stop: " << stop << "\n";
			  break;
			}
		      }
		    }
		    bool else_block_found = false;
		    if(!if_block_found){
		      BasicBlock* parent_block_else = elseBlocks[block_i-1];
		      for(auto else_BBI = parent_block_else->begin(); else_BBI != parent_block_else->end(); ++else_BBI){
			BranchInst* brIns = dyn_cast<llvm::BranchInst>(else_BBI);
			if(brIns != NULL){
			  if(brIns->isUnconditional()) continue;
			  Instruction* parent_br_succ;
			  if(curr_if_block)
			    parent_br_succ = dyn_cast<llvm::Instruction>(brIns->getSuccessor(0)->begin());
			  else
			    parent_br_succ = dyn_cast<llvm::Instruction>(brIns->getSuccessor(1)->begin());

			  errs() << "  brins: " << *((Instruction*)brIns) << "\n";
			  errs() << "  parent_br_succ: " << *parent_br_succ << " - current_block_ins: " << *current_block_ins << "\n";
			  
			  if(parent_br_succ == current_block_ins){
			    level++;
			    block_i--;
			    curr_if_block = false;
			    else_block_found = true;
			    current_block_ins = dyn_cast<llvm::Instruction>(parent_block_else->begin());

			    if(std::find(unconditional_blocks.begin(), unconditional_blocks.end(), parent_block_else) != unconditional_blocks.end() || \
			       parent_block_else == loopLatch)
			      stop = true;
			    
			    errs() << "    Found block in else! Level = " << level << " - block_i: " << block_i << " - stop: " << stop << "\n";
			    break;
			  }
			}
		      }
		    }
		    if(!if_block_found && !else_block_found){
		      errs() << "WARNING: parent block of current inst block: " << *current_block_ins << " not found!\n";
		      break;
		    }
		  } while(block_i > 0);
		}

		  if(DEBUG1) errs() << "Predicating level: " << level << "\n";
		
		if(level >= 1){
		  if(DEBUG1) errs() << "  ii = 1\n";
		  // First find the parent predicating node
		  //BasicBlock* pred_inst = condSet[ii-1];
		  BranchInst* pred_BrInst = condSet[ii-1]; // (dyn_cast<llvm::BranchInst>(pred_inst));
		  if(DEBUG1)
		    errs() << "  pred_inst: " << (*pred_BrInst) << "\n";
		    

		  // BasicBlock* curr_pred = condSet[ii];
		  BranchInst* curr_BrInst = condSet[ii]; // (dyn_cast<llvm::BranchInst>(curr_pred));
		  if(DEBUG1)
		    errs() <<	"  curr_inst: " << (*curr_BrInst) << "\n";

		  bool block_with_current_pred = false;   // if or else parent block containing next level pred - 1 for if, 0 for else
		  for(auto if_BBI = ifBlocks[ii-1]->begin(); if_BBI != ifBlocks[ii-1]->end(); ++if_BBI){
		    Instruction* block_ins = dyn_cast<llvm::Instruction>(if_BBI);
		    Instruction* cond_ins = (Instruction*) condSet[ii];
		    if(block_ins == cond_ins) block_with_current_pred = true;
		  }
		    
		  bool block_with_current_inst = false;  // if or else current block containing store inst - 1 for if, 0 for else
		  for(auto if_BBI = ifBlocks[ii]->begin(); if_BBI != ifBlocks[ii]->end(); ++if_BBI)
		    if(if_BBI == BBI) block_with_current_inst = true;

		  bool common_successor = false;
		  if(block_with_current_pred){
		    BasicBlock* pred_else = elseBlocks[ii-1];
		    if(block_with_current_inst){
		      BasicBlock* curr_else = elseBlocks[ii];
		      if(pred_else == curr_else) common_successor = true;
		    } else {
		      BasicBlock* curr_if = ifBlocks[ii];
		      if(pred_else == curr_if) common_successor = true;
		    }
		  } else {
		    BasicBlock* pred_if = ifBlocks[ii-1];
		    if(block_with_current_inst){
		      BasicBlock* curr_else = elseBlocks[ii];
		      if(pred_if == curr_else) common_successor = true;
		    } else {
		      BasicBlock* curr_if = ifBlocks[ii];
		      if(pred_if == curr_if) common_successor = true;
		    }
		  }

		  if(DEBUG1) errs() << "  current pred if block: " << block_with_current_pred << " - current inst if block: " << block_with_current_inst << "\n  common successor: " << common_successor << "\n";

		  if(common_successor){
		    Instruction* cond  = dyn_cast<llvm::Instruction>(pred_BrInst->getCondition());
		    NODE* parent_node = myDFG->get_Node(cond);
		    cond  = dyn_cast<llvm::Instruction>(curr_BrInst->getCondition());
		    NODE* current_node = myDFG->get_Node(cond);

		    if(DEBUG1) errs() << "  parent node: " << parent_node->get_Name() << " - current node: " << current_node->get_Name() << " - node3: " << node3->get_Name() << " - node5: " << node5->get_Name() << " - node_ld_data: " << node_ld_data->get_Name() << "\n";

		    if(block_with_current_pred){
		      ARC* temp = myDFG->get_Arc(current_node, node3);
		      if(temp != NULL)
			myDFG->Remove_Arc(temp);
		      
		      if(block_with_current_inst){
			NODE* new_combine = new NODE(andop, 1, NodeID++, "", NULL);
			new_combine->setDatatype(int32);
			myDFG->insert_Node(new_combine);
			
			myDFG->make_Arc(current_node, new_combine, EdgeID++, 0, TrueDep, 0);
			myDFG->make_Arc(parent_node, new_combine, EdgeID++, 0, TrueDep, 1);
			myDFG->make_Arc(new_combine, node3, EdgeID++, 0, PredDep, 2);

		      } else {
			NODE* curr_pred_neg = new NODE(xorop, 1, NodeID++, "", NULL);
			curr_pred_neg->setDatatype(int32);
			myDFG->insert_Node(curr_pred_neg);
			NODE* const1 = myDFG->get_Node("ConstInt1");
			if(const1 == NULL){
			  const1 = new NODE(constant, 1, NodeID++, "ConstInt1", NULL);
			  myDFG->insert_Node(const1);
			}
			
			myDFG->make_Arc(current_node, curr_pred_neg, EdgeID++, 0, TrueDep, 0);
			myDFG->make_Arc(const1, curr_pred_neg, EdgeID++, 0, TrueDep, 1);

			NODE* new_combine = new NODE(andop, 1, NodeID++, "", NULL);
			new_combine->setDatatype(int32);
			myDFG->insert_Node(new_combine);
			myDFG->make_Arc(curr_pred_neg, new_combine, EdgeID++, 0, TrueDep, 0);
			myDFG->make_Arc(parent_node, new_combine, EdgeID++, 0, TrueDep, 1);
			myDFG->make_Arc(new_combine, node3, EdgeID++, 0, PredDep, 2);
		      }
		    } else {
		      ARC* temp = myDFG->get_Arc(current_node, node3);
		      if(temp != NULL)
			myDFG->Remove_Arc(temp);

		      NODE* parent_pred_neg = new NODE(xorop, 1, NodeID++, "", NULL);
		      parent_pred_neg->setDatatype(int32);
		      myDFG->insert_Node(parent_pred_neg);
		      NODE* const1 = myDFG->get_Node("ConstInt1");
		      if(const1 == NULL){
			const1 = new NODE(constant, 1, NodeID++, "ConstInt1", NULL);
			myDFG->insert_Node(const1);
		      }
		      myDFG->make_Arc(parent_node, parent_pred_neg, EdgeID++, 0, TrueDep, 0);
		      myDFG->make_Arc(const1, parent_pred_neg, EdgeID++, 0, TrueDep, 1);

		      if(block_with_current_inst){
			NODE* new_combine = new NODE(andop, 1, NodeID++, "", NULL);
			new_combine->setDatatype(int32);
			myDFG->insert_Node(new_combine);

			myDFG->make_Arc(current_node, new_combine, EdgeID++, 0, TrueDep, 0);
			myDFG->make_Arc(parent_pred_neg, new_combine, EdgeID++, 0, TrueDep, 1);
			myDFG->make_Arc(new_combine, node3, EdgeID++, 0, PredDep, 2);
		      } else {
			NODE* curr_pred_neg = new NODE(xorop, 1, NodeID++, "", NULL);
			curr_pred_neg->setDatatype(int32);
			myDFG->insert_Node(curr_pred_neg);
			myDFG->make_Arc(current_node, curr_pred_neg, EdgeID++, 0, TrueDep, 0);
			myDFG->make_Arc(const1, curr_pred_neg, EdgeID++, 0, TrueDep, 1);

			NODE* new_combine = new NODE(andop, 1, NodeID++, "", NULL);
			new_combine->setDatatype(int32);
			myDFG->insert_Node(new_combine);
			  
			myDFG->make_Arc(curr_pred_neg, new_combine, EdgeID++, 0, TrueDep, 0);
			myDFG->make_Arc(parent_pred_neg, new_combine, EdgeID++, 0, TrueDep, 1);
			myDFG->make_Arc(new_combine, node3, EdgeID++, 0, PredDep, 2);
		      }	
		    }

		    if(myDFG->get_Arc(node5, node3) != NULL && myDFG->get_Arc(node5, node3)->GetOperandOrder() == 0){
		      myDFG->get_Arc(node5, node3)->SetOperandOrder(1);
		      myDFG->get_Arc(node_ld_data, node3)->SetOperandOrder(0);
		    }
		  } else {
		    errs() << "Incommon successor! Implement me!\n";
		    exit(0);
		  }

		  // Second level of predication
		  if(level >= 2){
		    if(DEBUG1) errs() << "  ii = 2\n";
		    //BasicBlock* pred_inst = condSet[ii-2];
		    BranchInst* pred_BrInst = condSet[ii-2]; // (dyn_cast<llvm::BranchInst>(pred_inst));
		    if(DEBUG1)
		      errs() << "  pred_inst: " << (*pred_BrInst) << "\n";
		      
		    //BasicBlock* curr_pred = condSet[ii-1];
		    BranchInst* curr_BrInst = condSet[ii-1]; // (dyn_cast<llvm::BranchInst>(curr_pred));
		    if(DEBUG1)
		      errs() << "  curr_inst: " << (*curr_BrInst) << "\n";

		    bool block_with_current_pred = false;   // if or else parent block containing next level pred - 1 for if, 0 for else
		    for(auto if_BBI = ifBlocks[ii-2]->begin(); if_BBI != ifBlocks[ii-2]->end(); ++if_BBI){
		      Instruction* block_ins = dyn_cast<llvm::Instruction>(if_BBI);
		      Instruction* cond_ins = (Instruction*) condSet[ii-1];
		      if(block_ins == cond_ins) block_with_current_pred = true;
		    }

		    bool block_with_next_pred = false;   // if or else current block containing next level pred - 1 for if, 0 for else
		    for(auto if_BBI = ifBlocks[ii-1]->begin(); if_BBI != ifBlocks[ii-1]->end(); ++if_BBI){
		      Instruction* block_ins = dyn_cast<llvm::Instruction>(if_BBI);
		      Instruction* cond_ins = (Instruction*) condSet[ii];
		      if(block_ins == cond_ins) block_with_next_pred = true;
		    }

		    bool common_successor = false;
		    if(block_with_current_pred){
		      BasicBlock* pred_else = elseBlocks[ii-2];
		      if(block_with_next_pred){
			BasicBlock* curr_else = elseBlocks[ii-1];
			if(pred_else == curr_else) common_successor = true;
		      } else {
			BasicBlock* curr_if = ifBlocks[ii-1];
			if(pred_else == curr_if) common_successor = true;
		      }
		    } else {
		      BasicBlock* pred_if = ifBlocks[ii-2];
		      if(block_with_next_pred){
			BasicBlock* curr_else = elseBlocks[ii-1];
			if(pred_if == curr_else) common_successor = true;
		      } else {
			BasicBlock* curr_if = ifBlocks[ii-1];
			if(pred_if == curr_if) common_successor = true;
		      }
		    }
		    if(DEBUG1) errs() << "  current pred if block: " << block_with_current_pred << " - next pred if block: " << block_with_next_pred << "\n";
		    if(DEBUG1) errs() << "  Common successor: " << common_successor << "\n";
		    if(common_successor){
		      Instruction* cond  = dyn_cast<llvm::Instruction>(pred_BrInst->getCondition());
		      NODE* parent_node = myDFG->get_Node(cond);
		      cond  = dyn_cast<llvm::Instruction>(curr_BrInst->getCondition());
		      NODE* current_node = myDFG->get_Node(cond);
		      NODE* combine_node;
		      if((current_node->Get_Next_Nodes()).size() > 1){
			for(auto curr_it = current_node->Get_Next_Nodes().begin(); curr_it != current_node->Get_Next_Nodes().end(); ++curr_it)
			  if((*curr_it)->Get_Next_Nodes()[0] == node3) combine_node = (*curr_it);
		      } else 
			combine_node = current_node->Get_Next_Nodes()[0];
			  
		      if(DEBUG1) errs() << "  parent node: " << parent_node->get_ID() << " - current node: " << current_node->get_ID() << " - combine node: " << combine_node->get_ID() << "\n";

		      if(block_with_current_pred && block_with_next_pred){
			NODE* new_combine = new NODE(andop, 1, NodeID++, "", NULL);
			new_combine->setDatatype(int32);
			myDFG->insert_Node(new_combine);
			myDFG->Remove_Arc(myDFG->get_Arc(current_node, combine_node));
			myDFG->make_Arc(current_node, new_combine, EdgeID++, 0, TrueDep, 0);
			myDFG->make_Arc(parent_node, new_combine, EdgeID++, 0, TrueDep, 1);
			myDFG->make_Arc(new_combine, combine_node, EdgeID++, 0, TrueDep, 1);

		      } else if(!block_with_current_pred && block_with_next_pred) {
			NODE* parent_pred_neg = new NODE(xorop, 1, NodeID++, "", NULL);
			parent_pred_neg->setDatatype(int32);
			myDFG->insert_Node(parent_pred_neg);
			NODE* const1 = myDFG->get_Node("ConstInt1");
			if(const1 == NULL) {
			  const1 = new NODE(constant, 1, NodeID++, "ConstInt1", NULL);
			  myDFG->insert_Node(const1);
			}
			myDFG->make_Arc(parent_node, parent_pred_neg, EdgeID++, 0, TrueDep, 0);
			myDFG->make_Arc(const1, parent_pred_neg, EdgeID++, 0, TrueDep, 1);

			NODE* new_combine = new NODE(andop, 1, NodeID++, "", NULL);
			new_combine->setDatatype(int32);
			myDFG->insert_Node(new_combine);

			myDFG->Remove_Arc(myDFG->get_Arc(current_node, combine_node));
			myDFG->make_Arc(current_node, new_combine, EdgeID++, 0, TrueDep, 0);
			myDFG->make_Arc(parent_pred_neg, new_combine, EdgeID++, 0, TrueDep, 1);
			myDFG->make_Arc(new_combine, combine_node, EdgeID++, 0, TrueDep, 1);
			  
		      } else if(block_with_current_pred && !block_with_next_pred){
			NODE* curr_pred_neg = current_node->Get_Next_Nodes()[0];
			if(DEBUG1) errs() << "  curr_pred_neg: " << curr_pred_neg->get_ID() << "\n";
			  
			NODE* new_combine = new NODE(andop, 1, NodeID++, "", NULL);
			new_combine->setDatatype(int32);
			myDFG->insert_Node(new_combine);

			myDFG->Remove_Arc(myDFG->get_Arc(curr_pred_neg, combine_node));
			myDFG->make_Arc(curr_pred_neg, new_combine, EdgeID++, 0, TrueDep, 0);
			myDFG->make_Arc(parent_node, new_combine, EdgeID++, 0, TrueDep, 1);
			myDFG->make_Arc(new_combine, combine_node, EdgeID++, 0, TrueDep, 1);
			  
		      } else {
			NODE* parent_pred_neg = new NODE(xorop, 1, NodeID++, "", NULL);
			parent_pred_neg->setDatatype(int32);
			myDFG->insert_Node(parent_pred_neg);
			NODE* const1 = myDFG->get_Node("ConstInt1");
			if(const1 == NULL) {
			  const1 = new NODE(constant, 1, NodeID++, "ConstInt1", NULL);
			  myDFG->insert_Node(const1);
			}
			myDFG->make_Arc(parent_node, parent_pred_neg, EdgeID++, 0, TrueDep, 0);
			myDFG->make_Arc(const1, parent_pred_neg, EdgeID++, 0, TrueDep, 1);
			  
			NODE* curr_pred_neg = current_node->Get_Next_Nodes()[0];
			if(DEBUG1) errs() << "  curr_pred_neg: " << curr_pred_neg->get_ID() << "\n";
			  
			NODE* new_combine = new NODE(andop, 1, NodeID++, "", NULL);
			new_combine->setDatatype(int32);
			myDFG->insert_Node(new_combine);
			  
			myDFG->Remove_Arc(myDFG->get_Arc(curr_pred_neg, combine_node));
			myDFG->make_Arc(curr_pred_neg, new_combine, EdgeID++, 0, TrueDep, 0);
			myDFG->make_Arc(parent_pred_neg, new_combine, EdgeID++, 0, TrueDep, 1);
			myDFG->make_Arc(new_combine, combine_node, EdgeID++, 0, TrueDep, 1);
		      }
		    } else {
		      errs() << "*** Incommon successor -> Implement me! ***\n";
		      exit(1);
		    }
		  }
                }
              }
              else
              {
                if(DEBUG1) errs() << "PTR operand is null\n";
              }
            }
          }
          //} 
        }

      }
      // This for loop collects the incoming variable ptr or simple node to the store.
      // Particularly to the st_add. 
      //for(unsigned int ii = 0; ii < condBlocks.size(); ii++)
      // {
      //errs() << "ii: " << ii << "\n";
      // std::vector<bool> varLoaded;
      // std::vector<Instruction*> variableValue;
      // std::vector<Value*> variablePtr;
      // std::vector<unsigned> variableAlignment;
      // std::vector<std::string> storeArrNames;
      //std::vector<int> arrayFound;
      //std::vector<Instruction *> arrayIndex;

      //errs() << "before for loop: " << ii << "\n";
      /*for(unsigned int it = 0; it < 2; it++)
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
        if(DEBUG1)
        errs() << "in store: " << *BBI << "\n";
        StoreInst *stInst = (dyn_cast<llvm::StoreInst>(BBI));
      // TODO
      Instruction *ptrOperand = (dyn_cast<Instruction>(stInst->getPointerOperand()));
      if(DEBUG1)
      {
      errs() << "ptroperand: " << *ptrOperand << "\n";
      }
      if(ptrOperand != nullptr)
      {
      GetElementPtrInst *ptrInst = dyn_cast<GetElementPtrInst>(stInst->getPointerOperand());
      std::string varName; 
      if(ptrInst != nullptr) 
      {
      varName = ptrInst->getPointerOperand()->getName().str();

      //if(varName.empty())
      //  {
      //  varName = myDFG->get_Node(ptrInst->getPointerOperand())->get_Name(); 
      //  errs() << "varname: " << varName << "\n";
      //  varName = myDFG->get_Node(stInst->getPointerOperand())->get_Name();
      //  errs() << "varname: " << varName << "\n";

      // }

      if(varName.empty()) varName = myDFG->get_Node(stInst->getPointerOperand())->get_Name();
      }
      else
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
    }*/
    // Iterate for each variable or array
    /*for(unsigned int it = 0; it < storeVarNames.size(); it++)
      {
      bool presentInBothBlks = (varFound[it] == 2);
      if(DEBUG1) errs() << "present if both blks: " << presentInBothBlks << "\n";
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
      selectNode->setDatatype(node1->getDatatype()); 
      myDFG->insert_Node(selectNode);
    Datatype dt;      

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
        if(T->isIntegerTy())
        {
          if(alignment == 1)
            dt = character;
          if(alignment == 2)
            dt = int16;
          else  
            dt = int32;
        }
        if(T->isFloatingPointTy())
        {
          if(alignment == 1)
            dt = character;
          if(alignment == 2)
            dt = int16;
          else
            dt = int32;
        }
      }
      else if(T->isPointerTy())
      {
        PointerType* PointerTy_0 = dyn_cast<PointerType>(T);// PointerType::get(T, 0);
        ConstantPointerNull* const_ptr_2 = ConstantPointerNull::get(PointerTy_0);
        SequentialType* PT = dyn_cast<SequentialType>(T);
        //errs() << "after get seq ptr\n";

        int bit_width;
        if(PT != nullptr)
        { 
          if(PT->getPrimitiveSizeInBits() % 8 == 0)
          {
            //errs()<< "inside if\n";
            bit_width = (PT->getElementType()->getPrimitiveSizeInBits())/8;
          }
          else
          {
            //errs() << "inside else\n";
            bit_width = PT->getElementType()->getPrimitiveSizeInBits() %8;
          }
          //errs() << "passed bit width\n"; 
          if(bit_width == 4)
          {
            if(PT->getElementType()->isIntegerTy())
              dt = int32;
            else
              dt = float32;  
          }
          else if(bit_width == 8)
          {
            dt = float64;
          }
          else if(bit_width < 4)
          {
            dt = character;
          }
        }
        else
        {
          if(T->getPrimitiveSizeInBits() % 8 == 0)
          {
            //errs()<< "inside if\n";
            bit_width = (T->getPrimitiveSizeInBits())/8;
          }
          else
          {
            //errs() << "inside else\n";
            bit_width = T->getPrimitiveSizeInBits() %8;
          }
          //errs() << "passed bit width\n";
          if(bit_width == 4)
          {
            if(T->isIntegerTy())
              dt = int32;
            else
              dt = float32;
          }
          else if(bit_width == 8)
          {
            dt = float64;
          }
          else if(bit_width < 4)
          {
            dt = character;
          }
        } 
        dummyStore->setInitializer(const_ptr_2);
        alignment = T->getPointerAddressSpace();
        if(alignment < 4) alignment = 4;
      }
      dummyStore->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);
      constInt->setDatatype(dt);
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
    }*/
    //}

    }

    BasicBlock * SearchCommonSuccessorForBranching(BasicBlock *bb, BasicBlock *bb1, BasicBlock *bb2, BasicBlock *loopLatch)
    {
      if(DEBUG1)
        errs() << "We are in searchcommon\n";
      BasicBlock* retBlk = NULL;
      if((bb1 == NULL) || (bb2 == NULL)) return retBlk;

      BasicBlock* succ1 = bb1->getSingleSuccessor();
      BasicBlock* succ2 = bb2->getSingleSuccessor();
      if((succ1 == succ2) && (succ1 != NULL))
      {
        if(DEBUG1)
          errs() << "succ1\n";
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
        if(DEBUG1)
          errs() << "we are in else\n";
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
          if(DEBUG1) errs() << "succ2 NULL and bb not looplatch\n";
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

      if(DEBUG)
        errs() << "DEBUG -- From Update_dependency\n";

      if(DEBUG) {
        errs() << "Instruction: " << *BI << "\n";
        errs() << " before trun\n";
      }

      if(BI->getOpcode() == Instruction::FPToUI || BI->getOpcode() == Instruction::FPToSI)
      {
        node1 = myDFG->get_Node(BI);
        node2 = new NODE(constant, 1, NodeID++, "ConstInt0", NULL);
        node2->setDatatype((Datatype) int32);
        myDFG->insert_Node(node2);
        myDFG->make_Arc(node2, node1, EdgeID++, 0, TrueDep, 1);

      }
      if(BI->getOpcode() == Instruction::UIToFP || BI->getOpcode() == Instruction::SIToFP)
      {
        node1 = myDFG->get_Node(BI);
        node2 = new NODE(constant, 1, NodeID++, "ConstFP0", NULL);
        node2->setDatatype((Datatype) float32);
        myDFG->insert_Node(node2);
        myDFG->make_Arc(node2, node1, EdgeID++, 0, TrueDep, 1);

      }
      if (BI->getOpcode() == Instruction::Trunc || BI->getOpcode() == Instruction::FPTrunc)
      {
        Type* T; 
        if(BI->getOpcode() == Instruction::Trunc)
          T = (dyn_cast<TruncInst>(BI))->getDestTy();
        else
          T = (dyn_cast<FPTruncInst>(BI))->getDestTy();
        unsigned mask = (1 << T->getPrimitiveSizeInBits()) - 1;

	if(mask == 0 || mask > (0xffffffffUL))
	  mask = (1 << 32) - 1; // cap to 32 bits
	
        node1 = myDFG->get_Node(BI);
        if(BI->getOpcode() == Instruction::Trunc)
          node2 = new NODE(constant, 1, NodeID++, "ConstInt"+std::to_string(mask), NULL);
        else
          node2 = new NODE(constant, 1, NodeID++, "ConstFP"+std::to_string(mask), NULL);
        myDFG->insert_Node(node2);
        node2->setDatatype(node1->getDatatype()); 
        myDFG->make_Arc(node2, node1, EdgeID++, 0, TrueDep, 1);
      }
      if(BI->getOpcode() == Instruction::BitCast)
      {
        node1 = myDFG->get_Node(BI);
        if(node1->getDatatype() == character || node1->getDatatype() == int16 || node1->getDatatype() == int32)
        {
          node2 = new NODE(constant, 1, NodeID++, "ConstInt0", NULL);
          myDFG->insert_Node(node2);
          myDFG->make_Arc(node2, node1, EdgeID++, 0, TrueDep, 1);
        }
        else
        {
          node = new NODE(constant, 1, NodeID++, "ConstFP0", NULL);
          myDFG->insert_Node(node2);
          myDFG->make_Arc(node2, node1, EdgeID++, 0, TrueDep, 1);
        }
      }
      if(DEBUG)
        errs() << " before sext\n";
      if((BI->getOpcode() == Instruction::SExt))
      {
        node1 = myDFG->get_Node(BI);
        Value *v = dyn_cast<Value>(BI->getOperand(0));
        Type* T = v->getType();
        unsigned mask = T->getPrimitiveSizeInBits();
	errs () << "  Mask: " << mask << "\n";
        node2 = new NODE(constant, 1, NodeID++, "ConstInt"+std::to_string(mask), NULL);
        node2->setDatatype(node1->getDatatype());
        myDFG->insert_Node(node2);
        myDFG->make_Arc(node2, node1, EdgeID++, 0, TrueDep, 1);
      }
      if(DEBUG)
        errs() << " before ZExt\n"; 
      if (BI->getOpcode() == Instruction::ZExt || BI->getOpcode() == Instruction::FPExt)
      {
        Value *v = dyn_cast<Value>(BI->getOperand(0));
        Type* T = v->getType();
        unsigned mask = (1 << T->getPrimitiveSizeInBits()) - 1;

	if(mask == 0 || mask > (0xffffffffUL)) mask = (1 << 32) - 1;
	
        node1 = myDFG->get_Node(BI);
        if(BI->getOpcode() == Instruction::ZExt)
          node2 = new NODE(constant, 1, NodeID++, "ConstInt"+std::to_string(mask), NULL);
        else
          node2 = new NODE(constant, 1, NodeID++, "ConstFP"+std::to_string(mask), NULL);
        node2->setDatatype(node1->getDatatype());
        myDFG->insert_Node(node2);
        myDFG->make_Arc(node2, node1, EdgeID++, 0, TrueDep, 1);
      }
      if(DEBUG)
        errs() << " after zext\n";

      
      for (unsigned int j = 0; j < BI->getNumOperands(); j++)
      {
        if(DEBUG) {
          errs() << " BI: " << *BI << "\n";
          errs() << " j: " << j << "\n";
          errs() << " val_id: " << BI->getOperand(j)->getValueID() << "\n";
        }

        dep = TrueDep;
        //constant values can be immediate
        //if it is greater than immediate field; instruction generation should treat it as nonrecurring value
        if (BI->getOperand(j)->getValueID() == llvm::Value::ConstantIntVal)
        {
          if(DEBUG)
            errs() << "  before constintval\n";
          std::ostringstream os;
          int constVal = 0;
          if(dyn_cast<llvm::ConstantInt>((BI)->getOperand(j))->getBitWidth() > 1)
            constVal = dyn_cast<llvm::ConstantInt>((BI)->getOperand(j))->getSExtValue();
          else
            constVal = dyn_cast<llvm::ConstantInt>((BI)->getOperand(j))->getZExtValue();
          os << constVal;
          std::string name = "ConstInt" + os.str();
	  errs() << "  Node name: " << name << "\n";
          Datatype dt;
          //errs() << "before dyncast\n";
          Value *v = dyn_cast<Value>(BI);
          Type* T = v->getType();
          int bit_width = T->getPrimitiveSizeInBits()/8;
          if(bit_width == 1)
            dt = character;
          else if(bit_width == 2)
            dt = int16;
          else
            dt = int32;
          if (myDFG->get_Node((BI)->getOperand(j)) == NULL)
          {
            node1 = new NODE(constant, 1, NodeID++, name, (BI)->getOperand(j));
            node1->setDatatype(dt);
            myDFG->insert_Node(node1);
          }
          else
            node1 = myDFG->get_Node((BI)->getOperand(j));
          node1->setDatatype(dt); 
          node2 = myDFG->get_Node(BI);
          if(BI->getOpcode() != Instruction::Store)
            myDFG->make_Arc(node1, node2, EdgeID++, 0, dep,j);
          continue;
        }
        else if (BI->getOperand(j)->getValueID() == llvm::Value::ConstantFPVal)
        {
          if(DEBUG)
            errs() << "  before constFPval\n";
          //We support only single precision fp. But sometimes IR converts the float to doubles.
          // SO we need to get the appropriate FP val.
          std::ostringstream os;
          Datatype dt;
          Value *v = dyn_cast<Value>(BI);
          Type* T = v->getType();
          int bit_width = T->getPrimitiveSizeInBits()/8;
          if(bit_width == 8)
            dt = float64;
          else if(bit_width == 2)
            dt = float16;
          else if(bit_width == 4)
            dt = float32;

          if(dt == float64)
            os << cast<ConstantFP>((BI)->getOperand(j))->getValueAPF().convertToDouble();
          else
            os << cast<ConstantFP>((BI)->getOperand(j))->getValueAPF().convertToFloat();
          std::string name = "ConstFP" + os.str();

          if (myDFG->get_Node((BI)->getOperand(j)) == NULL)
          {
            node1 = new NODE(constant, 1, NodeID++, name, (BI)->getOperand(j));
            node1->setDatatype(dt);
            myDFG->insert_Node(node1);
          }
          else
            node1 = myDFG->get_Node((BI)->getOperand(j));
          node1->setDatatype(dt);
          node2 = myDFG->get_Node(BI);
          if(BI->getOpcode() != Instruction::Store)
            myDFG->make_Arc(node1, node2, EdgeID++, 0, dep,j);
          continue;
        }
        else if (BI->getOperand(j)->getValueID() == llvm::Value::BasicBlockVal)
        {
          if(DEBUG) {
	    errs() << "  !!BasicBlockVal Detected!!\n";
            errs() << "  intr: " << *BI << "\n";
            errs() << "  val: " <<  BI->getOperand(j) << "\n"; 
          }
          BasicBlock *bb = dyn_cast<llvm::BasicBlock>(BI->getOperand(j)); 

          //for(BasicBlock::iterator it  = bb->begin(); it != bb->end(); ++it) 
          //{
          //errs() << "instr inside : " << *it << "\n" ;
          //}

          continue;
        }
        else if ((BI->getOperand(j)->getValueID() >= llvm::Value::InstructionVal) ||
            (BI->getOperand(j)->getValueID() == llvm::Value::GlobalVariableVal) ||
            (BI->getOperand(j)->getValueID() == llvm::Value::ArgumentVal))
        {
          if(DEBUG)
            errs() << "  Inside globalvariable\n";
          if(BI->getOperand(j)->getValueID() == llvm::Value::GlobalVariableVal)
          {
            if(BI->getOpcode() != Instruction::Load && BI->getOpcode() != Instruction::Store)
              continue;
          }

          std::string name;
          name = cast<Instruction>((BI)->getOperand(j))->getName().str();
          int distance = 0;
          Datatype dt; 
          if(DEBUG)
            errs() << "   name: " << name << "\n"; 
          //if the node has not been created, create it, else get distance
          if (myDFG->get_Node((BI)->getOperand(j)) == NULL)
          {
            if(DEBUG) {
              errs() << "   inside getoperand null\n";
              errs() << "    BI: " << *BI << "\n"; 
              errs() << "    operand: " << BI->getOperand(j)->getValueID() << "\n";
              errs() << "    value name: " << BI->getOperand(j)->getName().str() << "\n";
            }
            Value *defInst = BI->getOperand(j);
            std::map<Value *,std::string>::iterator it;
            it = map_instn_defn_livein_to_load.find(defInst);
            if ((it != map_instn_defn_livein_to_load.end()) ||
                (BI->getOperand(j)->getValueID() == llvm::Value::GlobalVariableVal))
            {
              if(DEBUG)
                errs() << "    inside the if function\n";
              std::string ptrName;
              unsigned alignment = 4;
              if (it != map_instn_defn_livein_to_load.end())
              {
                if(DEBUG)
                {
                  errs() << "     inside if ptr name: " << it->second << "\n";
                  errs() << "     dt: " << map_livein_to_datatype[ptrName] << "\n";
                }
                ptrName = it->second;
                alignment = map_livein_to_alignment[ptrName];
                dt = map_livein_to_datatype[ptrName];
              }
              else
              {
                if(DEBUG)
                  errs() << "    inside else to get gptr name livein: " << name << "\n";
                ptrName = name;
              }

              if(DEBUG)
                errs() << "   passed ifelse\n";
              node1 = new NODE(constant, 1, NodeID++, ptrName, (BI)->getOperand(j));
              node1->setAlignment(alignment);
              node2 = myDFG->get_Node(BI);  
              //llvm::Datatype dt = node2->getDatatype();
              //node1->setDatatype(dt);
              myDFG->insert_Node(node1);
              unsigned int livein_datatype = (unsigned int)(node2->getDatatype());
              dep = LiveInDataDep;
              unsigned int CGRA_loadAddID = ((NodeID++)+100);
              unsigned int CGRA_loadDataID = ((NodeID++)+100);
              // Update LiveIn information
              liveInNodefile << node2->get_ID() << "\t" << node2->get_Instruction() << "\t" << node2->get_Name() << "\t" << livein_datatype << "\n";
              liveInNodefile << node1->get_ID() << "\t" << constant << "\t" << ptrName << "\t" << livein_datatype << "\n";
              liveInNodefile << CGRA_loadAddID << "\t" << ld_add << "\t" << "ld_add_" + ptrName << "\t" << livein_datatype << "\n";
              liveInNodefile << CGRA_loadDataID << "\t" << ld_data << "\t" << "ld_data_" + ptrName << "\t" << livein_datatype <<"\n";

              liveInEdgefile << CGRA_loadAddID << "\t" << CGRA_loadDataID << "\t0\tLRE\t0\n";
              liveInEdgefile << CGRA_loadDataID << "\t" << node2->get_ID() << "\t0\tTRU\t" << j << "\n";
              liveInEdgefile << node1->get_ID() << "\t" << CGRA_loadAddID << "\t0\tTRU\t0\n";
            }
            else
            {
              std::cout << "   !Cannot detect defining instruction for the live-in\n\n";
            }
          }
          else
          {
	    if(DEBUG)
              errs() << "   else getoperand null\n";
	    
	    Value *defInst = BI->getOperand(j);
            std::map<Value *,std::string>::iterator it;
	    
            it = map_instn_defn_livein_to_load.find(defInst);
            if ((it != map_instn_defn_livein_to_load.end()) ||
                (BI->getOperand(j)->getValueID() == llvm::Value::GlobalVariableVal)){
	      errs() << "    inside if for livein or globalvariable\n";
	      node1 = myDFG->get_Node(BI->getOperand(j));
	      node2 = myDFG->get_Node(BI);
	      std::string ptrName = node1->get_Name();
	      unsigned int livein_datatype = (unsigned int)(node2->getDatatype());
              dep = LiveInDataDep;
	      distance = 0;
              unsigned int CGRA_loadAddID = ((NodeID++)+100);
              unsigned int CGRA_loadDataID = ((NodeID++)+100);

	      liveInNodefile << node2->get_ID() << "\t" << node2->get_Instruction() << "\t" << node2->get_Name() << "\t" << livein_datatype << "\n";
              liveInNodefile << node1->get_ID() << "\t" << constant << "\t" << ptrName << "\t" << livein_datatype << "\n";
              liveInNodefile << CGRA_loadAddID << "\t" << ld_add << "\t" << "ld_add_" + ptrName << "\t" << livein_datatype << "\n";
              liveInNodefile << CGRA_loadDataID << "\t" << ld_data << "\t" << "ld_data_" + ptrName << "\t" << livein_datatype <<"\n";

	      liveInEdgefile << CGRA_loadAddID << "\t" << CGRA_loadDataID << "\t0\tLRE\t0\n";
              liveInEdgefile << CGRA_loadDataID << "\t" << node2->get_ID() << "\t0\tTRU\t" << j << "\n";
              liveInEdgefile << node1->get_ID() << "\t" << CGRA_loadAddID << "\t0\tTRU\t0\n";
	    }
	    else
	      distance = getDistance(cast<Instruction>((BI)->getOperand(j)),
                                   cast<Instruction>((BI)->getOperand(j))->getParent(), BI, BI->getParent(), bbs,loopLatch);
          }

          //if the operand instruction corresponds to a load operation
          if (cast<Instruction>((BI)->getOperand(j))->getOpcode() == Instruction::Load)
          {
            if(DEBUG)
              errs() << "  if load\n"; 
            node1 = myDFG->get_Node_Mem_Data(cast<Instruction>((BI)->getOperand(j)));
            if (node1 == NULL)
            {
              node1 = myDFG->get_Node((BI)->getOperand(j));
            }
          }
          else
          { 
            if(DEBUG)
              errs() << "  else load\n";
            node1 = myDFG->get_Node((BI)->getOperand(j));
          }

          //if its a store instruction
          if (BI->getOpcode() == Instruction::Store)
          {
            if(DEBUG)
              errs() << "  in opcode store\n";
            if((dyn_cast<StoreInst>(BI))->getPointerOperand() == BI->getOperand(j))
            {
              if(DEBUG) {
                errs() << "   in if\n";
                errs() << "   BI: " << *BI << "\n";
              }
              node2 = myDFG->get_Node_Mem_Add(BI);
              if(DEBUG) {
                errs() << "   passed node2: " << node2->get_Name() << "\n";
                errs() << "   value id for store:" << (dyn_cast<StoreInst>(BI))->getValueOperand()->getValueID() << "\n"; 
              }

              if(((dyn_cast<StoreInst>(BI))->getValueOperand()->getValueID() == llvm::Value::ConstantIntVal) || ((dyn_cast<StoreInst>(BI))->getValueOperand()->getValueID() == llvm::Value::ConstantFPVal)) {
                if(DEBUG)
                  errs() << "    before get node\n";
                node3 = myDFG->get_Node((dyn_cast<StoreInst>(BI))->getValueOperand());
                //Datatype dt1 = node2->getDatatype();
                node4 = myDFG->get_Node_Mem_Data(BI);
		
                if(DEBUG){
                  errs() << "    node3: " << node3->get_Name() << " -> node4: " << node4->get_Name() << "\n";
		}
                myDFG->make_Arc(node3, node4, EdgeID++, 0, TrueDep,0);
                //node3->setDatatype(dt1);
                //node4->setDatatype(dt1); 
              }
              else
              {
                node3 = myDFG->get_Node(BI->getOperand(j));
                if(DEBUG)
                  errs() << "   get the other incoming to store: " << node3->get_ID() << "\n";
                node4 = myDFG->get_Node_Mem_Data(BI);
                //Datatype dt1 = node3->getDatatype();
                //node2->setDatatype(dt1);
                //node4->setDatatype(dt1);
              }
              if(DEBUG)
                errs() << "   exiting if\n";
            }
            else
            {
              if(DEBUG)
                errs() << "   in else\n";

              node2 = myDFG->get_Node_Mem_Data(BI);
              if(DEBUG) {
                errs() << "    passed node2: " << node2->get_Name() << "\n";
                errs() << "    value id for store:" << (dyn_cast<StoreInst>(BI))->getValueOperand()->getValueID() << "\n";
                errs() << "    node2 dt: " << node2->getDatatype() << "\n";
              }

              if(((dyn_cast<StoreInst>(BI))->getPointerOperand()->getValueID() == llvm::Value::ConstantIntVal) || ((dyn_cast<StoreInst>(BI))->getPointerOperand()->getValueID() == llvm::Value::ConstantFPVal)) {
                node3 = myDFG->get_Node((dyn_cast<StoreInst>(BI))->getPointerOperand());
                node4 = myDFG->get_Node_Mem_Add(BI);
                if(DEBUG)
                {
                  errs() << "    node4: " << node4->get_ID() << "\n";
                  errs() << "    node 2 dt: " << node2->getDatatype() << "\n";
                  errs() << "    dt: " << node2->getDatatype() << "\n";
                  exit(1);
                }
                //Datatype dt1 = node2->getDatatype();
                // node3->setDatatype(dt1);
                myDFG->make_Arc(node3, node4, EdgeID++, 0, TrueDep,0);
              }
            }
          }
          else if (BI->getOpcode() == Instruction::Load)
          {
            if(DEBUG)
              errs() << "  else load2\n"; 
            node2 = myDFG->get_Node_Mem_Add(BI);
          }
          else
          {
            if(DEBUG) {
              errs() << "  else else load\n";
              errs() << "   BI: " << *BI << "\n"; 
            }
            node2 = myDFG->get_Node(BI);
            if(DEBUG)
              errs() << "   after get node\n";
          }
	  if(DEBUG) errs() << "  node1: " << node1->get_Name() << " - node2: " << node2->get_Name() << "\n";
          myDFG->make_Arc(node1, node2, EdgeID++, distance,dep,j);
        }
      }
      if(DEBUG)
        errs() << " before select\n";
      if(BI->getOpcode() == Instruction::Select)
      {
        if(DEBUG) {
          errs() << " Select BI: " << *BI << "\n"; 
          errs() << "  Select node: " << myDFG->get_Node(BI)->get_ID() << "\n";
        }
        
        //change OperandOrder To 1
        node1 = myDFG->get_Node(BI);
        node = myDFG->get_Node((dyn_cast<SelectInst>(BI))->getCondition());
        arc1 = myDFG->get_Arc(node, node1);
        arc1->SetOperandOrder(2);
        arc1->Set_Dependency_Type(PredDep);
        node = myDFG->get_Node((dyn_cast<SelectInst>(BI))->getTrueValue());
        if(node->is_Load_Address_Generator())
          node = node->get_Related_Node();  
        arc1 = myDFG->get_Arc(node, node1);
        arc1->SetOperandOrder(1);
        node = myDFG->get_Node((dyn_cast<SelectInst>(BI))->getFalseValue());
        if(node->is_Load_Address_Generator())
          node = node->get_Related_Node();
        arc1 = myDFG->get_Arc(node, node1);
        arc1->SetOperandOrder(0);
      }

      
      if(DEBUG)
        errs() << " Before getelementPtr\n"; 
      if(BI->getOpcode() == Instruction::GetElementPtr)
      {
	if(DEBUG) errs() << " Inside getelementptr\n";
        ArrID++;
        std::ostringstream arrid;
        arrid << ArrID;

	// Check if GEP inst contains a struct
	bool isStructInst = false;
	std::string struct_name;
	Type* op0_type = dyn_cast<Value>(BI->getOperand(0))->getType();
	if(op0_type->isPointerTy()){
	  Type* element_type = op0_type->getPointerElementType();
	  while(element_type->isPointerTy())
	    element_type = element_type->getPointerElementType();  // Iterate until element type found
	  if(element_type->isStructTy()){
	    isStructInst = true;
	    struct_name = element_type->getStructName().str();
	    op0_type = element_type;
	  }
	}
	else if(op0_type->isStructTy()){
	  isStructInst = true;
	  struct_name = op0_type->getStructName().str();
	}

	if(isStructInst){
	  if(DEBUG) errs() << "Getelementptr with struct name: " << struct_name << " - bitwidth: " << op0_type->getPrimitiveSizeInBits()/8 << "\n";
	  if(map_struct_size.find(struct_name) == map_struct_size.end())
	    get_Datatype(op0_type, op0_type->getPrimitiveSizeInBits()/8);
	  
	  if(DEBUG) errs() << "  Struct size: " << map_struct_size.find(struct_name)->second << "\n";
	}
	
        for(unsigned int i=0; i< BI->getNumOperands(); ++i)
        {
          //Fix Predecessor Nodes With idxprom
          node1 = myDFG->get_Node(BI);
          std::string operandname = BI->getOperand(i)->getName().str();
          Datatype dt = node1->getDatatype();
	  if(DEBUG) errs() << "  Operand " << i << " - " << BI->getOperand(i) << " - name: " << operandname << "\n";
          //Add Node If Unnamed Private
          if(!operandname.empty())
          {
            std::string str = BI->getOperand(i)->getName().str();
	    node2 = myDFG->get_Node(str);
	    if(node2 == NULL){
	      node2 = new NODE(constant, 1, NodeID++, str, NULL);
	      node2->setDatatype(dt);
	      myDFG->insert_Node(node2);
	      myDFG->make_Arc(node2, node1, EdgeID++, 0, TrueDep, 0);
	    }
	    if(DEBUG) errs() << "  Uncertain behavior: adding node2: " << str << " - node1: " << node1->get_Name() << "\n";
          }
          else
          {
            node2 = myDFG->get_Node(BI->getOperand(i));
          }
	  if(DEBUG) errs() << "  node2: " << node2->get_Name() << "\n";
	  
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

	  if(isStructInst){
	  if(i == 0){
	    map_operand_node[0] = node2;
	  }
	  else if(i == 1){
	    if(node2->get_Name().compare("ConstInt0") != 0){
	      if(DEBUG) errs() << "    Operand1 is non zero -> accessing struct array element\n";
	      if(node2->get_Name().find("ConstInt") != std::string::npos){  // Exclude global variable
		std::string node2_name = node2->get_Name();
		char* temp_str = new char[node2_name.length()+1];
		strcpy(temp_str,node2_name.c_str());
		unsigned int constVal = 0;  // extra constant from node2 name
		sscanf(temp_str, "%*[^-0123456789]%d",&constVal);
		if(DEBUG) errs() << "    Operand0 is a constant " << constVal << "\n";

		constVal *= map_struct_size[struct_name];
		std::string new_name = "ConstInt" + std::to_string(constVal);
		NODE* temp_node = myDFG->get_Node(new_name);
		if(temp_node == NULL){
		  temp_node = new NODE(constant, 1, NodeID++, new_name, NULL);
		  temp_node->setDatatype(int32);
		  myDFG->insert_Node(temp_node);
		}
		if(DEBUG) errs() << "    New node2: " << temp_node->get_Name() << "\n";
		myDFG->Remove_Arc(myDFG->get_Arc(node2, node1));
		myDFG->make_Arc(temp_node, node1, EdgeID++, 0, TrueDep, 1);
		
		map_operand_node[1] = node2;
	      }
	      else {
		node3 = myDFG->get_Node("ConstInt" + std::to_string(map_struct_size[struct_name]));
		node4 = NULL;
		if(node3 == NULL){
		  node3 = new NODE(constant, 1, NodeID++, "ConstInt" + std::to_string(map_struct_size[struct_name]), NULL);  // holds size of the target struct
		  myDFG->insert_Node(node3);
		} else {
		  std::vector<NODE*> node3_out = node3->Get_Next_Nodes();
		  for(int out_it=0; out_it < node3_out.size(); out_it++)
		    if(node3_out[out_it]->get_Instruction() == mult){
		      std::vector<NODE*> mult_prev = node3_out[out_it]->Get_Prev_Nodes();
		      if(mult_prev[0] == node2){
			node4 = node3_out[out_it];
			break;
		      } else if(mult_prev[1] == node2){
			node4 = node3_out[out_it];
			break;
		      }
		    }
		}
		if(node4 == NULL){
		  node4 = new NODE(mult, 1, NodeID++, std::to_string(NodeID-1), NULL);
		  node4->setDatatype(int32);
		  myDFG->insert_Node(node4);
		}
		if(DEBUG) errs() << "    node3: " << node3->get_Name() << " - node4: " << node4->get_Name() << "\n";
	        
		myDFG->Remove_Arc(myDFG->get_Arc(node2, node1));

		myDFG->make_Arc(node2, node4, EdgeID++, 0, TrueDep, 0);
		myDFG->make_Arc(node3, node4, EdgeID++, 0, TrueDep, 1);
		myDFG->make_Arc(node4, node1, EdgeID++, 0, TrueDep, 1);
		
		map_operand_node[1] = node4;
	      }
	    }
	    else {
	      myDFG->Remove_Arc(myDFG->get_Arc(node2, node1));
	      map_operand_node[1] = NULL;
	    }
	  }
	  
	  else if(i == 2) {  // Operand2 is used to access struct member -> must be constant
	    unsigned offset = 0;
	    if(node2->get_Instruction() != constant){
	      errs() << "ERROR: Accessing struct element should be constant! Exitting.\n";
	      exit(1);
	    }
	    std::string node2_name = node2->get_Name();
	    
	    errs() << "Operand 2 node 2 name: " << node2_name << "\n";
	    
	    char* temp_str = new char[node2_name.length()+1];
	    strcpy(temp_str,node2_name.c_str());
	    unsigned int constVal = 0;
	    sscanf(temp_str, "%*[^-0123456789]%d",&constVal);
	    
	    for(int struct_member_i = 0; struct_member_i < constVal; struct_member_i++)
	      offset += map_struct_member_size[struct_name][struct_member_i];
	    
	    std::string new_name = "ConstInt" + std::to_string(offset);
	    NODE* temp_node = myDFG->get_Node(new_name);
	    if(temp_node == NULL){
	      temp_node = new NODE(constant, 1, NodeID++, new_name, NULL);
	      temp_node->setDatatype(int32);
	      myDFG->insert_Node(temp_node);
	    }
	    if(DEBUG) errs() << "    New node2: " << temp_node->get_Name() << "\n";

	    NODE* op1_node = map_operand_node[1];
	    if(!op1_node){
	      myDFG->Remove_Arc(myDFG->get_Arc(node2, node1));
	      myDFG->make_Arc(temp_node, node1, EdgeID++, 0, TrueDep, 1);
	      map_operand_node[2] = temp_node;
	    }
	    else {
	      myDFG->Remove_Arc(myDFG->get_Arc(op1_node, node1));
	      myDFG->Remove_Arc(myDFG->get_Arc(node2, node1));

	      node3 = NULL;
	      std::vector<NODE*> node2_out = temp_node->Get_Next_Nodes();
	      for(int out_it=0; out_it < node2_out.size(); out_it++)
		if(node2_out[out_it]->get_Instruction() == add){
		  std::vector<NODE*> add_prev = node2_out[out_it]->Get_Prev_Nodes();
		  if(add_prev[0] == op1_node){
		    node3 = node2_out[out_it];
		    break;
		  } else if(add_prev[1] == op1_node){
		    node3 = node2_out[out_it];
		    break;
		  }
		}
	      if(node3 == NULL){
		node3 = new NODE(add, 1, NodeID++, "", NULL);
		node3->setDatatype(int32);
		myDFG->insert_Node(node3);
	      }
	      if(DEBUG) errs() << "    node3: " << node3->get_Name() << " - op1_node: " << op1_node->get_Name() << "\n";
	      
	      myDFG->make_Arc(temp_node, node3, EdgeID++, 0, TrueDep, 0);  // operand order matters! op0 should be for const
	      myDFG->make_Arc(op1_node, node3, EdgeID++, 0, TrueDep, 1);
	      myDFG->make_Arc(node3, node1, EdgeID++, 0, TrueDep, 1);
	      map_operand_node[2] = node3;
	    }
	  }
	  else if(i == 3) {  // Operand2 is assumed to always be a constant for struct
	    int struct_index = 0;  // Access index of struct element
	    if(dyn_cast<llvm::ConstantInt>((BI)->getOperand(2))->getBitWidth() > 1)
	      struct_index = dyn_cast<llvm::ConstantInt>((BI)->getOperand(2))->getSExtValue();
	    else
	      struct_index = dyn_cast<llvm::ConstantInt>((BI)->getOperand(2))->getZExtValue();

	    
	    NODE* op2_node = map_operand_node[2];
	    NODE* op2_const;
	    if(op2_node->get_Instruction() != constant){
	      std::vector<NODE*> op2_prev = op2_node->Get_Prev_Nodes();
	      if(op2_prev[0]->get_Name()[0] == 'C') op2_const = op2_prev[0];
	      else op2_const = op2_prev[1]; // Assumes only 1 of 2 operand is a constant
	    } else op2_const = op2_node;
	    
	    std::string op2_name = op2_const->get_Name();
            char* temp_str = new char[op2_name.length()+1];
            strcpy(temp_str,op2_name.c_str());
            unsigned int constVal = 0;   // Memory offset at struct index
            sscanf(temp_str, "%*[^-0123456789]%d",&constVal);

	    Type* op2_type = op0_type->getStructElementType(struct_index);

	    if(op2_type->isArrayTy()) {
	      Datatype elementDT = get_Datatype(op2_type->getArrayElementType(), op2_type->getPrimitiveSizeInBits()/8);
	      unsigned element_size = 0;
	      if(elementDT == _struct){
		// Very odd situation: an array of struct inside a bigger struct -> inspect more
		auto op2_size = map_struct_size.find(op2_type->getArrayElementType()->getStructName().str());
	        element_size = op2_size->second;  // Could cause seg fault if struct not discovered
	      } else if(elementDT == _array) {
		errs() << "HELP: Array inside array, what should I do?\n";
		exit(1);
	      } else
		element_size = DT_Size[elementDT];
	      
	      if(node2->get_Instruction() == constant){
		std::string op3_name = node2->get_Name();
		char* temp_str = new char[op3_name.length()+1];
		strcpy(temp_str,op3_name.c_str());
		unsigned int offset = 0;
		sscanf(temp_str, "%*[^-0123456789]%d",&offset);

		offset *= element_size;
		offset += constVal;
		std::string new_name = "ConstInt" + std::to_string(offset);  // Since op2 and op3 are const, add them directly into 1 node
		op2_const->set_Name(new_name);
		NODE* temp_const = myDFG->get_Node(new_name);
		
		// Delete op3 node and edge (node2, node1) since op3 node const is added to op2 const
		myDFG->delete_Node(node2); // delete_Node should also remove arc
		map_operand_node[3] = temp_const;
		
	      } else {
		// Accesing array inside struct -> Similar behavior to outside struct, w/o idxprom since address should be static
		node3 = myDFG->get_Node("ConstInt" + std::to_string(element_size));
		node4 = NULL;
		if(node3 == NULL){
		  node3 = new NODE(constant, 1, NodeID++, "ConstInt" + std::to_string(element_size), NULL);
		  myDFG->insert_Node(node3);
		} else {
		  std::vector<NODE*> node3_out = node3->Get_Next_Nodes();
		  for(int out_it=0; out_it < node3_out.size(); out_it++)
		    if(node3_out[out_it]->get_Instruction() == mult){
		      std::vector<NODE*> mult_prev = node3_out[out_it]->Get_Prev_Nodes();
		      if(mult_prev[0] == node2){
			node4 = node3_out[out_it];
			break;
		      } else if(mult_prev[1] == node2){
			node4 = node3_out[out_it];
			break;
		      }
		    }
		}
		if(node4 == NULL){
		  node4 = new NODE(mult, 1, NodeID++, "", NULL);
		  node4->setDatatype(int32);
		  myDFG->insert_Node(node4);
		}
		if(DEBUG) errs() << "    node3: " << node3->get_Name() << " - node4: " << node4->get_Name() << "\n";

		NODE* node5 = NULL;
		std::vector<NODE*> op2_succ = op2_node->Get_Next_Nodes(), node4_succ = node4->Get_Next_Nodes();
		for(int op2_it = 0; op2_it < op2_succ.size(); op2_it++)
		  for(int node4_it = 0; node4_it < node4_succ.size(); node4_it++)
		    if(op2_succ[op2_it] == node4_succ[node4_it]) node5 = op2_succ[op2_it];
		if(node5 == NULL){
		  node5 = new NODE(add, 1, NodeID++, "", NULL);
		  node5->setDatatype(int32);
		  myDFG->insert_Node(node5);
		}
		
		myDFG->Remove_Arc(myDFG->get_Arc(node2, node1));
		myDFG->Remove_Arc(myDFG->get_Arc(op2_node, node1));
		
		myDFG->make_Arc(node2, node4, EdgeID++, 0, TrueDep, 1);
		myDFG->make_Arc(node3, node4, EdgeID++, 0, TrueDep, 0);
		
		myDFG->make_Arc(op2_node, node5, EdgeID++, 0, TrueDep, 0);
		myDFG->make_Arc(node4, node5, EdgeID++, 0, TrueDep, 1);
		myDFG->make_Arc(node5, node1, EdgeID++, 0, TrueDep, 1);

		map_operand_node[3] = node5;
	      }
	    } else if(op2_type->isStructTy()) {
	      // If op2 points to another struct, op3 should be constant since accessing struct member is const
	      // Add memory offset from op3 directly to op2
	      std::string op2_struct_name = op2_type->getStructName().str();
	      if(map_struct_size.find(op2_struct_name) == map_struct_size.end())
		get_Datatype(op2_type, op2_type->getPrimitiveSizeInBits()/8);  // Discover new struct
	      
	      std::string op3_name = node2->get_Name();
	      char* temp_str = new char[op3_name.length()+1];
	      strcpy(temp_str,op3_name.c_str());
	      unsigned int offset = 0;
	      sscanf(temp_str, "%*[^-0123456789]%d",&offset);

	      for(int j=0; j<offset; j++)
		offset += map_struct_member_size[op2_struct_name][j];
	      offset += constVal;

	      std::string new_name = "ConstInt" + std::to_string(offset);
	      op2_const->set_Name(new_name);
	      myDFG->delete_Node(node2);
	      map_operand_node[3] = op2_node;
	      
	    } else if(op2_type->isPointerTy()) {
	      errs() << "ERROR: Operand2 points to a pointer! What should I do in operand3?\n";
	      exit(1);
	    } else {
	      errs() << "ERROR: Operand2 points to primitive type! What should I do in operand3?\n";
	      exit(1);
	    }
	  }
	  else if(i == 4){  // Accessing struct of struct of struct or array of struct of struct!
	    int op2_index = 0;  // Access index of struct element
	    if(dyn_cast<llvm::ConstantInt>((BI)->getOperand(2))->getBitWidth() > 1)
	      op2_index = dyn_cast<llvm::ConstantInt>((BI)->getOperand(2))->getSExtValue();
	    else
	      op2_index = dyn_cast<llvm::ConstantInt>((BI)->getOperand(2))->getZExtValue();

	    int op3_index = 0;
	    if(dyn_cast<llvm::ConstantInt>((BI)->getOperand(3))->getBitWidth() > 1)
              op3_index = dyn_cast<llvm::ConstantInt>((BI)->getOperand(3))->getSExtValue();
            else
              op3_index = dyn_cast<llvm::ConstantInt>((BI)->getOperand(3))->getZExtValue();

	    
	    NODE* op3_node = map_operand_node[3];
	    NODE* op3_const;
	    if(op3_node->get_Instruction() != constant){
              std::vector<NODE*> op3_prev = op3_node->Get_Prev_Nodes();
              if(op3_prev[0]->get_Name()[0] == 'C') op3_const = op3_prev[0];
              else op3_const = op3_prev[1]; // Assumes only 1 of 2 operand is a constant
            } else op3_const = op3_node;
	    
	    std::string op3_name = op3_const->get_Name();
            char* temp_str = new char[op3_name.length()+1];
            strcpy(temp_str,op3_name.c_str());
            unsigned int constVal = 0;   // Memory offset at struct index
            sscanf(temp_str, "%*[^-0123456789]%d",&constVal);

	    Type* op2_type = op0_type->getStructElementType(op2_index);
	    Type* op3_type = op2_type->getStructElementType(op3_index);
	    
	    if(op3_type->isArrayTy()) {
	      Datatype elementDT = get_Datatype(op3_type->getArrayElementType(), op3_type->getPrimitiveSizeInBits()/8);
	      unsigned element_size = 0;
	      if(elementDT == _struct){
		// Very odd situation: array of struct inside a bigger struct of a struct -> inspect more
		auto op3_size = map_struct_size.find(op3_type->getArrayElementType()->getStructName().str());
	        element_size = op3_size->second;  // May cause seg fault if struct is not discovered yet
	      } else if(elementDT == _array) {
		errs() << "HELP: Array inside array, what should I do?\n";
		exit(1);
	      } else
		element_size = DT_Size[elementDT];

	      if(node2->get_Instruction() == constant){
		std::string op4_name = node2->get_Name();
		char* temp_str = new char[op4_name.length()+1];
		strcpy(temp_str,op4_name.c_str());
		unsigned int offset = 0;
		sscanf(temp_str, "%*[^-0123456789]%d",&offset);

		offset *= element_size;
		offset += constVal;
		std::string new_name = "ConstInt" + std::to_string(offset);
		op3_const->set_Name(new_name);

		myDFG->delete_Node(node2); // delete_Node should also remove arc
		
	      } else {
		// Accesing array inside struct -> Similar behavior to outside struct, w/o idxprom since address should be static
		node3 = new NODE(constant, 1, NodeID++, "ConstInt" + std::to_string(element_size), NULL);
		node4 = new NODE(mult, 1, NodeID++, std::to_string(NodeID-1), NULL);
		node4->setDatatype(int32);
		NODE* node5 = new NODE(add, 1, NodeID++, std::to_string(NodeID-1), NULL);
		node5->setDatatype(int32);
		
		myDFG->insert_Node(node3);
		myDFG->insert_Node(node4);
		myDFG->insert_Node(node5);
		
		myDFG->Remove_Arc(myDFG->get_Arc(node2, node1));
		myDFG->Remove_Arc(myDFG->get_Arc(op3_node, node1));
		
		myDFG->make_Arc(node2, node4, EdgeID++, 0, TrueDep, 1);
		myDFG->make_Arc(node3, node4, EdgeID++, 0, TrueDep, 0);
		
		myDFG->make_Arc(op3_node, node5, EdgeID++, 0, TrueDep, 0);
		myDFG->make_Arc(node4, node5, EdgeID++, 0, TrueDep, 1);
		myDFG->make_Arc(node5, node1, EdgeID++, 0, TrueDep, 1);
	      }
	    } else if(op3_type->isStructTy()) {
	      // If op3 points to another struct, op4 should be constant since accessing struct member is const
	      // Add memory offset from op4 directly to op3
	      std::string op3_struct_name = op3_type->getStructName().str();
	      if(map_struct_size.find(op3_struct_name) == map_struct_size.end())
		get_Datatype(op3_type, op3_type->getPrimitiveSizeInBits()/8);  // Discover new struct
	      
	      std::string op4_name = node2->get_Name();
	      char* temp_str = new char[op4_name.length()+1];
	      strcpy(temp_str,op4_name.c_str());
	      unsigned int offset = 0;
	      sscanf(temp_str, "%*[^-0123456789]%d",&offset);

	      for(int j=0; j<offset; j++)
		offset += map_struct_member_size[op3_struct_name][j];
	      offset += constVal;

	      std::string new_name = "ConstInt" + std::to_string(offset);
	      op3_const->set_Name(new_name);
	      myDFG->delete_Node(node2);
	      
	    } else if(op3_type->isPointerTy()) {
	      errs() << "ERROR: Operand2 points to a pointer! What should I do in operand3?\n";
	      exit(1);
	    } else {
	      errs() << "ERROR: Operand2 points to primitive type! What should I do in operand3?\n";
	      exit(1);
	    }
	  }
	  else {
	    errs() << "ERROR: Currently only support accessing struct up to 3 indices -> Parameterize me!\n";
	    exit(1);
	  }
	  } // End struct GEP


	  else {
          //FIX Insertion Of idxprom, if not there
          if((BI->getNumOperands()==3 && i==2) || (BI->getNumOperands()==2 && i==1))
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

	    if(DEBUG) errs() << "Before idxprom insertion\n";
	    if(idxprom_nodeID_pred_map.find(node->get_ID()) != idxprom_nodeID_pred_map.end())
	    {
	      if(DEBUG) errs() << "   idxprom with node created\n";
	      arc1 = myDFG->get_Arc(node, node1);
	      myDFG->Remove_Arc(arc1);
	      node3 = myDFG->get_Node(idxprom_nodeID_pred_map[node->get_ID()]);
	      myDFG->make_Arc(node, node3, EdgeID++, 0, TrueDep, 0);
	      myDFG->make_Arc(node3, node1, EdgeID++, 0, TrueDep, 1);
	    }
	    else if(node->get_Instruction() != constant && node->get_Name()[0] != 'g')
	    {
	      if(DEBUG) errs() << "   idxprom with new nodes\n";
	      node3 = new NODE(mult, 1, NodeID++, "idxprom"+arrid.str() , NULL);
	      unsigned alignment = idxprom_nodeID_load_alignment_map[BI];
	      node4 = new NODE(constant, 1, NodeID++, "ConstInt"+std::to_string(alignment), NULL);
	      Datatype dt = int32; 
	      node3->setDatatype(dt); 
	      node4->setDatatype(dt);
	      myDFG->insert_Node(node3);
	      myDFG->insert_Node(node4);
	      errs() << "      Node: " << node->get_Name() << " - Node1: " << node1->get_Name() << "\n";
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
      }
      
      //Fix Successor Operand Order`
      if((BI->getOpcode() == Instruction::Load) || (BI->getOpcode() == Instruction::Store))
      {
	if(DEBUG) errs() << " Inside Load or Store\n";
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

      /*std::vector<NODE*> condNodes;
        if(BI->getOpcode() == Instruction::ICmp || BI->getOpcode() == Instruction::FCmp)
        {
        for(unsigned int i=0; i<bbs.size(); ++i) {
        std::vector<ARC*> outGoingArcs = myDFG->getSetOfArcs();
        errs() << "inside CMP" << "\n";
        node1 = myDFG->get_Node(BI);
        errs() << node1->get_Name() << "\n";
      //errs() << "bbs.size: " << bbs.size() << "\n";  
      }
      //node1 = myDFG->get_Node(BI);
      //for(unsigned int ii = 0; ii < outGoingArcs.size(); ii++)
      //if(outGoingArcs[ii]->get_From_Node() == node1)
      //   condNodes.push_back(outGoingArcs[ii]->get_To_Node());
      }*/

      //for (int i = 0; i < bbs.size(); i++)
      //{
      // for (BasicBlock::iterator it = bbs[i]->begin(); it != bbs[i]->end(); ++it)
      //{
      //if(BI->getOpcode == Instruction::ICmp || BI->getOpcode == Instruction::FCmp)
      //{     
      //      node1 = myDFG->get_Node(it);
      //      errs() << "cmp:" << node1->get_Name() << "\n"; 
      //}
      //Instruction *ii = &*BI;
      //errs() << "Inst: " << *ii << "\n";
      //}
      //}
      if(DEBUG)
        errs() << " before phi\n"; 
      if(BI->getOpcode() == Instruction::PHI)
      {

        if(DEBUG)
          errs() << " inside phi: " << *BI << "\n";
        node1 = myDFG->get_Node(BI);
	
        //If phi node is at top of the loop - it's not due to if-then-else
        //so avoid acting on phi node to supply predicate
        //This is because phi and selects have separate opcodes in backend
        //inputs selected through Phi is statically determined.

        // the following if statement is for 
        if(BI->getParent() != loopHeader)
        {
          //errs() << "top brinst: " << myDFG->get_Node(brInst)->get_Name() << "\n";

          if(DEBUG) {
	    errs() << "  Phi not in loopheader\n";
            errs() << "  Instr: " << *(BI) << "\n" ;
            errs() << "  node: " << myDFG->get_Node(BI)->get_ID() << "\n";     
          }

          std::vector<BasicBlock*> bbList;
          std::vector<Instruction*> operandList;

          //Populate lists with the information from phi node
          for(unsigned int ii=0; ii < dyn_cast<PHINode>(BI)->getNumIncomingValues(); ii++)
          {
            bbList.push_back(dyn_cast<PHINode>(BI)->getIncomingBlock(ii));
            operandList.push_back(dyn_cast<Instruction>(BI->getOperand(ii)));
          } 

          if(DEBUG)
          {
            for(unsigned int ii=0; ii < (int) operandList.size(); ii++)
              errs() << "  op nodes: " << myDFG->get_Node(operandList[ii])->get_ID() << "\n";

            errs() << "  BBLIST:\n\n"; 
            for(unsigned int ii=0; ii < (int) bbList.size(); ii++)
            {
              errs() << "   ii: " << ii << "\n"; 
              for(BasicBlock::iterator BBBI = bbList[ii]->begin(); BBBI!= bbList[ii]->end(); ++BBBI)
                errs() << "    " << *BBBI << "\n";
              errs() << "\n"; 
            }
            //exit(1);
          }

          if(DEBUG) errs() << "  passed operands" << "\n";

          std::vector<NODE*> succofPhi;
          std::vector<Instruction*> condNodes;
          std::vector<int> succofPhiDistance;
          std::vector<DataDepType> succofPhiDepType;
          std::vector<int> succofPhiOpOrder;


          /*for(unsigned int ii=0; ii< BI->getNumOperands(); ++ii)
            {
            node2 = myDFG->get_Node(BI->getOperand(ii));
            if(DEBUG) {
            errs() << "we are removing the arc for: " << node1->get_ID() << "\t" << node2->get_ID() << "\n";
            }
            arc1 = myDFG->get_Arc(node2,node1);
            if(arc1 != NULL)
            myDFG->Remove_Arc(arc1);
            }

            std::vector<ARC*> outGoingArcs = myDFG->getSetOfArcs();
            if(DEBUG) errs() << "total arcs: " << (int) outGoingArcs.size() << "\n";
            for(unsigned int ii = 0; ii < outGoingArcs.size(); ii++)
            {
            if(outGoingArcs[ii]->get_From_Node() == node1)
            {
            if(DEBUG) errs() << "outgoing nodes: " << outGoingArcs[ii]->get_To_Node()->get_ID() << "\n";
            succofPhi.push_back(outGoingArcs[ii]->get_To_Node());
            succofPhiDistance.push_back(outGoingArcs[ii]->Get_Inter_Iteration_Distance());
            succofPhiDepType.push_back(outGoingArcs[ii]->Get_Dependency_Type());
            succofPhiOpOrder.push_back(outGoingArcs[ii]->GetOperandOrder());
            }
            }
           */
          if(DEBUG)
            for(unsigned int ii=0; ii < (int)succofPhi.size(); ii++)
            {
              errs() << "  succ nodes: " << succofPhi[ii]->get_ID() << "\n";
            }


          if(bbList.size() > 1)
          {
            if(bbList.size() > 2) {errs() << "Fatal: bbList has more than two basicblocks\n"; exit(1);}

            BasicBlock* succ1 = bbList[0]->getSingleSuccessor(); 
            BasicBlock* succ2 = bbList[1]->getSingleSuccessor();
            BasicBlock *commonParent;
            BasicBlock* current = BI->getParent(); 
            BranchInst * tempBr;

            if((succ1 == NULL) && (succ2 == NULL))
            {
              if(DEBUG) errs() << "   in new function if1 : " << *BI << "\n";
              // this function is to update brInst and condInst
              //tempBr = getConditionalBranch(bbList[0]);
              if(DEBUG) errs() << "   both succ are null" << "\n";  
              commonParent = SearchCommonParentForBranching(bbList[0],bbList[1],bbs);
            }
            else if((succ1 != NULL) && (succ2 == NULL))
            {
              if(DEBUG) errs() << "   in new function if2 : " << *BI << "\n";
              // this function is to update brInst and condInst
              if(current != loopLatch)
              commonParent = SearchCommonParentForBranching(bbList[0],bbList[1],bbs);
              else
              commonParent = SearchCommonParentForBranching(bbList[0],current,bbs);
              //tempBr = getConditionalBranch(bbList[1]); 
              //if(DEBUG) errs() << "br: " << tempBr << "\n"; 
            }
            else
            {
              if(DEBUG) errs() << "   in new function else : " << *BI << "\n";
              if((bbList[0]->getSinglePredecessor() == bbList[1]) ||
                  (bbList[1]->getSinglePredecessor() == bbList[0]))
                commonParent = SearchCommonParentForBranching(bbList[0],bbList[1],bbs);
            }

            for(unsigned int ii=0; ii< BI->getNumOperands(); ++ii)
            {
              node2 = myDFG->get_Node(BI->getOperand(ii));
              if(DEBUG) {
                errs() << "  we are removing the arc for: " << node1->get_ID() << "\t" << node2->get_ID() << "\n";
              }
              arc1 = myDFG->get_Arc(node2,node1);
              if(arc1 != NULL)
                myDFG->Remove_Arc(arc1);
            }

            std::vector<ARC*> outGoingArcs = myDFG->getSetOfArcs();
            if(DEBUG) errs() << "  total arcs: " << (int) outGoingArcs.size() << "\n";
            for(unsigned int ii = 0; ii < outGoingArcs.size(); ii++)
            {
              if(outGoingArcs[ii]->get_From_Node() == node1)
              {
                if(DEBUG) errs() << "   outgoing nodes: " << outGoingArcs[ii]->get_To_Node()->get_ID() << "\n";
                succofPhi.push_back(outGoingArcs[ii]->get_To_Node());
                succofPhiDistance.push_back(outGoingArcs[ii]->Get_Inter_Iteration_Distance());
                succofPhiDepType.push_back(outGoingArcs[ii]->Get_Dependency_Type());
                succofPhiOpOrder.push_back(outGoingArcs[ii]->GetOperandOrder());
              }
            }
            myDFG->delete_Node(node1);
            if(dyn_cast<PHINode>(BI)->getNumIncomingValues() > 2)
              node3 = new NODE(cond_select, 1, NodeID++, brInst->getName().str(), brInst);
            else
              node3 = new NODE(cond_select, 1, NodeID++, brInst->getName().str(), BI);
            node3->setDatatype(node2->getDatatype()); 
            myDFG->insert_Node(node3);
            if(DEBUG) errs() << "  New node no: " << node3->get_ID() << "\n";
            // update operands of the phi-->select
            for(int ii=0; ii<(int) operandList.size(); ii++)
            {
              // This is to support phi nodes that have one or both operands
              // as Constants.
              if(ii==0)
              {
                if(operandList[ii] != NULL) 
                {
                  if(DEBUG) errs() << "   node 2 in operand list: " << myDFG->get_Node(operandList[ii])->get_ID() << "\tInstruction: " << myDFG->get_Node(operandList[ii])->get_Instruction() << "\n";
                  node2 = myDFG->get_Node(operandList[ii]);
                }
                else
                {
                  if(DEBUG) errs() << "   node 2 NOT in operand list: " << myDFG->get_Node(BI->getOperand(ii))->get_ID() << "\n";
                  node2 = myDFG->get_Node(BI->getOperand(ii)); 
                }

                if(node2->is_Load_Address_Generator()) node2 = node2->get_Related_Node();
              }
              else
              {         
                if(operandList[ii] != NULL)
                  node4 = myDFG->get_Node(operandList[ii]);
                else
                  node4 = myDFG->get_Node(BI->getOperand(ii));

                if(node4->is_Load_Address_Generator()) node4 = node4->get_Related_Node();

              }

              //if(operandList[ii]->getOpcode() == Instruction::Select)
              //{
              //    myDFG->make_Arc(node2, node3, EdgeID++, 0, TrueDep, ii);
              // }
              //else
              // {
              // if(ii == 0)
              //     myDFG->make_Arc(node2, node3, EdgeID++, 0, TrueDep, 1);
              // else
              //     myDFG->make_Arc(node2, node3, EdgeID++, 0, TrueDep, 0);
              // }
            }

            // Mahesh: Make arcs. If one of the inputs is a select node, then the operand order changes.
            if(node2->get_Instruction() == cond_select || node4->get_Instruction() == cond_select)
            {
              if(node2->get_Instruction() == cond_select)
              {
                myDFG->make_Arc(node2, node3, EdgeID++, 0, TrueDep, 0);
                myDFG->make_Arc(node4, node3, EdgeID++, 0, TrueDep, 1);
              }
              else
              {
                myDFG->make_Arc(node4, node3, EdgeID++, 0, TrueDep, 0);
                myDFG->make_Arc(node2, node3, EdgeID++, 0, TrueDep, 1);
              }
            }
            else
            {
              myDFG->make_Arc(node2, node3, EdgeID++, 0, TrueDep, 1);
              myDFG->make_Arc(node4, node3, EdgeID++, 0, TrueDep, 0);
            }    

            //BasicBlock *commonParent = 
            // update condInst
            if(DEBUG) errs() << "  passed making arc condinst: " << condInst << "\n";              
            node2 = myDFG->get_Node(condInst);
            if(DEBUG) errs() << "  passed get node\n";
            if(DEBUG) errs() << "  condinst node2: " << node2->get_ID() << "\n";
            node3->setDatatype(node2->getDatatype());
            if(DEBUG) errs() << "  passed getDatatype\n";
            myDFG->make_Arc(node2, node3, EdgeID++, 0, PredDep, 2); 
            if(DEBUG) errs() << "  passed making pred  arc\n";

            // update outgoing arcs
            for(unsigned int kk = 0; kk < succofPhi.size(); kk++)
            {
              myDFG->make_Arc(node3, succofPhi[kk], EdgeID++, succofPhiDistance[kk],
                  succofPhiDepType[kk], succofPhiOpOrder[kk]);
            }

          }


          //myDFG->delete_Node(node1);

          /*while(bbList.size() > 1)
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
            int removebb; 
          //Check that all successors are already operated.
          //Otherwise skip such pair (ii,jj)
          BasicBlock *succII = bbList[ii]->getSingleSuccessor();
          BasicBlock *succJJ = bbList[jj]->getSingleSuccessor();
          if(succII == NULL || succJJ == NULL)
          {
          if(DEBUG) {
          if(succII == NULL ) errs() << "ii: " << ii << " II is null\n";
          if(succJJ == NULL ) errs() << "jj: " << jj << " JJ is null\n";
          }
          std::vector<BasicBlock*>::iterator it;
          for (succ_iterator SI = succ_begin(bbList[ii]), SE = succ_end(bbList[ii]); SI != SE; ++SI)
          {
          if(DEBUG) errs() << "we are inside ii\n";
          it = find(bbList.begin(), bbList.end(), (*SI));
          if (it != bbList.end()) {
          if(DEBUG) errs() << "we are inside ii skip\n";
          skip = true;
          removebb = ii;
          break;
          }
          }

          for (succ_iterator SI = succ_begin(bbList[jj]), SE = succ_end(bbList[jj]); SI != SE; ++SI)
          {
          if(DEBUG) errs() << "we are inside jj\n";
          it = find(bbList.begin(), bbList.end(), (*SI));
          if (it != bbList.end()) {
          if(DEBUG) errs() << "we are inside jj skip\n";
          skip = true;
          removebb = jj; 
          break;
          }
          }

          if((bbList[ii]->getSinglePredecessor() == bbList[jj]) ||
          (bbList[jj]->getSinglePredecessor() == bbList[ii]))
          skip = false;
          }

          if(skip) { errs() << "removing removebb for node22 : " << removebb << "\n";bbList.erase(bbList.begin()+removebb); continue; }
          BasicBlock *commonParent = SearchCommonParentForBranching(bbList[ii],bbList[jj],bbs); 
          if(commonParent == NULL) 
          { 
          if(DEBUG)
          errs() << "No common parents found\n";
          continue; 
          }

          if(DEBUG)
          errs() << "Common parents found not NULL\n";
          bbList.erase(bbList.begin()+ii);
          bbList.insert(bbList.begin()+ii,commonParent);
          bbList.erase(bbList.begin()+jj);
          for(unsigned int iii=0; iii< BI->getNumOperands(); ++iii)
          {
          node2 = myDFG->get_Node(BI->getOperand(iii));
          if(DEBUG) {
            errs() << "we are removing the arc for: " << node1->get_ID() << "\t" << node2->get_ID() << "\n";
          }
          arc1 = myDFG->get_Arc(node2,node1);
          if(arc1 != NULL)
            myDFG->Remove_Arc(arc1);
        }

        std::vector<ARC*> outGoingArcs = myDFG->getSetOfArcs();
        if(DEBUG) errs() << "total arcs: " << (int) outGoingArcs.size() << "\n";
        for(unsigned int iii = 0; iii < outGoingArcs.size(); iii++)
        {
          if(outGoingArcs[iii]->get_From_Node() == node1)
          {
            if(DEBUG) errs() << "outgoing nodes: " << outGoingArcs[iii]->get_To_Node()->get_ID() << "\n";
            succofPhi.push_back(outGoingArcs[iii]->get_To_Node());
            succofPhiDistance.push_back(outGoingArcs[iii]->Get_Inter_Iteration_Distance());
            succofPhiDepType.push_back(outGoingArcs[iii]->Get_Dependency_Type());
            succofPhiOpOrder.push_back(outGoingArcs[iii]->GetOperandOrder());
          }
        }
        myDFG->delete_Node(node1);
        if(dyn_cast<PHINode>(BI)->getNumIncomingValues() > 2)
          node3 = new NODE(cond_select, 1, NodeID++, brInst->getName().str(), brInst);
        else
          node3 = new NODE(cond_select, 1, NodeID++, brInst->getName().str(), BI);
        node3->setDatatype(node2->getDatatype()); 
        myDFG->insert_Node(node3);
        if(DEBUG) errs() << "New node no: " << node3->get_ID() << "\n";
        //errs() << "operandlst: " << operandList[ii]->getOpcodeName() << "\n";
        //if(operandList[ii] == NULL) 
        //  {
        //  errs() << BI->getOperand(0) << "\n";
        //  NODE *node7 = myDFG->get_Node(BI->getOperand(0));  
        //  errs() << "node7: " << node7->get_Name() << "\n"; 
        //  NODE *node8 = myDFG->get_Node(BI->getOperand(1));
        //  errs() << "node8: " << node8->get_Name() << "\n";

        //}
        // This is to support phi nodes that have one or both operands
        // as Constants. 
        if(operandList[ii] != NULL) 
          node2 = myDFG->get_Node(operandList[ii]);
        else
          node2 = myDFG->get_Node(BI->getOperand(ii)); 

        if(node2->is_Load_Address_Generator()) node2 = node2->get_Related_Node();
        // This is to support phi nodes that have one or both operands
        // as Constants.
        NODE* node4;  
        if(operandList[jj] != NULL)
          node4 = myDFG->get_Node(operandList[jj]); 
        else
          node4 = myDFG->get_Node(BI->getOperand(jj));
        if(node4->is_Load_Address_Generator()) node4 = node4->get_Related_Node();

        if(operandList[ii] != NULL || operandList[jj] != NULL)
        {
          if(operandList[ii]->getOpcode() == Instruction::Select || operandList[jj]->getOpcode() == Instruction::Select)
          {
            errs() << "we are here\n";
            errs() << "ins: " << *BI << "\n";
            errs() << "node 3: " << node3->get_ID() << "\n";
            errs() << "node2: " << node2->get_ID() << "\n";
            errs() << "node4: " << node4->get_ID() << "\n";
            if(operandList[ii]->getOpcode() == Instruction::Select)
            {
              myDFG->make_Arc(node2, node3, EdgeID++, 0, TrueDep, 0);
              myDFG->make_Arc(node4, node3, EdgeID++, 0, TrueDep, 1);
            }
            else if(operandList[jj]->getOpcode() == Instruction::Select)
            {
              myDFG->make_Arc(node4, node3, EdgeID++, 0, TrueDep, 0);
              myDFG->make_Arc(node2, node3, EdgeID++, 0, TrueDep, 1);
            }
          }
          else
          {
            myDFG->make_Arc(node4, node3, EdgeID++, 0, TrueDep, 0); 
            myDFG->make_Arc(node2, node3, EdgeID++, 0, TrueDep, 1); 
          }
        }
        else
        {
          myDFG->make_Arc(node4, node3, EdgeID++, 0, TrueDep, 0); 
          myDFG->make_Arc(node2, node3, EdgeID++, 0, TrueDep, 1); 
        }
        node2 = myDFG->get_Node(condInst);
        node3->setDatatype(node2->getDatatype());
        myDFG->make_Arc(node2, node3, EdgeID++, 0, PredDep, 2);
        if(dyn_cast<PHINode>(BI)->getNumIncomingValues() <= 2)
        {
          for(unsigned int kk = 0; kk < succofPhi.size(); kk++)
          {
            myDFG->make_Arc(node3, succofPhi[kk], EdgeID++, succofPhiDistance[kk],
                succofPhiDepType[kk], succofPhiOpOrder[kk]);
          }
        }
        //operandList.erase(operandList.begin()+ii);
        //operandList.insert(operandList.begin()+ii,brInst);
        //operandList.erase(operandList.begin()+jj);
        break;
        }
        //if(bbList.size() == 1) break;
        }
        }*/

        /* BasicBlock *tempBB1 = dyn_cast<PHINode>(BI)->getIncomingBlock(0)->getSinglePredecessor();
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

           myDFG->make_Arc(node2, node3, EdgeID++, 0, PredDep, 2);*/

        }
        else
        {
          // There are loop were the exit cond does not add a recurrent edge with distance 1.
          // If the recurrent to the phi is not added then the scheduiing algo runs into a infinite loop.
          // In that case this routine is a hack. It deletes the edge without recurrent and adds a recurrent edge. 
          ARC* arc2, *arc3; 
          if(DEBUG) 
            errs() << "  We are here! just check if the incoming nodes other than a const should be from next iterations. If not, make it into interiteration dependent\n";
          node1 = myDFG->get_Node(BI);
          if(DEBUG)
            errs() << "  Node: " << node1->get_Name() << " - type: " << node1->get_Instruction() << "\n";
          std::vector<NODE*> incoming;
          incoming = node1->Get_Prev_Nodes(); 
          if(DEBUG)   
            errs() << "  incoming size: " << (int)incoming.size() << "\n";
          for(int i=0; i<(int) incoming.size(); i++)
          {
            if(DEBUG)
              errs() << "   incoming node: " << incoming[i]->get_ID() << "\tname: " << incoming[i]->get_Name() << "\n"; 

            arc2 = myDFG->get_Arc(incoming[i], node1); 

            if(arc2->Get_Inter_Iteration_Distance() == 0 && incoming[i]->get_Instruction() != constant)
            {
              if(DEBUG)
                errs() << "    incoming node if: " << incoming[i]->get_ID() << "\n";
              int inter_dep=1;
              arc2->Set_Inter_Iteration_Distance(inter_dep); 
              //myDFG->Remove_Arc(arc2); 
              //myDFG->make_Arc(incoming[i], node1, EdgeID++, 1, TrueDep, 0);
            }
          }
        }
      }
      if(DEBUG)
        errs() << "DEBUG -- Exiting Update_dependency\n";

      return true;
    }

    /* Added by Vinh TA
       This function makes edges from a unique loop control node to liveOut nodes to be later used by mapping algos to enforce mapping constraints
     */
    NODE* Update_Loop_Control(std::vector<BasicBlock *> bbs, BasicBlock* ExitBlock, DFG* myDFG){
      if(DEBUG)	errs() << "Inside update_loop_control\n";
      
      
      BasicBlock::reverse_iterator branchinst = bbs[(int)bbs.size()-1]->rbegin();
      if(DEBUG) errs() << " loop_branch_inst: " << *branchinst << "\n";
      
      Instruction* op0_inst = cast<Instruction>(branchinst->getOperand(0));
      if(op0_inst == NULL) return NULL;
      
      if(DEBUG) errs() << " branch_inst_pred: " << *op0_inst << "\n";
      NODE* loopCtrlNode;
      if(myDFG->get_Node(op0_inst) != NULL){
	loopCtrlNode = myDFG->get_Node(op0_inst);
	if(DEBUG) errs() << "  branch_inst_pred node found: " << loopCtrlNode->get_Name() << "\n";
      }else{
	if(DEBUG) errs() << "  branch_inst_pred node not found!\n";
	return NULL;
      }

      std::vector<llvm::NODE*> LiveOutNodes;
      for(std::map<Instruction*, std::string>::iterator it = map_instn_defn_liveout_to_load.begin(); it != map_instn_defn_liveout_to_load.end(); ++it){
	NODE* temp = myDFG->get_Node(it->first);
	/*if(temp->get_Instruction() == cond_select){
	  if(loopCtrlNode->is_Connected_To(temp))
	    continue;
	}*/
	if(temp->get_Instruction() == ld_data) temp = temp->get_Related_Node();
	std::vector<NODE*> temp_prev = temp->Get_Prev_Nodes();
	
	if(temp_prev.size() > 0 && temp_prev[0]->get_Instruction() == cond_select) continue;
	if(temp_prev.size() > 1 && temp_prev[1]->get_Instruction() == cond_select) continue;
	if(temp_prev.size() > 2 && temp_prev[2]->get_Instruction() == cond_select) continue;
	
	LiveOutNodes.push_back(myDFG->get_Node(it->first));
	if(DEBUG) errs() << "  Added: " << myDFG->get_Node(it->first)->get_Name() << "\n";
      }
      
      std::vector<NODE*> allNodes = myDFG->getSetOfVertices();
      for(std::vector<NODE*>::iterator it = allNodes.begin(); it != allNodes.end(); ++it)
	if((*it)->is_Store_Data_Bus_Write()){
	  // Filter out predicated stores
	  //std::vector<NODE*> liveout_prev = (*it)->Get_Prev_Nodes();
	  //if(liveout_prev[0]->is_Store_Address_Generator() && liveout_prev[1]->get_Instruction() == cond_select) continue;
	  //if(liveout_prev[1]->is_Store_Address_Generator() && liveout_prev[0]->get_Instruction() == cond_select) continue;

	  LiveOutNodes.push_back(*it);
	  if(DEBUG) errs() << "  Added from stores: " << (*it)->get_Name() << "\n";
	}

      if(DEBUG){
	errs() << " Collected LiveOut nodes:\n";
	for(std::vector<NODE*>::iterator it = LiveOutNodes.begin(); it != LiveOutNodes.end(); ++it)
	  errs() << "  " << (*it)->get_Name() << " - #pred: "  << (*it)->get_Number_of_Pred() << "\n";
      }

      // Detect circular graph when adding LCE
      // Cannot be completed
      // TODO: update_liveout_variables was called before this function and it writes to liveout_node.txt and liveout_edge.txt files. This function has to find and replace that liveout node with a route node to break the circle, but replacing a node/edge requires modifying the written file -> have to go to the file and replace just the needed node with the route node.

      /*NODE* route1 = new NODE(add, 1, NodeID++, "route", NULL);
      //NODE* route2 = new NODE(add, 1, NodeID++, "route", NULL);
      NODE* node1 = LiveOutNodes[0];
      //NODE* node2 = LiveOutNodes[1];
      myDFG->insert_Node(route1);
      //myDFG->insert_Node(route2);
      std::vector<NODE*> out1 = node1->Get_Next_Nodes();
      //std::vector<NODE*> out2 = node2->Get_Next_Nodes();

      for(auto it = out1.begin(); it != out1.end(); ++it)
	if((*it)->get_Name()[0] == 'g'){
	  route1->setDatatype((*it)->getDatatype());
	  ARC* arc_old = myDFG->get_Arc(node1, *it);
	  myDFG->Remove_Arc(arc_old);
	  myDFG->make_Arc(node1, route1, EdgeID++, 0, TrueDep, 0);
	  myDFG->make_Arc(route1, *it, EdgeID++, 0, TrueDep, 0);
	  LiveOutNodes[0] = route1;
	}

      for(auto it = out2.begin(); it != out2.end(); ++it)
	if((*it)->get_Name()[0] == 'g'){
	  ARC* arc_old = myDFG->get_Arc(node2, *it);
	  myDFG->Remove_Arc(arc_old);
	  myDFG->make_Arc(node2, route2, EdgeID++, 0, TrueDep, 0);
	  myDFG->make_Arc(route2, *it, EdgeID++, 0, TrueDep, 0);
	  LiveOutNodes[1] = route2;
	}
      
      std::vector<NODE*> loopCtrl_pred = loopCtrlNode->Get_Prev_Nodes();
      for(std::vector<NODE*>::iterator it = LiveOutNodes.begin(); it != LiveOutNodes.end(); ++it){
	std::vector<NODE*> succ_nodes = (*it)->Get_Next_Nodes();
	bool circular = false;
	for(auto succ_it = succ_nodes.begin(); succ_it != succ_nodes.end(); ++succ_it){
	  if((*succ_it)->get_ID() == loopCtrlNode->get_ID()){
	    circular = true;
	    break;
	  }
	  else{
	    for(auto pred_it = loopCtrl_pred.begin(); pred_it != loopCtrl_pred.end(); ++pred_it){
	      if((*succ_it)->get_ID() == (*pred_it)->get_ID()){
		circular = true;
		break;
	      }
	    }
	  }
	}
	if(circular){
	  if(DEBUG) errs() << "  ! Circular graph detected for loop control edge !\n";
	  NODE* const0 = NULL;
	  for(auto node_it = allNodes.begin(); node_it != allNodes.end(); ++node_it){
	    if((*node_it)->get_Name().find("ConstInt0") != std::string::npos){
	      const0 = *node_it;
	      break;
	    }
	  }

	  if(const0 == NULL){
	    const0 = new NODE(constant, 1, NodeID++, "ConstInt0", NULL);
	    myDFG->insert_Node(const0);
	  }
	  
	  NODE* temp = new NODE(add, 1, NodeID++, "DE_CIRCULAR" , NULL);
	  myDFG->insert_Node(temp);
	  myDFG->make_Arc((*it), temp, EdgeID++, 0, TrueDep, 0);
	  myDFG->make_Arc(const0, temp, EdgeID++, 0, TrueDep, 1);
	  LiveOutNodes.erase(it);
	  LiveOutNodes.push_back(temp);
	}
	}*/

      for(std::vector<NODE*>::iterator it = LiveOutNodes.begin(); it != LiveOutNodes.end(); ++it){
	myDFG->make_Arc(loopCtrlNode, (*it), EdgeID++, 0, LoopControlDep, (*it)->get_Number_of_Pred());
      }
    
      return loopCtrlNode;
    }

    
    bool Update_Control_Dependencies(DFG* myDFG, std::vector<BasicBlock *> bbs, BasicBlock* loopLatch, BasicBlock* loopHeader)
      {

        NODE* node, *node1, *node2, *node3, *node4;
        ARC* arc1;
        DataDepType dep = TrueDep;


        /* Mahesh says: First get the condition variables/nodes in each bb. There are a few cases here:
           case 1: Loops without conditional statements will have one conditional node at most. 	
           case 2: Loops with One level if-else will have two conditional variables
           case 3: Loops with Nested and sibling if-else will have greater than 2 cond nodes.

           Now in case of case 1 and case 2 it is already solved in Update_Data_Dependencies. I am commenting
           out those statements there and adding them here for ease of understanding. 

           For case 3, we need first identify whether the conds are nested or siblings and then operate accordingly. 
           Siblings can be treated as multiple case 2's and operated. 

           Case 3 should have additional cond_select operations added and the dependencies updated correctly. 

         */

        //Step1: Get the number of conditions in the loop for each bb. This is called conditionals List or condList
        // for short

        std::vector<NODE*> condList;
        for(unsigned int i=0; i<bbs.size(); ++i) 
        {	
          for (BasicBlock::iterator BI = bbs[i]->begin(); BI != bbs[i]->end(); ++BI)
          {
            if(BI->getOpcode() == Instruction::ICmp || BI->getOpcode() == Instruction::FCmp)
            { 
              node1 = myDFG->get_Node(&(*BI));
              if(BI->getParent() == loopHeader) {
                node2 = myDFG->get_Node(&(*BI));
              }
              errs() << node1->get_Name() << "\n";
              condList.push_back(node1);
              std::vector<ARC*> outGoingArcs = myDFG->getSetOfArcs();
              //for(unsigned int ii = 0; ii < outGoingArcs.size(); ii++)
              // {
              // if(outGoingArcs[ii]->get_From_Node() == node1)
              //{
              //  errs() << "outnode:" << outGoingArcs[ii]->get_To_Node()->get_Name() << "\n"; 	
              // }
              // }
            }
          }
        }	
        errs() << condList.size() << "\n";	


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
          //vectorInstructions++;
        }
      }

      for (BasicBlock::iterator BI = ExitBlk->begin(); BI != ExitBlk->end(); ++BI)
      {
        if((BI->getOpcode() == Instruction::ExtractElement) ||
            (BI->getOpcode() == Instruction::InsertElement) ||
            (BI->getOpcode() == Instruction::ShuffleVector))
        {
          //vectorInstructions++;
        }
      }

      /*if(totalInstructions > 30)
        {
        std::cout << "3) Complex Loop with More Instructions. Total Instructions:  " << totalInstructions << "\n";
        retVal = false;
        }

        if(brInstructions > 2)
        {
        std::cout << "4) Loop with Complex Control Flow. Total branches are: " << brInstructions << "\n";
        retVal = false;
        }*/

      if(functionCalls > 0)
      {
        std::cout << "5) Loop with total " << functionCalls << " Function Calls.\n";
        retVal = false;
      }

      /*if(floatignPtInstructions > 0)
        {
        std::cout << "6) Loop with " << floatignPtInstructions << " floating point operations.\n";
        retVal = false;
        }*/

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

    std::vector<MDNode *> visited_loops;    

    virtual bool HasBeenVisited(MDNode * loopid)
    {
      for(int i=0; i < visited_loops.size() ; i++)
        if(visited_loops[i] == loopid)
          return true;

      return false;
    }

    virtual bool runOnLoop(Loop *L, LPPassManager &LPM)
    {
      dynamicTC = true; 
      //errs() << "loop id: " << L->getLoopID() << "\n"; 

      //if(HasBeenVisited(L->getLoopID()))
      //  return false;

      visited_loops.push_back(L->getLoopID()); 

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
      else {
	if(DEBUG)
		errs() << "preheader function name: " << Preheader->getParent()->getName() << "\n";
      }

      // If LoopSimplify form is not available, stay out of trouble.
      if (!L->hasDedicatedExits()) {
        return false;
      }


      errs() << "loop id: " << L->getLoopID() << "\n";

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

      //std::cout << "Loop has \tDepth=" << L->getLoopDepth() << "\tNumber of BBs=" << bbs.size() << " With one exiting branch\n";

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
	errs() << "runOnLoop::4041: instructions @ bbs[" << i << "]:\n";
        for (BasicBlock::iterator BI = bbs[i]->begin(); BI != bbs[i]->end(); ++BI)
        {
          errs() << "\tAdding " << *BI << "\n";
          if (!Add_Node(&(*BI), myDFG, L->getHeader()))
            return false;
        }
      }

      if(DEBUG){
	errs() << "Done adding node\nAdded structs:\n";
	for(auto struct_it = map_struct_size.begin(); struct_it != map_struct_size.end(); ++struct_it){
	  errs() << "Name: " << struct_it->first << " - size: " << struct_it->second << " - member sizes: {";
	  for(auto member_it = map_struct_member_size[struct_it->first].begin(); member_it != map_struct_member_size[struct_it->first].end(); ++member_it)
	    errs() << *member_it << ", ";
	  errs() << "}\n";
	}
      }

      if(DEBUG) 
      {
        errs() << "Add nodes complete\n";
        for (int i = 0; i < (int) bbs.size(); i++) 
          for (BasicBlock::iterator BI = bbs[i]->begin(); BI != bbs[i]->end(); ++BI)
          {
            if (myDFG->get_Node(&(*BI)) != NULL)
            {
              errs() << "Ins: " << *BI << "\n";
              errs() << "node: " << myDFG->get_Node(&(*BI))->get_ID() << "\n\n";
            }
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
      errs() << "Update livein complete\n";
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

      errs() << "completed update data dependency\n";
      //if(!Update_Control_Dependencies(myDFG, bbs, L->getLoopLatch(), L->getHeader() ))
      //	return false;

      liveInNodefile.close();
      liveInEdgefile.close();

      newPath = "./CGRAExec/L" + osLoopID.str() + "/liveout_node.txt";
      liveoutNodefile.open(newPath.c_str());
      newPath = "./CGRAExec/L" + osLoopID.str() + "/liveout_edge.txt";
      liveoutEdgefile.open(newPath.c_str());

      unsigned TripCount = calculateLoopTC(L, SE);
      if(DEBUG) errs() << "Calculated trip count: " << TripCount << "\n";
      if(TripCount != 0)
        dynamicTC = false;

      //Write TripCount in file
      std::ofstream lpTCfile;
      std::string tcfilename = "./CGRAExec/L" + osLoopID.str() + "/loop_iterations.txt";
      lpTCfile.open(tcfilename.c_str());
      lpTCfile << TripCount;
      lpTCfile.close();

      if(DEBUG) {
	errs() << "closed all files\n";
	errs() << "updated TC: " << TripCount << "\n";
	errs() << "dynamicTC: " << dynamicTC << "\n";
      }
      if(dynamicTC)
      {
        // TODO: If there are multiple exiting blocks, how to find the condition variable for dynamic TC loops
        // May be a hierarchy of select operations can help
        // In such case, need to exit even if a single condition is true, ensuring we exit from loop
	if(DEBUG)
	  errs() << "before getLoopExitBranch\n";
        getLoopExitBranch(ExitBlocks[0]);

        // Need to determine if trip-count is known at run-time
        // If yes, generate corresponding variables and calculate TC
        // If not, pad unsafe operations with selection instructions
        if(isTCknownAtLoopEntry(bbs))  // New version does not need to predicate result for dynamicTC
        {
          PHINode *indsvar = getInductionVariable(L,SE,DT);
          //calculateTCDynamically(indsvar, bbs, Preheader);
          dynamicTC = false;
        }
	dynamicTC = false;
      }  

      // NOTE: Comparison and testing for dynamicTCVal. Debug calculateTCDynamically.
      // What happens if TCVal was not able to execute? 

      errs() << "completed dynamic TC\n"; 
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
	  else errs() << "Node for " << (*BI) << " not found!\n";
        }
      } 

      errs() << "completed update liveout\n";

      liveoutNodefile.close();
      liveoutEdgefile.close();
      collectAllBranchInfo(bbs,L->getLoopLatch());
      updateStoresInConditionalBBs(L->getLoopLatch(),L->getHeader(),bbs, myDFG);

      // Added by: Vinh TA
      NODE* loopCtrl_node = Update_Loop_Control(bbs, ExitBlk, myDFG);
      std::ofstream LoopCtrlNodeFile;
      std::string filename = "./CGRAExec/L" + osLoopID.str() + "/Control_Node.txt";
      LoopCtrlNodeFile.open(filename.c_str());
      if(loopCtrl_node) LoopCtrlNodeFile << loopCtrl_node->get_Name() << "\n";
      else LoopCtrlNodeFile << "-1\n";
      LoopCtrlNodeFile.close();
      
      std::ostringstream osNodeID;
      osNodeID << NodeID;
      myDFG->Dump_Loop("./CGRAExec/L" + osLoopID.str() + "/loop" + osNodeID.str());
      directoryPath = "loop_" + osNodeID.str();
      myDFG->Dot_Print_DFG(directoryPath.c_str());
      directoryPath = directoryPath + "DFG.dot";
      newPath = "./CGRAExec/L" + osLoopID.str() + "/" + directoryPath;
      std::rename(directoryPath.c_str(), newPath.c_str());


      // InvokeCGRA pass added here.
      // * The osLoopID is unique for the loop. When DFG is created it is safe to extract the loop
      //   here with the respective loopID than call Invoke CGRA pass to do that.
      // * Calling another pass, we may lose the information of the loopID. 
      // * Recomputing the loopID can create problems when doing multiple pragmas.  
      //
      //

      M = L->getLoopPreheader()->getParent()->getParent();

      // Now that we know the removal is safe, remove the loop by changing the
      // branch from the preheader to go to the single exit block.
      //
      // Because we're deleting a large chunk of code at once, the sequence in which
      // we remove things is very important to avoid invalidation issues.

      // If we have an LPM updater, tell it about the loop being removed.
      //if (Updater)
      //  Updater->markLoopAsDeleted(*L);

      DominatorTree &DT1 = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
      ScalarEvolution &SE1 = getAnalysis<ScalarEvolutionWrapperPass>().getSE();
      LoopInfo &LI1 = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();

      // Tell ScalarEvolution that the loop is deleted. Do this before
      // deleting the loop so that ScalarEvolution can look at the loop
      // to determine what it needs to clean up.
      SE1.forgetLoop(L);

      // Connect the preheader directly to the newly created block.
      if(DEBUG) errs() << "Finding Terminator for Preheaders\n";
      auto *TI = Preheader->getTerminator();
      BasicBlock *newBB;
      newBB = newBB->Create(Preheader->getContext(), "", Preheader->getParent(), nullptr);

      newBB->moveAfter(Preheader);
      TI->replaceUsesOfWith(L->getHeader(), newBB);
      // insert branch to destinationBB
      BranchInst *newBranchInst;
      newBranchInst->Create(ExitBlk, newBB); //ExitBlock

      newBB->replaceSuccessorsPhiUsesWith(ExitBlk);

      // Rewrite phis in the exit block to get their inputs from
      // the newBB instead of the exiting block.
      if(DEBUG) errs() << "Attempting to rewrite PHI nodes in exit block\n";
      BasicBlock *ExitingBlock = ExitingBlocks[0];
      BasicBlock::iterator BI = ExitBlk->begin();
      while (PHINode *P = dyn_cast<PHINode>(BI)) {
        int j = P->getBasicBlockIndex(ExitingBlock);
        assert(j >= 0 && "Can't find exiting block in exit block's phi node!");
        P->setIncomingBlock(j, newBB);
        for (unsigned i = 1; i < ExitingBlocks.size(); ++i)
          P->removeIncomingValue(ExitingBlocks[i]);
        ++BI;
      }

      // Update the dominator tree and remove the instructions and blocks that will
      // be deleted from the reference counting scheme.
      if(DEBUG) errs() << "Updating dominator tree and removing instructions\n";
      SmallVector<DomTreeNode*, 8> ChildNodes;
      for (Loop::block_iterator LI1 = L->block_begin(), LE = L->block_end();
           LI1 != LE; ++LI1) {
        // Move all of the block's children to be children of the Preheader, which
        // allows us to remove the domtree entry for the block.
        ChildNodes.insert(ChildNodes.begin(), DT1[*LI1]->begin(), DT1[*LI1]->end());
        for (DomTreeNode *ChildNode : ChildNodes) {
          DT1.changeImmediateDominator(ChildNode, DT1[Preheader]);
        }

        BasicBlock * bb = *LI1;
        for(BasicBlock::iterator II = bb->begin(); II != bb->end(); ++II) {
           if(DEBUG) errs() << "Dropping references\n";
           Instruction * insII = &(*II);
           insII->dropAllReferences();
        }

        ChildNodes.clear();
        DT1.eraseNode(*LI1);

        // Remove the block from the reference counting scheme, so that we can
        // delete it freely later.
        (*LI1)->dropAllReferences();
      }

      // Erase the instructions and the blocks without having to worry
      // about ordering because we already dropped the references.
      // NOTE: This iteration is safe because erasing the block does not remove its
      // entry from the loop's block list.  We do that in the next section.
      if(DEBUG) errs() << "Erasing instructions without caring about references\n";
      for (Loop::block_iterator LI1 = L->block_begin(), LE = L->block_end();
           LI1 != LE; ++LI1)
        (*LI1)->eraseFromParent();

      // Finally, erase the blocks from loopinfo.
      // This has to happen late because
      // otherwise our loop iterators won't work.
      if(DEBUG) errs() << "Erasing loopinfo\n";
      SmallPtrSet<BasicBlock *, 8> blocks;
      blocks.insert(L->block_begin(), L->block_end());
      for (BasicBlock *BB : blocks)
        LI1.removeBlock(BB);

      // The last step is to update LoopInfo now that we've eliminated this loop.
      //LI1.erase(L); //TODO

      //Now we have deleted loop successfully
      //Let's insert function call in place of the loop
      Constant *hookFunc;
      hookFunc = M->getFunction("accelerateOnCGRA");
      if(DEBUG) errs() << "hookFunc is " << hookFunc << "\n";
      hook= cast<Function>(hookFunc);
      Value *LoopNumber = ConstantInt::get(Type::getInt32Ty(M->getContext()), TotalLoops);
      if(DEBUG) errs() << "Loop Number is " << *LoopNumber << "\n";
      Instruction *newInst = CallInst::Create(hook, LoopNumber, "");
      if(DEBUG) errs() << "newInst = " << newInst << "\n";
      if(DEBUG) errs() << "the BB = " << &newBB->getInstList() << "\n";
      newBB->getInstList().push_front(newInst);

      // Update total Number of Loops
      std::ofstream totalLoopsfile;
      std::string totalLoopsfilefilename = "./CGRAExec/total_loops.txt";
      totalLoopsfile.open(totalLoopsfilefilename.c_str());
      totalLoopsfile << TotalLoops;
      totalLoopsfile.close();

      if(DEBUG) errs() << "runOnLoop went well\n"; 

      delete myDFG;return true; //retVal;
    }
  };
}

char CondDDGGen::ID = 0;
static RegisterPass<CondDDGGen> X("CondDDGGen", "Loop Pass to Generate DDG for Accelerating Loops with conditionals On CGRA", false, true);
