#include "CommonSubexpressionElimination.hpp"

using namespace llvm;

PreservedAnalyses
CommonSubexpressionElimination::run(Module& mod, ModuleAnalysisManager& mam)
{
    int cseTimes = 0;
    for (auto& func : mod) {
        for (auto& bb : func) {
            std::unordered_map<Expression, Instruction*, ExpressionHash> exprTable;
            std::vector<Instruction*> instToErase;

            for (auto& inst : bb) {
                if (auto binOp = dyn_cast<BinaryOperator>(&inst)) {
                    std::vector<Value*> operands;
                    for (unsigned i = 0; i < binOp->getNumOperands(); ++i) {
                        operands.push_back(binOp->getOperand(i));
                    }

                    Expression expr{binOp->getOpcode(), binOp->getType(), operands};

                    auto it = exprTable.find(expr);
                    if (it != exprTable.end()) {
                        binOp->replaceAllUsesWith(it->second);
                        instToErase.push_back(binOp);
                        ++cseTimes;
                    } else {
                        exprTable[expr] = binOp;
                    }
                }
            }

            for (auto& i : instToErase)
                i->eraseFromParent();
        }
    }

    mOut << "CommonSubexpressionElimination running...\nEliminated " << cseTimes << " common subexpressions\n";
    return PreservedAnalyses::all();
}






