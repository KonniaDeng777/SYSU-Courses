#pragma once

#include <llvm/IR/PassManager.h>
#include <llvm/Support/raw_ostream.h>

class LICM : public llvm::PassInfoMixin<LICM> {
public:
  explicit LICM(llvm::raw_ostream& out)
    : mOut(out)
  {
  }

  llvm::PreservedAnalyses run(llvm::Module& mod,
                              llvm::ModuleAnalysisManager& mam);

private:
  llvm::raw_ostream& mOut;
  bool isLogicalOp(unsigned int Opcode);
};

