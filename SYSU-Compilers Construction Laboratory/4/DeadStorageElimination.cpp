#include "DeadStorageElimination.hpp"

using namespace llvm;

PreservedAnalyses
DeadStorageElimination::run(Module& mod, ModuleAnalysisManager& mam)
{
  int deadStorageElimTimes = 0;
  auto isUsedMeaningfully = [](Instruction* inst) {
    for (auto& use : inst->uses()) {
      Instruction* userInst = dyn_cast<Instruction>(use.getUser());
      if (!userInst) continue;

      if (isa<StoreInst>(userInst) || isa<ReturnInst>(userInst) || 
          isa<CallInst>(userInst) || isa<LoadInst>(userInst) || 
          userInst->isTerminator()) {
        return true;
      }
    }
    return false;
  };

  for (auto& func : mod) {
    if (func.isDeclaration()) continue;

    std::vector<Instruction*> instToErase;

    for (auto& bb : func) {
      for (auto& inst : bb) {
        if (auto alloca = dyn_cast<AllocaInst>(&inst)) {
          bool allUsesDead = true;

          for (auto& use : alloca->uses()) {
            Instruction* userInst = dyn_cast<Instruction>(use.getUser());
            if (!userInst) continue;

            if (isa<StoreInst>(userInst)) {
              if (isUsedMeaningfully(userInst)) {
                allUsesDead = false;
                break;
              }
            } else if (isa<LoadInst>(userInst)) {
              if (isUsedMeaningfully(userInst)) {
                allUsesDead = false;
                break;
              }
            } else {
              allUsesDead = false;
              break;
            }
          }

          if (allUsesDead) {
            for (auto& use : alloca->uses()) {
              Instruction* userInst = dyn_cast<Instruction>(use.getUser());
              if (userInst) instToErase.push_back(userInst);
            }
            instToErase.push_back(alloca);
            ++deadStorageElimTimes;
          }
        }
      }
    }

    for (auto& i : instToErase)
      i->eraseFromParent();
  }

  mOut << "DeadStorageElimination running...\nEliminated " << deadStorageElimTimes
       << " dead storage instructions\n";
  return PreservedAnalyses::all();
}














