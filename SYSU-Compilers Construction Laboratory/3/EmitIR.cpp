#include "EmitIR.hpp"
#include <llvm/Transforms/Utils/ModuleUtils.h>
//#include "../../../../usr/include/llvm-14/llvm/IR/Type.h"

#define self (*this)

using namespace asg;

EmitIR::EmitIR(Obj::Mgr& mgr, llvm::LLVMContext& ctx, llvm::StringRef mid)
  : mMgr(mgr)
  , mMod(mid, ctx)
  , mCtx(ctx)
  , mIntTy(llvm::Type::getInt32Ty(ctx))
  , mCurIrb(std::make_unique<llvm::IRBuilder<>>(ctx))
  , mCtorTy(llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false))
{
}

llvm::Module&
EmitIR::operator()(asg::TranslationUnit* tu)
{
  for (auto&& i : tu->decls)
    self(i);
  return mMod;
}
void
EmitIR::trans_init(llvm::Value* val, asg::Expr* obj)
{
  auto& irb = *mCurIrb;

  // 仅处理整数字面量的初始化
  if (auto p = obj->dcst<IntegerLiteral>()) {
    auto initVal = llvm::ConstantInt::get(self(p->type), p->val);
    irb.CreateStore(initVal, val);
    return;
  }
    // 处理字符串字面量的初始化
  if (auto p = obj->dcst<StringLiteral>()) {
    auto strVal = llvm::ConstantDataArray::getString(irb.getContext(), p->val);
    auto globalStr = new llvm::GlobalVariable(*irb.GetInsertBlock()->getModule(),
    strVal->getType(),true, llvm::GlobalValue::PrivateLinkage,strVal, ".str");
    auto strPtr = irb.CreatePointerCast(globalStr, val->getType());
    irb.CreateStore(strPtr, val);
    return;
  }
  // 中断编译
  ABORT();
}

//==============================================================================
// 类型
//==============================================================================

llvm::Type*
EmitIR::operator()(const Type* type)
{
  if (type->texp == nullptr) {
    switch (type->spec) {
      case Type::Spec::kInt:
        return llvm::Type::getInt32Ty(mCtx);
      // TODO: 在此添加对更多基础类型的处理
      case Type::Spec::kVoid:
        return llvm::Type::getVoidTy(mCtx);
      case Type::Spec::kChar:
        return llvm::Type::getInt8Ty(mCtx);
      case Type::Spec::kLong:
        return llvm::Type::getInt64Ty(mCtx); 
      case Type::Spec::kLongLong:
        return llvm::Type::getInt64Ty(mCtx);
      default:
        ABORT();
    }
  }

  Type subt;
  subt.spec = type->spec;
  subt.qual = type->qual;
  subt.texp = type->texp->sub;

  // TODO: 在此添加对指针类型、数组类型和函数类型的处理
  if (auto p = type->texp->dcst<PointerType>()) {
    return llvm::PointerType::get(self(&subt), 0);
  }
  if (auto p = type->texp->dcst<ArrayType>()) {
    llvm::Type* elementType = self(&subt);
    uint64_t numElements = p->len;
    return llvm::ArrayType::get(elementType, numElements);
  }

  if (auto p = type->texp->dcst<FunctionType>()) 
  {
    std::vector<llvm::Type*> pty;
    // TODO: 在此添加对函数参数类型的处理
    for (auto param : p->params) {
      pty.push_back(self(param));
    }
    return llvm::FunctionType::get(self(&subt), std::move(pty), false);
  }

  ABORT();
}

//==============================================================================
// 表达式
//==============================================================================

llvm::Value*
EmitIR::operator()(Expr* obj)
{
  // TODO: 在此添加对更多表达式处理的跳转
  if (auto p = obj->dcst<IntegerLiteral>())
    return self(p);

  if (auto p = obj->dcst<DeclRefExpr>())
    return self(p);

  if (auto p = obj->dcst<ImplicitCastExpr>())
    return self(p);

  if (auto p = obj->dcst<BinaryExpr>())
    return self(p);

  if (auto p = obj->dcst<UnaryExpr>())
    return self(p);

  if (auto p = obj->dcst<ParenExpr>())
    return self(p);

  if (auto p = obj->dcst<CallExpr>())
    return self(p);

  if (auto p = obj->dcst<InitListExpr>())
    return self(p);
  
  if (auto p = obj->dcst<ImplicitInitExpr>())
    return self(p);
  ABORT();
}

