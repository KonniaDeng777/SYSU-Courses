#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Support/raw_ostream.h>

class InstructionCombining : public llvm::PassInfoMixin<InstructionCombining> {
public:
  explicit InstructionCombining(llvm::raw_ostream& out)
    : mOut(out) {
  }

  llvm::PreservedAnalyses run(llvm::Module& mod,
                              llvm::ModuleAnalysisManager& mam);

private:
  llvm::raw_ostream& mOut;

  bool combineInstructions(llvm::BasicBlock& bb);
  bool combineBinaryOperator(llvm::BinaryOperator* binOp, llvm::BasicBlock& bb, std::vector<llvm::Instruction*>& instToErase);
};






