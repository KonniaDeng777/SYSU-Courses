#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Support/raw_ostream.h>

class ConstantPropagation : public llvm::PassInfoMixin<ConstantPropagation> {
public:
  explicit ConstantPropagation(llvm::raw_ostream& out) : mOut(out) {}
  llvm::PreservedAnalyses run(llvm::Module& mod, llvm::ModuleAnalysisManager& mam);
  int constPropagationTimes = 0;

private:
  llvm::raw_ostream& mOut;
  void propagateConstants(llvm::Function& func);
};

