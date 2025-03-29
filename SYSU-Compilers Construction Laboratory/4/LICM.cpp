#include "LICM.hpp"
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Dominators.h>
#include <llvm/Analysis/LoopInfo.h>


using namespace llvm;

bool LICM::isLogicalOp(unsigned int Opcode) {
    switch (Opcode) {
        case Instruction::And:
        case Instruction::Or:
        case Instruction::Xor:
            return true;
        default:
            return false;
    }
}

PreservedAnalyses 
LICM::run(Module &mod, ModuleAnalysisManager &mam) {
    int moveCount = 0;

    for (auto &func : mod) {
        if (func.isDeclaration())
            continue;

        DominatorTree dt(func);
        LoopInfo li(dt);

        for (auto *loop : li) {
            if (!loop->isInnermost())
                continue;

            BasicBlock *preheader = loop->getLoopPreheader();
            if (!preheader)
                continue;

            SmallVector<BasicBlock *, 8> exitBlocks;
            loop->getExitBlocks(exitBlocks);

            for (auto *bb : loop->getBlocks()) {
                if (bb == preheader)
                    continue;

                std::vector<Instruction *> toHoist;
                for (auto &I : *bb) {
                    if (I.isBinaryOp() || I.isCast() || I.isShift() || isLogicalOp(I.getOpcode())) {
                        bool isInvariant = true;
                        for (Use &U : I.operands()) {
                            Value *v = U.get();
                            Instruction *opInst = dyn_cast<Instruction>(v);
                            if (opInst && loop->contains(opInst->getParent())) {
                                isInvariant = false;
                                break;
                            }
                        }
                        if (isInvariant) {
                            toHoist.push_back(&I);
                        }
                    }
                }

                for (auto *I : toHoist) {
                    I->moveBefore(preheader->getTerminator());
                    ++moveCount;
                }
            }
        }
    }

    mOut << "LICM running...\nMoved " << moveCount << " instructions.\n";
    return PreservedAnalyses::all();
}