llvm::Constant*
EmitIR::operator()(IntegerLiteral* obj)
{
  return llvm::ConstantInt::get(self(obj->type), obj->val);
}


// TODO: 在此添加对更多表达式类型的处理

llvm::Value*
EmitIR::operator()(ImplicitCastExpr* obj)
{
  auto sub = self(obj->sub);

  auto& irb = *mCurIrb;
  switch (obj->kind) {
    case ImplicitCastExpr::kLValueToRValue: {
      auto ty = self(obj->sub->type);
      auto loadVal = irb.CreateLoad(ty, sub);
      return loadVal;
    }
    case ImplicitCastExpr::kFunctionToPointerDecay:{
      auto funcType = self(obj->sub->type);
      auto ptrType = funcType->getPointerTo();
      auto bitcast = irb.CreateBitCast(sub, ptrType);
      return bitcast;
    }
    case ImplicitCastExpr::kArrayToPointerDecay: {
      auto arrayTy = llvm::dyn_cast<llvm::ArrayType>(self(obj->sub->type));
      if (!arrayTy) {
        ABORT();
      }
      auto elementPtrTy = arrayTy->getElementType()->getPointerTo();
      std::vector<llvm::Value*> idxList{irb.getInt64(0), irb.getInt64(0)};
      auto elementPtr = irb.CreateInBoundsGEP(arrayTy, sub, idxList);
      return irb.CreateBitCast(elementPtr, elementPtrTy);
    }
    default:
      ABORT();
  }
}

llvm::Value*
EmitIR::operator()(DeclRefExpr* obj)
{
  return reinterpret_cast<llvm::Value*>(obj->decl->any);
}

llvm::Value*
EmitIR::operator()(BinaryExpr* obj)
{
  llvm::Value *lftVal, *rhtVal;

  //auto func = mCurFunc;
  if (obj->op == BinaryExpr::kAssign) 
  {
    if (auto decl_expr = obj->lft->dcst<DeclRefExpr>()) {
      lftVal = self(decl_expr);
    }
    else {
      ABORT(); 
    }
  }
  else {
    lftVal = self(obj->lft);
  }


  //lftVal = self(obj->lft);
  auto& irb = *mCurIrb;
  rhtVal = self(obj->rht);
  switch (obj->op) {
    case BinaryExpr::kAdd:
      return irb.CreateAdd(lftVal, rhtVal);
    case BinaryExpr::kSub:
      return irb.CreateSub(lftVal,rhtVal);
    case BinaryExpr::kMul:
      return irb.CreateMul(lftVal,rhtVal);
    case BinaryExpr::kDiv:
      return irb.CreateUDiv(lftVal,rhtVal);
    case BinaryExpr::kMod:
      return irb.CreateURem(lftVal,rhtVal);
    case BinaryExpr::kAssign:
      return irb.CreateStore(rhtVal,lftVal);
    case BinaryExpr::kGt:
      return irb.CreateICmpSGT(lftVal,rhtVal);
    case BinaryExpr::kGe:
      return irb.CreateICmpSGE(lftVal,rhtVal);
    case BinaryExpr::kLt:
      return irb.CreateICmpSLT(lftVal,rhtVal);
    case BinaryExpr::kLe:
      return irb.CreateICmpSLE(lftVal,rhtVal);
    case BinaryExpr::kEq:
      return irb.CreateICmpEQ(lftVal,rhtVal);
    case BinaryExpr::kNe:
      return irb.CreateICmpNE(lftVal,rhtVal);
    case BinaryExpr::kAnd:
      return irb.CreateAnd(lftVal,rhtVal);
    case BinaryExpr::kOr:
      return irb.CreateOr(lftVal,rhtVal);
    case BinaryExpr::kComma:
      return rhtVal;
    case BinaryExpr::kIndex:
    {
      llvm::Value* indices[] = { irb.getInt32(0), rhtVal };
      llvm::Value* elementPtr = irb.CreateGEP(lftVal->getType(), lftVal, indices);
      return irb.CreateLoad(elementPtr->getType(), elementPtr);
    }
    //case BinaryExpr::kIndex:
    //  return irb.CreateInBoundsGEP(obj->lft,obj->rht);
    default:
      ABORT();
  }
}

