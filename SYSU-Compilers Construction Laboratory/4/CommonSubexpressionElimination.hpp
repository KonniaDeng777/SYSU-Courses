#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Support/raw_ostream.h>
#include <unordered_map>
#include <vector>

class CommonSubexpressionElimination : public llvm::PassInfoMixin<CommonSubexpressionElimination>
{
public:
    explicit CommonSubexpressionElimination(llvm::raw_ostream& out)
        : mOut(out)
    {
    }

    llvm::PreservedAnalyses run(llvm::Module& mod,
                                llvm::ModuleAnalysisManager& mam);

private:
    llvm::raw_ostream& mOut;

    struct Expression
    {
        unsigned opcode;
        llvm::Type* type;
        std::vector<llvm::Value*> operands;

        bool operator==(const Expression& other) const
        {
            return opcode == other.opcode && type == other.type && operands == other.operands;
        }
    };

    struct ExpressionHash
    {
        std::size_t operator()(const Expression& e) const
        {
            std::size_t hash = std::hash<unsigned>()(e.opcode) ^ std::hash<llvm::Type*>()(e.type);
            for (auto* operand : e.operands)
                hash ^= std::hash<llvm::Value*>()(operand);
            return hash;
        }
    };
};





