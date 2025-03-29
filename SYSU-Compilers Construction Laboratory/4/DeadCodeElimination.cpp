#include "DeadCodeElimination.hpp"
#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstIterator.h>

using namespace llvm;

PreservedAnalyses
DeadCodeElimination::run(Module& mod, ModuleAnalysisManager& mam)
{
  int deadCodeCount = 0;
  std::vector<Instruction*> instToErase;

  for (auto& func : mod) {
    for (auto& inst : instructions(func)) {
      if (!hasSideEffects(inst) && !isUsedOutsideOfDef(inst)) {
        instToErase.push_back(&inst);
      }
    }
  }

  for (auto* inst : instToErase) {
    inst->eraseFromParent();
    ++deadCodeCount;
  }

  mOut << "DeadCodeElimination running...\nEliminated " << deadCodeCount << " dead instructions\n";
  return PreservedAnalyses::all();
}

bool
DeadCodeElimination::hasSideEffects(const Instruction& inst) const {
  return inst.mayHaveSideEffects() || inst.isTerminator();
}

bool
DeadCodeElimination::isUsedOutsideOfDef(const Instruction& inst) const {
  return !inst.use_empty();
}