llvm::Value* EmitIR::operator()(UnaryExpr* obj)
{
  auto& irb = *mCurIrb;

  llvm::Value* subVal = self(obj->sub);
  if (obj->sub->cate == Expr::Cate::kLValue)
  {
    subVal = irb.CreateLoad(irb.getInt32Ty(), subVal);
  }

  switch (obj->op)
  {
  case UnaryExpr::kNeg:
    return irb.CreateNeg(subVal);

  case UnaryExpr::kPos:
    return subVal;

  case UnaryExpr::kNot:
    return irb.CreateNot(subVal);

  default:
    ABORT();
  }
}


llvm::Value*
EmitIR::operator()(ParenExpr* obj)
{
  return self(obj->sub);
}


llvm::Value* 
EmitIR::operator()(CallExpr* obj)
{
    auto& irb = *mCurIrb;
    llvm::Function *func = static_cast<llvm::Function*>(self(obj->head));
    std::vector<llvm::Value*> args;
    for (auto arg : obj->args) {
        args.push_back(self(arg));
    }
    return irb.CreateCall(func, args);
}


llvm::Value*
EmitIR::operator()(InitListExpr* obj)
{
  auto& irb = *mCurIrb;
  auto ty = self(obj->type);

  if (auto arrayTy = llvm::dyn_cast<llvm::ArrayType>(ty)) {
    auto numElements = arrayTy->getNumElements();
    auto elementTy = arrayTy->getElementType();
    auto alloc = irb.CreateAlloca(arrayTy, nullptr, "arrayinit");
    for (std::size_t i = 0; i < obj->list.size(); ++i) {
      auto element = obj->list[i];
      auto initVal = self(element);
      std::vector<llvm::Value*> idxList{
        irb.getInt64(0), // 指向数组开头
        irb.getInt64(i)
      };
      auto elementPtr = irb.CreateInBoundsGEP(arrayTy, alloc, idxList);
      irb.CreateStore(initVal, elementPtr);
    }
    return alloc;
  }
  // 处理非数组类型的初始化表达式
  ABORT();
}

llvm::Value*
EmitIR::operator()(ImplicitInitExpr *obj)
{
  return self(obj);
}


//==============================================================================
// 语句
//==============================================================================

void
EmitIR::operator()(Stmt* obj)
{
  // TODO: 在此添加对更多Stmt类型的处理的跳转

  if (auto p = obj->dcst<DeclStmt>())
    return self(p);

  if (auto p = obj->dcst<CompoundStmt>())
    return self(p);

  if (auto p = obj->dcst<ReturnStmt>())
    return self(p);
  
  if (auto p = obj->dcst<ExprStmt>())
    return self(p);

  if (auto p = obj->dcst<IfStmt>())
    return self(p);

  if (auto p = obj->dcst<WhileStmt>())
    return self(p);

  if (auto p = obj->dcst<BreakStmt>())
    return self(p);

  if (auto p = obj->dcst<ContinueStmt>())
    return self(p);

  if (auto p = obj->dcst<NullStmt>())
    return self(p);

  ABORT();
}

// TODO: 在此添加对更多Stmt类型的处理

void
EmitIR::operator()(CompoundStmt* obj)
{
  // TODO: 可以在此添加对符号重名的处理
  for (auto&& stmt : obj->subs)
    self(stmt);
}

