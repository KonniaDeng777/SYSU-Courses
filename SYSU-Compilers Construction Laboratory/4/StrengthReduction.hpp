#pragma once

#include <llvm/IR/PassManager.h>
#include <llvm/Support/raw_ostream.h>

class StrengthReduction : public llvm::PassInfoMixin<StrengthReduction> {
public:
  explicit StrengthReduction(llvm::raw_ostream& out) : mOut(out) {}

  llvm::PreservedAnalyses run(llvm::Module& mod, llvm::ModuleAnalysisManager& mam);

  // Helper struct to hold magic number and shift for division optimization
  struct MagicInfo {
    uint64_t magic;
    unsigned shift;
  };

  MagicInfo computeMagicAndShift(uint64_t d); // Declaration moved outside private section

private:
  llvm::raw_ostream& mOut;
};

