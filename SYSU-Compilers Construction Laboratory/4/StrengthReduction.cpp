#include "StrengthReduction.hpp"
#include <llvm/IR/Instructions.h>
#include <llvm/IR/PatternMatch.h>
#include <llvm/Support/MathExtras.h>

using namespace llvm;
using namespace llvm::PatternMatch;

// Implement computeMagicAndShift using StrengthReduction::MagicInfo
StrengthReduction::MagicInfo StrengthReduction::computeMagicAndShift(uint64_t d) {
  unsigned p = 64; // Assuming 64-bit integers
  uint64_t nc = -1ULL - (-1ULL % d);
  uint64_t m = 1 + nc / d;
  unsigned l = llvm::Log2_64(d);

  return {m, l}; // Using aggregate initialization for returning MagicInfo
}

llvm::PreservedAnalyses 
StrengthReduction::run(llvm::Module& mod, llvm::ModuleAnalysisManager& mam) {
  int strengthReductionCount = 0;

  for (auto& func : mod) {
    for (auto& bb : func) {
      std::vector<Instruction*> instToErase;

      for (auto& inst : bb) {
        if (auto* mul = dyn_cast<BinaryOperator>(&inst)) {
          if (match(mul, m_Mul(m_Value(), m_ConstantInt()))) {
            Value* lhs = mul->getOperand(0);
            ConstantInt* rhs = cast<ConstantInt>(mul->getOperand(1));
            
            // Check if rhs is a power of two
            if (rhs->getValue().isPowerOf2()) {
              // Replace multiplication with left shift
              auto* newInst = BinaryOperator::CreateShl(lhs, ConstantInt::get(rhs->getType(), rhs->getValue().logBase2()), "", &inst);
              mul->replaceAllUsesWith(newInst);
              instToErase.push_back(mul);
              ++strengthReductionCount;
            } else {
              // Replace multiplication with constant using shifts and adds
              APInt rhsValue = rhs->getValue();
              Value* result = nullptr;
              bool first = true;
              for (unsigned i = 0; i < rhsValue.getBitWidth(); ++i) {
                if (rhsValue[i]) {
                  Value* shift = lhs;
                  if (i > 0) {
                    shift = BinaryOperator::CreateShl(lhs, ConstantInt::get(rhs->getType(), i), "", &inst);
                  }
                  if (first) {
                    result = shift;
                    first = false;
                  } else {
                    result = BinaryOperator::CreateAdd(result, shift, "", &inst);
                  }
                }
              }
              if (result) {
                mul->replaceAllUsesWith(result);
                instToErase.push_back(mul);
                ++strengthReductionCount;
              }
            }
          }
        } else if (auto* rem = dyn_cast<BinaryOperator>(&inst)) {
          if (match(rem, m_URem(m_Value(), m_ConstantInt()))) {
            Value* lhs = rem->getOperand(0);
            ConstantInt* rhs = cast<ConstantInt>(rem->getOperand(1));
            
            // Replace remainder with subtraction and division
            auto* newInst = BinaryOperator::CreateUDiv(lhs, rhs, "", &inst);
            auto* subInst = BinaryOperator::CreateSub(lhs, BinaryOperator::CreateMul(newInst, rhs, "", &inst), "", &inst);
            rem->replaceAllUsesWith(subInst);
            instToErase.push_back(rem);
            ++strengthReductionCount;
          }
        } else if (auto* div = dyn_cast<BinaryOperator>(&inst)) {
          if (match(div, m_UDiv(m_Value(), m_ConstantInt()))) {
            Value* lhs = div->getOperand(0);
            ConstantInt* rhs = cast<ConstantInt>(div->getOperand(1));
            
            // Replace division with right shift
            if (rhs->getValue().isPowerOf2()) {
              auto* newInst = BinaryOperator::CreateLShr(lhs, ConstantInt::get(rhs->getType(), rhs->getValue().logBase2()), "", &inst);
              div->replaceAllUsesWith(newInst);
              instToErase.push_back(div);
              ++strengthReductionCount;
            } else {
              // Replace division with constant using multiplication and shift
              uint64_t rhsValue = rhs->getValue().getZExtValue();
              StrengthReduction::MagicInfo magicInfo = computeMagicAndShift(rhsValue);
              auto* mulMagic = BinaryOperator::CreateMul(lhs, ConstantInt::get(rhs->getType(), magicInfo.magic), "", &inst);
              auto* newInst = BinaryOperator::CreateLShr(mulMagic, ConstantInt::get(rhs->getType(), magicInfo.shift), "", &inst);
              div->replaceAllUsesWith(newInst);
              instToErase.push_back(div);
              ++strengthReductionCount;
            }
          }
        }
      }

      // Erase all instructions scheduled for deletion
      for (auto* inst : instToErase) {
        inst->eraseFromParent();
      }
    }
  }

  mOut << "StrengthReduction running...\n";
  mOut << "Replaced " << strengthReductionCount << " instructions.\n";

  return llvm::PreservedAnalyses::all();
}