void
EmitIR::operator()(ReturnStmt* obj)
{
  auto& irb = *mCurIrb;

  llvm::Value* retVal;
  if (!obj->expr)
    retVal = nullptr;
  else
    retVal = self(obj->expr);

  mCurIrb->CreateRet(retVal);

  auto exitBb = llvm::BasicBlock::Create(mCtx, "return_exit", mCurFunc);
  mCurIrb = std::make_unique<llvm::IRBuilder<>>(exitBb);
}

void 
EmitIR::operator()(DeclStmt* obj)
{
  for(auto&& decl : obj->decls) {
    //self(decl);
    // 局部变量声明
    auto& irb = *mCurIrb;
    auto lvar = decl->dcst<VarDecl>();
    auto ty = self(lvar->type);
    llvm::AllocaInst* p = nullptr;
    //llvm::AllocaInst* p = irb.CreateAlloca(ty, nullptr, lvar->name);
    //irb.CreateStore(llvm::ConstantInt::get(ty, 0), p); 
    if (auto Array_Type = llvm::dyn_cast<llvm::ArrayType>(ty))
    {
      p = irb.CreateAlloca(Array_Type,nullptr,lvar->name);
    }
    else
    {
      p = irb.CreateAlloca(ty, nullptr, lvar->name);
      irb.CreateStore(llvm::ConstantInt::get(ty, 0), p);
    }

    if (lvar->init != nullptr) {
      trans_init(p, lvar->init);
    }
    lvar->any=p;
  }
}

void
EmitIR::operator()(asg::ExprStmt* obj)
{
  self(obj->expr);
}

void
EmitIR::operator()(asg::IfStmt* obj)
{
  auto thenBb = llvm::BasicBlock::Create(mCtx, "if.then", mCurFunc);
  auto elseBb = llvm::BasicBlock::Create(mCtx, "if.else", mCurFunc);
  auto mergeBb = llvm::BasicBlock::Create(mCtx, "if.end", mCurFunc);

  llvm::Value* condValue = self(obj->cond);

  mCurIrb->CreateCondBr(condValue, thenBb, elseBb);

  mCurIrb->SetInsertPoint(thenBb);
  if (obj->then)
  {
    self(obj->then);
  }
  mCurIrb->CreateBr(mergeBb);

  mCurIrb->SetInsertPoint(elseBb);
  if (obj->else_)
  {
    self(obj->else_);
  }
  mCurIrb->CreateBr(mergeBb);

  mCurIrb->SetInsertPoint(mergeBb);
}

void EmitIR::operator()(asg::WhileStmt* obj)
{
  auto whileCondBb = llvm::BasicBlock::Create(mCtx, "while.cond", mCurFunc);
  auto whileBodyBb = llvm::BasicBlock::Create(mCtx, "while.body", mCurFunc);
  auto whileEndBb = llvm::BasicBlock::Create(mCtx, "while.end", mCurFunc);

  obj->body->any=whileEndBb;
  // Branch to the condition block
  mCurIrb->CreateBr(whileCondBb);

  // Emit condition block
  mCurIrb->SetInsertPoint(whileCondBb);
  llvm::Value* condValue = self(obj->cond);
  mCurIrb->CreateCondBr(condValue, whileBodyBb, whileEndBb);

  // Emit body block
  mCurIrb->SetInsertPoint(whileBodyBb);
  llvm::PHINode* loopExitPhi = mCurIrb->CreatePHI(llvm::Type::getInt1Ty(mCtx), 2, "loop.exit");
  loopExitPhi->addIncoming(condValue, whileCondBb); // Incoming value from condition block

  if (obj->body)
  {
    // 设置循环体的起始基本块
    obj->body->any = whileCondBb;
    self(obj->body);
  }

  // Branch to either loop start or loop end based on loop exit condition
  //mCurIrb->CreateCondBr(loopExitPhi, whileCondBb, whileEndBb);
  mCurIrb->CreateBr(whileCondBb);
  // Set insertion point for the end block
  mCurIrb->SetInsertPoint(whileEndBb);
}



