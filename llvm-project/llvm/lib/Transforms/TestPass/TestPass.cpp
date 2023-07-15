#define DEBUG_TYPE "test-pass"

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


#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace llvm;

std::vector<MDNode*> visited_loops;

namespace llvm
{
    struct TestPass: public LoopPass
    {
        static char ID;
        TestPass(): LoopPass(ID)
        {

        }
        virtual bool HasBeenVisited(MDNode* loopid)
        {
            for(auto it = visited_loops.begin(); it != visited_loops.end(); it++)
            {
                if(*it == loopid)
                    return true;
            }
            return false;
        }
        virtual bool runOnLoop(Loop *L, LPPassManager &LPM)
        {
            if(HasBeenVisited(L->getLoopID()))
                return false;
            
            std::vector<BasicBlock*> bbs = L->getBlocks();
            L->dumpVerbose();
            
            return false;
        }
    }; // struct TessPass: Public LoopPass

} // namespace llvm
char TestPass::ID = 0;
static RegisterPass<TestPass> X("testpass", "Test Pass to test pass", false, true);

static RegisterStandardPasses Y(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
    legacy::PassManagerBase &PM) { PM.add(new TestPass()); });

