#include "LoopUnrolling.hpp"

using namespace llvm;

PreservedAnalyses 
LoopUnrolling::run(Module& mod, ModuleAnalysisManager& mam)
{
  int unrollCount = 0;

  for (auto& func : mod) {
    for (auto& bb : func) {
      std::vector<Instruction*> instToErase;
      for (auto& inst : bb) {
        if (auto *brInst = dyn_cast<BranchInst>(&inst)) {
          if (brInst->isConditional()) {
            if (auto *cmpInst = dyn_cast<ICmpInst>(brInst->getCondition())) {
              if (cmpInst->getPredicate() == ICmpInst::ICMP_SLT) {
                if (auto *loopVar = dyn_cast<PHINode>(cmpInst->getOperand(0))) {
                  if (auto *loopBound = dyn_cast<LoadInst>(cmpInst->getOperand(1))) {
                    if (auto *loopBoundValue = dyn_cast<ConstantInt>(loopBound->getPointerOperand())) {
                      int loopBoundInt = loopBoundValue->getSExtValue();
                      IRBuilder<> builder(&inst);
                      for (int i = 0; i < loopBoundInt; ++i) {
                        for (auto &bbInst : *brInst->getSuccessor(0)) {
                          auto *clonedInst = bbInst.clone();
                          builder.Insert(clonedInst);
                          instToErase.push_back(clonedInst);
                        }
                      }
                      instToErase.push_back(&inst);
                      unrollCount++;
                    }
                  }
                }
              }
            }
          }
        }
      }
      for (auto& i : instToErase)
        i->eraseFromParent();
    }
  }

  mOut << "LoopUnrolling running...\nUnrolled " << unrollCount << " loops\n";
  return PreservedAnalyses::all();
}




