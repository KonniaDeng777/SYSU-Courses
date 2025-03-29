#include "Inliner.hpp"

#include <llvm/IR/Instructions.h>
#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/ADT/SCCIterator.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

#define DEBUG_TYPE "function-inlining"

PreservedAnalyses 
FunctionInlining::run(Module &M, ModuleAnalysisManager &AM) {
  SmallPtrSet<Function *, 8> FunctionsToInline = getFunctionsToInline(M);
  for (Function *F : FunctionsToInline) {
    if (!F->isDeclaration()) {
      InlineFunctionInfo IFI;
      for (auto &BB : *F) {
        for (auto &I : BB) {
          if (CallBase *CB = dyn_cast<CallBase>(&I)) {
            if (Function *Callee = CB->getCalledFunction()) {
              if (!Callee->isDeclaration()) {
                InlineResult IR = InlineFunction(*CB, IFI);
                if (IR.isSuccess()) {
                  // Function was inlined
                  LLVM_DEBUG(dbgs() << "Function " << Callee->getName()
                                    << " was inlined into " << F->getName() << "\n");
                } else {
                  // Inlining failed
                  LLVM_DEBUG(dbgs() << "Inlining failed for call to "
                                    << Callee->getName() << " in function "
                                    << F->getName() << ": " << IR.getFailureReason() << "\n");
                }
              }
            }
          }
        }
      }
    }
  }

  return PreservedAnalyses::all();
}

SmallPtrSet<Function *, 8> FunctionInlining::getFunctionsToInline(Module &M) {
  SmallPtrSet<Function *, 8> Result;

  CallGraph CG(M);

  for (scc_iterator<CallGraph *> I = scc_begin(&CG); !I.isAtEnd(); ++I) {
    const std::vector<CallGraphNode *> &SCCNodes = *I;
    bool SCCHasExternalCall = false;

    for (CallGraphNode *CGN : SCCNodes) {
      Function *F = CGN->getFunction();

      if (!F || F->isDeclaration())
        continue;

      if (CGN->getNumReferences() > 0) {
        SCCHasExternalCall = true;
        break;
      }
      Result.insert(F);
    }

    if (SCCHasExternalCall) {
      for (CallGraphNode *CGN : SCCNodes) {
        Function *F = CGN->getFunction();
        if (F && !F->isDeclaration())
          Result.erase(F);
      }
    }
  }

  return Result;
}
