//===- CGRAGen.cpp - Example code from "Writing an LLVM Pass" -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the LLVM pass to accelerate loops over CGRA
// This transformation inserts a function at beginning of main method
// to create a new CGRA thread
//
//  Author: Shail Dave
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "cgra-gen"

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

#include "llvm/IR/TypeBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/DerivedTypes.h"

#include <fstream>
#include <sstream>

using namespace llvm;


namespace llvm
{
  struct CGRAGen: public ModulePass
  {
    static char ID;
    Function *hookCreate;
    Function *hookDelete;
    CGRAGen() :
      ModulePass(ID)
    {

    }

    virtual bool runOnModule(Module &M)
    {
      Constant *hookFuncCreate;
      Constant *hookFuncDelete;
      bool retVal = false;
      hookFuncCreate = M.getFunction("createCGRA");
      hookCreate = cast<Function>(hookFuncCreate);
      hookFuncDelete = M.getFunction("deleteCGRA");
      hookDelete = cast<Function>(hookFuncDelete);
      for(Module::iterator F = M.begin(), E = M.end(); F!= E; ++F)
      {
        retVal |= runOnFunction(*F);
      }
      return retVal;
    }

    virtual bool runOnFunction(Function &F) {
      if(F.getName() == "main")
      {
        //Let's insert function call at the beginning of main function
        Instruction *newInst = CallInst::Create(hookCreate,"");
        F.begin()->getInstList().push_front(newInst);

        // F is a pointer to a Function instance
        Function::iterator i = F.end();
        i--;
        BasicBlock::iterator bbi = (*i).end();
        bbi--;

        Instruction *prevInst = &(*bbi);
        //Instruction *lastInst = CallInst::Create(hookDelete,"",prevInst);
      }
      return true;
    }

  };

}

char CGRAGen::ID = 0;
static RegisterPass<CGRAGen> X("CGRAGen", "Function Pass to Insert Functions for Creating CGRA Thread in order to Accelerate Loops Over CGRA");
