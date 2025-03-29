#include <iostream>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/raw_ostream.h>

#include "ConstantFolding.hpp"
#include "Mem2Reg.hpp"
#include "StaticCallCounter.hpp"
#include "StaticCallCounterPrinter.hpp"
#include "ConstantPropagation.hpp"
#include "DeadCodeElimination.hpp"
#include "CommonSubexpressionElimination.hpp"
#include "InstructionCombining.hpp"
#include "StrengthReduction.hpp"
#include "LICM.hpp"
#include "AlgebraicIdentities.hpp"
#include "DeadStorageElimination.hpp"
#include "Inliner.hpp"
#include "LoopUnrolling.hpp"


void
opt(llvm::Module& mod)
{
  using namespace llvm;

  // 定义分析pass的管理器
  // 定义Analysis Pass的管理器
  LoopAnalysisManager lam;
  FunctionAnalysisManager fam;
  CGSCCAnalysisManager cgam;
  ModuleAnalysisManager mam;
  ModulePassManager mpm;
  FunctionPassManager fpm;

  // 注册分析pass的管理器
  // 注册Analysis Pass的管理器
  PassBuilder pb;
  pb.registerModuleAnalyses(mam);
  pb.registerCGSCCAnalyses(cgam);
  pb.registerFunctionAnalyses(fam);
  pb.registerLoopAnalyses(lam);
  pb.crossRegisterProxies(lam, fam, cgam, mam);

  // 添加分析pass到管理器中
  // 添加Analysis Pass到管理器中
  // Analysis Pass：使用MAM.registerPass([]() { return sysu::analysisPass(); })函数添加
  mam.registerPass([]() { return StaticCallCounter(); });


  // 添加优化pass到管理器中
  // 添加Transform Pass到管理器中
  // Transform Pass：使用MPM.addPass(sysu::optPass())函数添加
  
  mpm.addPass(StaticCallCounterPrinter(llvm::errs()));
  mpm.addPass(Mem2Reg());
  mpm.addPass(LoopUnrolling(llvm::errs())); 
  mpm.addPass(ConstantPropagation(llvm::errs()));
  mpm.addPass(ConstantFolding(llvm::errs()));
  mpm.addPass(StrengthReduction(llvm::errs()));
  mpm.addPass(AlgebraicIdentities(llvm::errs()));
  mpm.addPass(CommonSubexpressionElimination(llvm::errs()));
  mpm.addPass(DeadCodeElimination(llvm::errs()));
  mpm.addPass(DeadStorageElimination(llvm::errs()));
  mpm.addPass(FunctionInlining());
  mpm.addPass(InstructionCombining(llvm::errs()));
  mpm.addPass(LICM(llvm::errs()));
  
  // 运行优化pass
  // 运行Transform Pass
  mpm.run(mod, mam);
}

int
main(int argc, char** argv)
{
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " <input> <output>\n";
    return -1;
  }

  llvm::LLVMContext ctx;

  llvm::SMDiagnostic err;
  auto mod = llvm::parseIRFile(argv[1], err, ctx);
  if (!mod) {
    std::cout << "Error: unable to parse input file: " << argv[1] << '\n';
    err.print(argv[0], llvm::errs());
    return -2;
  }

  std::error_code ec;
  llvm::StringRef outPath(argv[2]);
  llvm::raw_fd_ostream outFile(outPath, ec);
  if (ec) {
    std::cout << "Error: unable to open output file: " << argv[2] << '\n';
    return -3;
  }

  opt(*mod); // IR的优化发生在这里

  mod->print(outFile, nullptr, false, true);
  if (llvm::verifyModule(*mod, &llvm::outs()))
    return 3;
}
