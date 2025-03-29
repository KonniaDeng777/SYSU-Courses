#include "ConstantPropagation.hpp"

using namespace llvm;

PreservedAnalyses 
ConstantPropagation::run(Module& mod, ModuleAnalysisManager& mam) {
  constPropagationTimes = 0;

  for (auto& func : mod) {
    propagateConstants(func); 
  }

  mOut << "ConstantFolding running...\nTo eliminate " << constPropagationTimes
       << " instructions\n";
  return PreservedAnalyses::all();
}

void ConstantPropagation::propagateConstants(Function& func) {
  constPropagationTimes = 0;
  std::unordered_map<Value*, Constant*> constMap;

  for (BasicBlock& bb : func) {
    for (Instruction& inst : bb) {
      if (auto* binOp = dyn_cast<BinaryOperator>(&inst)) {
        Value* lhs = binOp->getOperand(0);
        Value* rhs = binOp->getOperand(1);

        if (constMap.count(lhs) && constMap.count(rhs)) {
          ConstantInt* constLhs = dyn_cast<ConstantInt>(constMap[lhs]);
          ConstantInt* constRhs = dyn_cast<ConstantInt>(constMap[rhs]);

          if (constLhs && constRhs) {
            switch (binOp->getOpcode()) {
              case Instruction::Add:
                binOp->replaceAllUsesWith(ConstantInt::getSigned(
                    binOp->getType(), constLhs->getSExtValue() + constRhs->getSExtValue()));
                break;
              case Instruction::Sub:
                binOp->replaceAllUsesWith(ConstantInt::getSigned(
                    binOp->getType(), constLhs->getSExtValue() - constRhs->getSExtValue()));
                break;
              case Instruction::Mul:
                binOp->replaceAllUsesWith(ConstantInt::getSigned(
                    binOp->getType(), constLhs->getSExtValue() * constRhs->getSExtValue()));
                break;
              case Instruction::UDiv:
              case Instruction::SDiv:
                binOp->replaceAllUsesWith(ConstantInt::getSigned(
                    binOp->getType(), constLhs->getSExtValue() / constRhs->getSExtValue()));
                break;
              case Instruction::URem:
              case Instruction::SRem:
                binOp->replaceAllUsesWith(ConstantInt::getSigned(
                    binOp->getType(), constLhs->getSExtValue() % constRhs->getSExtValue()));
                break;
              default:
                break;
            }
            constPropagationTimes++;
          }
        }
      }

      if (auto* storeInst = dyn_cast<StoreInst>(&inst)) {
        Value* ptrOperand = storeInst->getPointerOperand();
        Value* valOperand = storeInst->getValueOperand();

        if (auto* constVal = dyn_cast<ConstantInt>(valOperand)) {
          constMap[ptrOperand] = constVal;
        }
      }
    }
  }
}
