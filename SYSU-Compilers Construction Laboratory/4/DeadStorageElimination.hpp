#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/InstVisitor.h>

class DeadStorageElimination : public llvm::PassInfoMixin<DeadStorageElimination>
{
public:
    explicit DeadStorageElimination(llvm::raw_ostream& out)
        : mOut(out)
    {
    }

    llvm::PreservedAnalyses run(llvm::Module& mod, llvm::ModuleAnalysisManager& mam);

private:
    llvm::raw_ostream& mOut;
};







