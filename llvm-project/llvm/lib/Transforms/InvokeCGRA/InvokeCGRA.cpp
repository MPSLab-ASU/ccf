//===- InvokeCGRA.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the LLVM pass to accelerate loops over CGRA
// This transformation deletes loop with metadata CGRA and
// necessary function call to invoke CGRA is inserted
//
//  Author: Shail Dave
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "invoke-cgra"

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"

#include "llvm/Analysis/LoopPass.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"
#include <string>
#include <iostream>
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/DebugInfo.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/Analysis/GlobalsModRef.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"
#include "llvm/Transforms/Utils/LoopUtils.h"

#include "llvm/ADT/APInt.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/TargetSelect.h"
#include <algorithm>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

//#include "llvm/IR/TypeBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/DerivedTypes.h"

#include <fstream>
#include <sstream>

#define DEBUG 1

using namespace llvm;

unsigned int TotalLoops = 0;

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
  struct InvokeCGRA: public LoopPass
  {
    static char ID;
    Function *hook;
    InvokeCGRA() :
      LoopPass(ID)
    {

    }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
        getLoopAnalysisUsage(AU);
    }

    virtual bool runOnLoop(Loop *L, LPPassManager &LPM)
    {
      if(DEBUG) errs() << "runOnLoop Hit\n";
      if (HasCGRADisablePragma(L))
        return false;

      bool HasPragma = HasCGRAEnablePragma(L);
      if(!HasPragma) {
        if(DEBUG) errs() << "pragma CGRA not detected\n";
        return false;
      }

      SetLoopAlreadyIterated(L);
      if(DEBUG) errs() << "Loop has not been iterated yet\n";

      std::vector<BasicBlock *> bbs = L->getBlocks();
      static std::vector<std::string> dumped;

      LLVMContext Context;

      // We can only remove the loop if there is a preheader that we can
      // branch from after removing it.
      BasicBlock *Preheader = L->getLoopPreheader();

      if (!Preheader) {
        if(DEBUG) errs() << "No Preheaders\n";
        return false;
      }

      // If LoopSimplify form is not available, stay out of trouble.
      if (!L->hasDedicatedExits()) {
        if(DEBUG) errs() << "No Dedicated Exits\n";
        return false;
      }

      SmallVector<BasicBlock *, 4> ExitingBlocks;
      L->getExitingBlocks(ExitingBlocks);

      // We require that the loop only have a single exit block.  Otherwise, we'd
      // be in the situation of needing to be able to solve statically which exit
      // block will be branched to, or trying to preserve the branching logic in
      // a loop invariant manner.
      BasicBlock *ExitBlock = L->getUniqueExitBlock();
      if (!ExitBlock)
        return false;

      TotalLoops++;

      Module *M = L->getLoopPreheader()->getParent()->getParent();

      // Now that we know the removal is safe, remove the loop by changing the
      // branch from the preheader to go to the single exit block.
      //
      // Because we're deleting a large chunk of code at once, the sequence in which
      // we remove things is very important to avoid invalidation issues.

      // If we have an LPM updater, tell it about the loop being removed.
      //if (Updater)
      //  Updater->markLoopAsDeleted(*L);

      DominatorTree &DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
      ScalarEvolution &SE = getAnalysis<ScalarEvolutionWrapperPass>().getSE();
      LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();

      // Tell ScalarEvolution that the loop is deleted. Do this before
      // deleting the loop so that ScalarEvolution can look at the loop
      // to determine what it needs to clean up.
      SE.forgetLoop(L);

      // Connect the preheader directly to the newly created block.
      if(DEBUG) errs() << "Finding Terminator for Preheaders\n";
      auto *TI = Preheader->getTerminator();
      BasicBlock *newBB;
      newBB = newBB->Create(Preheader->getContext(), "", Preheader->getParent(), nullptr);

      newBB->moveAfter(Preheader);
      TI->replaceUsesOfWith(L->getHeader(), newBB);
      // insert branch to destinationBB
      BranchInst *newBranchInst;
      newBranchInst->Create(ExitBlock, newBB); //ExitBlock

      newBB->replaceSuccessorsPhiUsesWith(ExitBlock);

      // Rewrite phis in the exit block to get their inputs from
      // the newBB instead of the exiting block.
      if(DEBUG) errs() << "Attempting to rewrite PHI nodes in exit block\n";
      BasicBlock *ExitingBlock = ExitingBlocks[0];
      BasicBlock::iterator BI = ExitBlock->begin();
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
      for (Loop::block_iterator LI = L->block_begin(), LE = L->block_end();
           LI != LE; ++LI) {
        // Move all of the block's children to be children of the Preheader, which
        // allows us to remove the domtree entry for the block.
        ChildNodes.insert(ChildNodes.begin(), DT[*LI]->begin(), DT[*LI]->end());
        for (DomTreeNode *ChildNode : ChildNodes) {
          DT.changeImmediateDominator(ChildNode, DT[Preheader]);
        }

        BasicBlock * bb = *LI;
        for(BasicBlock::iterator II = bb->begin(); II != bb->end(); ++II) {
           if(DEBUG) errs() << "Dropping references\n";
           Instruction * insII = &(*II);
           insII->dropAllReferences();
        }

        ChildNodes.clear();
        DT.eraseNode(*LI);

        // Remove the block from the reference counting scheme, so that we can
        // delete it freely later.
        (*LI)->dropAllReferences();
      }

      // Erase the instructions and the blocks without having to worry
      // about ordering because we already dropped the references.
      // NOTE: This iteration is safe because erasing the block does not remove its
      // entry from the loop's block list.  We do that in the next section.
      if(DEBUG) errs() << "Erasing instructions without caring about references\n";
      for (Loop::block_iterator LI = L->block_begin(), LE = L->block_end();
           LI != LE; ++LI)
        (*LI)->eraseFromParent();

      // Finally, erase the blocks from loopinfo.
      // This has to happen late because
      // otherwise our loop iterators won't work.
      if(DEBUG) errs() << "Erasing loopinfo\n";
      SmallPtrSet<BasicBlock *, 8> blocks;
      blocks.insert(L->block_begin(), L->block_end());
      for (BasicBlock *BB : blocks)
        LI.removeBlock(BB);

      // The last step is to update LoopInfo now that we've eliminated this loop.
      //LI.erase(L); //TODO

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
      return true;
    }

  };

}

char InvokeCGRA::ID = 0;
static RegisterPass<InvokeCGRA> X("InvokeCGRA", "Loop Pass to Invoke Functions for Loop Acceleration Over CGRA");
