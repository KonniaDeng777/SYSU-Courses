#include "InstructionCombining.hpp"

using namespace llvm;

PreservedAnalyses 
InstructionCombining::run(Module& mod, ModuleAnalysisManager& mam) {
  int combineTimes = 0;

  // 遍历所有函数
  for (auto& func : mod) {
    // 遍历每个函数的基本块
    for (auto& bb : func) {
      // 合并指令
      if (combineInstructions(bb)) {
        ++combineTimes;
      }
    }
  }

  mOut << "InstructionCombining running...\nCombined " << combineTimes << " instruction groups\n";
  return PreservedAnalyses::all();
}

bool InstructionCombining::combineInstructions(BasicBlock& bb) {
  bool changed = false;
  std::vector<Instruction*> instToErase;

  for (auto it = bb.begin(), end = bb.end(); it != end; ++it) {
    if (auto* binOp = dyn_cast<BinaryOperator>(&*it)) {
      if (combineBinaryOperator(binOp, bb, instToErase)) {
        changed = true;
      }
    }
  }

  for (auto* inst : instToErase) {
    inst->eraseFromParent();
  }

  return changed;
}

bool InstructionCombining::combineBinaryOperator(BinaryOperator* binOp, BasicBlock& bb, std::vector<Instruction*>& instToErase) {
  bool changed = false;
  Value* lhs = binOp->getOperand(0);
  Value* rhs = binOp->getOperand(1);

  if (auto* lhsBinOp = dyn_cast<BinaryOperator>(lhs)) {
    switch (binOp->getOpcode()) {
      case Instruction::Add:
        if (lhsBinOp->getOpcode() == Instruction::Add) {
          Value* lhsRhs = lhsBinOp->getOperand(1);
          binOp->replaceAllUsesWith(
            BinaryOperator::CreateAdd(lhsBinOp->getOperand(0),
                                      BinaryOperator::CreateAdd(lhsRhs, rhs, "", binOp), "", binOp));
          instToErase.push_back(binOp);
          changed = true;
        }
        break;

      case Instruction::Sub:
        if (lhsBinOp->getOpcode() == Instruction::Sub) {
          Value* lhsRhs = lhsBinOp->getOperand(1);
          binOp->replaceAllUsesWith(
            BinaryOperator::CreateSub(lhsBinOp->getOperand(0),
                                      BinaryOperator::CreateAdd(lhsRhs, rhs, "", binOp), "", binOp));
          instToErase.push_back(binOp);
          changed = true;
        }
        break;

      case Instruction::Mul:
        if (lhsBinOp->getOpcode() == Instruction::Mul) {
          Value* lhsRhs = lhsBinOp->getOperand(1);
          binOp->replaceAllUsesWith(
            BinaryOperator::CreateMul(lhsBinOp->getOperand(0),
                                      BinaryOperator::CreateMul(lhsRhs, rhs, "", binOp), "", binOp));
          instToErase.push_back(binOp);
          changed = true;
        }
        break;

      case Instruction::SDiv:
        if (lhsBinOp->getOpcode() == Instruction::SDiv) {
          Value* lhsRhs = lhsBinOp->getOperand(1);
          binOp->replaceAllUsesWith(
            BinaryOperator::CreateSDiv(lhsBinOp->getOperand(0),
                                       BinaryOperator::CreateSDiv(lhsRhs, rhs, "", binOp), "", binOp));
          instToErase.push_back(binOp);
          changed = true;
        }
        break;

      case Instruction::UDiv:
        if (lhsBinOp->getOpcode() == Instruction::UDiv) {
          Value* lhsRhs = lhsBinOp->getOperand(1);
          binOp->replaceAllUsesWith(
            BinaryOperator::CreateUDiv(lhsBinOp->getOperand(0),
                                       BinaryOperator::CreateUDiv(lhsRhs, rhs, "", binOp), "", binOp));
          instToErase.push_back(binOp);
          changed = true;
        }
        break;

      case Instruction::SRem:
        if (lhsBinOp->getOpcode() == Instruction::SRem) {
          Value* lhsRhs = lhsBinOp->getOperand(1);
          binOp->replaceAllUsesWith(
            BinaryOperator::CreateSRem(lhsBinOp->getOperand(0),
                                       BinaryOperator::CreateSRem(lhsRhs, rhs, "", binOp), "", binOp));
          instToErase.push_back(binOp);
          changed = true;
        }
        break;

      case Instruction::URem:
        if (lhsBinOp->getOpcode() == Instruction::URem) {
          Value* lhsRhs = lhsBinOp->getOperand(1);
          binOp->replaceAllUsesWith(
            BinaryOperator::CreateURem(lhsBinOp->getOperand(0),
                                       BinaryOperator::CreateURem(lhsRhs, rhs, "", binOp), "", binOp));
          instToErase.push_back(binOp);
          changed = true;
        }
        break;

      default:
        break;
    }
  }

  return changed;
}



