#pragma once

#include <llvm/IR/PassManager.h>
#include <llvm/Analysis/CallGraph.h>
#include <llvm/Transforms/Utils/Cloning.h>
#include <llvm/ADT/SmallPtrSet.h>

class FunctionInlining : public llvm::PassInfoMixin<FunctionInlining> {
public:
  llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &AM);

private:
  llvm::SmallPtrSet<llvm::Function *, 8> getFunctionsToInline(llvm::Module &M);
};






