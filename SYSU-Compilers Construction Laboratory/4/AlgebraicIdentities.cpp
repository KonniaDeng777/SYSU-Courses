#include "AlgebraicIdentities.hpp"

using namespace llvm;

PreservedAnalyses
AlgebraicIdentities::run(Module& mod, ModuleAnalysisManager& mam)
{
  int algebraicIdTimes = 0;
  for (auto& func : mod) {
    for (auto& bb : func) {
      std::vector<Instruction*> instToErase;
      for (auto& inst : bb) {
        if (auto binOp = dyn_cast<BinaryOperator>(&inst)) {
          Value* lhs = binOp->getOperand(0);
          Value* rhs = binOp->getOperand(1);
          auto constLhs = dyn_cast<ConstantInt>(lhs);
          auto constRhs = dyn_cast<ConstantInt>(rhs);
          bool simplified = false;
          Value* newVal = nullptr;

          switch (binOp->getOpcode()) {
            case Instruction::Add: {
              if (constRhs && constRhs->isZero()) {
                newVal = lhs;  // x + 0 = x
                simplified = true;
              } else if (constLhs && constLhs->isZero()) {
                newVal = rhs;  // 0 + x = x
                simplified = true;
              }
              break;
            }
            case Instruction::Sub: {
              if (constRhs && constRhs->isZero()) {
                newVal = lhs;  // x - 0 = x
                simplified = true;
              }
              break;
            }
            case Instruction::Mul: {
              if (constRhs && constRhs->isOne()) {
                newVal = lhs;  // x * 1 = x
                simplified = true;
              } else if (constLhs && constLhs->isOne()) {
                newVal = rhs;  // 1 * x = x
                simplified = true;
              } else if (constRhs && constRhs->isZero()) {
                newVal = rhs;  // x * 0 = 0
                simplified = true;
              } else if (constLhs && constLhs->isZero()) {
                newVal = lhs;  // 0 * x = 0
                simplified = true;
              }
              break;
            }
            case Instruction::UDiv:
            case Instruction::SDiv: {
              if (constRhs && constRhs->isOne()) {
                newVal = lhs;  // x / 1 = x
                simplified = true;
              }
              break;
            }
            case Instruction::URem:
            case Instruction::SRem: {
              if (constRhs && constRhs->isOne()) {
                newVal = ConstantInt::get(binOp->getType(), 0);  // x % 1 = 0
                simplified = true;
              }
              break;
            }
            default:
              break;
          }

          if (simplified && newVal) {
            binOp->replaceAllUsesWith(newVal);
            instToErase.push_back(binOp);
            ++algebraicIdTimes;
          }
        }
      }
      for (auto& i : instToErase)
        i->eraseFromParent();
    }
  }

  mOut << "AlgebraicIdentities running...\nTo eliminate " << algebraicIdTimes
       << " instructions\n";
  return PreservedAnalyses::all();
}