void 
EmitIR::operator()(asg::ContinueStmt* obj)
{
  // Branch back to the condition block of the loop
  if (obj->loop && obj->loop->any)
  {
    llvm::BasicBlock* loopEndBb = reinterpret_cast<llvm::BasicBlock*>(obj->loop->any);
    mCurIrb->CreateBr(loopEndBb);
  }
}


void 
EmitIR::operator()(asg::BreakStmt* obj)
{
  // if (obj->loop)
  // {
  //   auto whileStmt = dynamic_cast<asg::WhileStmt*>(obj->loop);
  //   if (whileStmt && whileStmt->any)
  //   {
  //     llvm::BasicBlock* loopEndBb = reinterpret_cast<llvm::BasicBlock*>(whileStmt->any);
  //     mCurIrb->CreateBr(loopEndBb);
  //   }
  // }
  if (obj->loop && obj->loop->any)
  {
    llvm::BasicBlock* loopEndBb = reinterpret_cast<llvm::BasicBlock*>(obj->loop->any);
    mCurIrb->CreateBr(loopEndBb);
  }
}



void EmitIR::operator()(asg::NullStmt *obj)
{

}



//==============================================================================
// 声明
//==============================================================================

void
EmitIR::operator()(Decl* obj)
{
  // TODO: 添加变量声明处理的跳转
  if (auto p = obj->dcst<VarDecl>())
    return self(p);

  if (auto p = obj->dcst<FunctionDecl>())
    return self(p);

  ABORT();
}

// TODO: 添加变量声明的处理
void
EmitIR::operator()(VarDecl* obj)
{
  auto ty = llvm::Type::getInt32Ty(mCtx); // 直接使用 LLVM 的 int32 类型
  //if (mCurIrb->GetInsertBlock() == nullptr){
  //bool isConst=obj->type->qual.const_;
  
  auto gvar = new llvm::GlobalVariable(
    mMod, ty, false, llvm::GlobalVariable::ExternalLinkage, nullptr, obj->name);

  obj->any = gvar;

  // 默认初始化为 0
  gvar->setInitializer(llvm::ConstantInt::get(ty, 0));

  if (obj->init == nullptr)
    return;

  // 创建构造函数用于初始化
  mCurFunc = llvm::Function::Create(
    mCtorTy, llvm::GlobalVariable::PrivateLinkage, "ctor_" + obj->name, mMod);
  llvm::appendToGlobalCtors(mMod, mCurFunc, 65535);

  auto entryBb = llvm::BasicBlock::Create(mCtx, "entry", mCurFunc);
  mCurIrb = std::make_unique<llvm::IRBuilder<>>(entryBb);
  trans_init(gvar, obj->init);
  mCurIrb->CreateRet(nullptr);
  //}
  // else {
  //       // 创建局部变量
  //       auto lvar = mCurIrb->CreateAlloca(ty, nullptr, obj->name);
  //       obj->any = lvar;
  //       mCurIrb->CreateStore(llvm::ConstantInt::get(ty, 0), lvar); 

  //       if (obj->init) {
  //           trans_init(lvar, obj->init);
  //       }
  //   }
  
}

void
EmitIR::operator()(FunctionDecl* obj)
{
  // 创建函数
  // FunctionType
  auto fty = llvm::dyn_cast<llvm::FunctionType>(self(obj->type));
  // Create Function
  auto func = llvm::Function::Create(
    fty, llvm::GlobalVariable::ExternalLinkage, obj->name, mMod);

  obj->any = func;

  if (obj->body == nullptr)
    return;
  auto entryBb = llvm::BasicBlock::Create(mCtx, "entry", func);
  mCurIrb = std::make_unique<llvm::IRBuilder<>>(entryBb);
  auto& entryIrb = *mCurIrb;

  // TODO: 添加对函数参数的处理
  auto argIter = func->arg_begin();
  for (auto& param:obj->params)
  {
      argIter->setName(param->name);
      ++argIter;
  }


  // 翻译函数体
  mCurFunc = func;
  self(obj->body);
  auto& exitIrb = *mCurIrb;

  if (fty->getReturnType()->isVoidTy())
    exitIrb.CreateRetVoid();
  else
    exitIrb.CreateUnreachable();
}

