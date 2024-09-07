#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>

typedef enum ASTNodeType {
   IMMEDIATE,
   VARIABLE,
   BINARY_OP,
   UNARY_OP,
   BLOCK,
   IF_STATEMENT,
   RETURN_STATEMENT,
   FUNCTION,
   ASSIGNMENT,
   WHILE_STATEMENT,
   TOP_LEVEL
} ASTNodeType;

typedef enum {
   NEGATION,
   LOGICAL_NEGATION
} UnaryExpType;

typedef enum {
   ADD,
   SUB,
   DIV,
   MULT,
   LESS_THAN,
   LESS_THAN_EQUAL
} BinaryExpType;

class ASTNode {
   ASTNodeType type;
   public:
      ASTNode(ASTNodeType t): type{t} {}
      virtual ~ASTNode() {}
      virtual void print() = 0;
      virtual llvm::Value* codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) = 0;
};

class Immediate : public ASTNode {
   int val;
   public:
      Immediate(int val): ASTNode{IMMEDIATE}, val{val} {}
      void print() override {
         std::cout << val << std::endl;
      }
      llvm::Value* codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) override {
         return llvm::ConstantInt::get(context, llvm::APInt(32, val, true));
      }
};

class Variable : public ASTNode {
   std::string var_name;
   public:
      Variable(const std::string &name) : ASTNode{VARIABLE}, var_name{name} {}
      void print() override {
         std::cout << "var name: " << var_name << std::endl;
      }
      llvm::Value* codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) override {
         llvm::Value* var = module.getGlobalVariable(var_name);
         if (!var) {
            std::cerr << "Unknown variable: " << var_name << std::endl;
            return nullptr;
         }
         llvm::Type* varType = var->getType();
         return builder.CreateLoad(varType, var, var_name.c_str());
      }
};

class BinaryExp : public ASTNode {
   std::unique_ptr<ASTNode> left;
   std::unique_ptr<ASTNode> right;
   BinaryExpType bin_type;
public:
   BinaryExp(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right, BinaryExpType operatorType) 
      : ASTNode{BINARY_OP}, left{std::move(left)}, right{std::move(right)}, bin_type{operatorType} {}
   void print() override {
      left->print();
      std::cout << "BINARY OPERATOR" << std::endl;
      right->print();
   }
   llvm::Value* codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) override {
      llvm::Value* l = left->codegen(context, builder, module);
      llvm::Value* r = right->codegen(context, builder, module);
      switch (bin_type) {
         case ADD: return builder.CreateAdd(l, r, "addtmp");
         case SUB: return builder.CreateSub(l, r, "subtmp");
         case MULT: return builder.CreateMul(l, r, "multmp");
         case DIV: return builder.CreateSDiv(l, r, "divtmp");
         case LESS_THAN: return builder.CreateICmpSLT(l, r, "ltmp");
         case LESS_THAN_EQUAL: return builder.CreateICmpSLE(l, r, "leqtmp");
         default: return nullptr;
      }
   }

};

class UnaryExp : public ASTNode {
   std::unique_ptr<ASTNode> exp; // expression
   UnaryExpType unary_type;
   public:
      UnaryExp(std::unique_ptr<ASTNode> exp, UnaryExpType operatorType): 
         ASTNode{UNARY_OP}, exp{std::move(exp)}, unary_type{operatorType} {}
      void print() override {
         std::cout << "UNARY OPERATOR" << std::endl;
         exp->print();
      }
      llvm::Value* codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) override {
         llvm::Value* expr = exp->codegen(context, builder, module);
         if (!expr) return nullptr;

         switch (unary_type) {
               case NEGATION:
                  return builder.CreateNeg(expr, "negtmp");
               case LOGICAL_NEGATION:
                  return builder.CreateNot(expr, "nottmp");
               default:
                  return nullptr;
         }
      }
};

class Assignment : public ASTNode {
   std::string var_name;
   std::unique_ptr<ASTNode> exp;
   public:
      Assignment(const std::string &name, std::unique_ptr<ASTNode> exp): 
         ASTNode{ASSIGNMENT}, var_name{name}, exp{std::move(exp)} {}
      void print() override {
         std::cout << var_name << "= " << std::endl;
         exp->print();
      }
      llvm::Value* codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) override {
         llvm::Value* var = module.getGlobalVariable(var_name);
         llvm::Value* value = exp->codegen(context, builder, module);
         if (!var || !value) return nullptr;
         return builder.CreateStore(value, var);
      }
};

class ReturnStatement : public ASTNode {
   std::unique_ptr<ASTNode> exp;
   public:
      ReturnStatement(std::unique_ptr<ASTNode> exp): 
         ASTNode{RETURN_STATEMENT}, exp{std::move(exp)} {}
      void print() override {
         std::cout << "return: " << std::endl;
         exp->print();
      }
      llvm::Value* codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) override {
         llvm::Value* retVal = exp->codegen(context, builder, module);
         return builder.CreateRet(retVal);
      }
};

class IfStatement : public ASTNode {
   std::unique_ptr<ASTNode> condition;
   std::unique_ptr<ASTNode> then;
   public:  
      IfStatement(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> then):    
         ASTNode{IF_STATEMENT}, condition{std::move(condition)}, then{std::move(then)} {}
      void print() override {
         std::cout << "if (" << std::endl;
         condition->print();
         std::cout << ") " << std::endl;
         then->print();
      }
      llvm::Value* codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) override {
         llvm::Value* cond = condition->codegen(context, builder, module);
         llvm::Function* function = builder.GetInsertBlock()->getParent();

         llvm::BasicBlock* thenBB = llvm::BasicBlock::Create(context, "then", function);
         llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(context, "ifcont", function);

         builder.CreateCondBr(cond, thenBB, mergeBB);

         builder.SetInsertPoint(thenBB);
         then->codegen(context, builder, module);
         builder.CreateBr(mergeBB);

         builder.SetInsertPoint(mergeBB);

         return nullptr;
      }

};

class WhileStatement: public ASTNode {
   std::unique_ptr<ASTNode> condition;
   std::unique_ptr<ASTNode> body;
   public:
      WhileStatement(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> body): 
         ASTNode{WHILE_STATEMENT}, condition{std::move(condition)}, body{std::move(body)} {}
      void print() override {
         std::cout << "while (" << std::endl;
         condition->print();
         std::cout << ") " << std::endl;
         body->print();
      }
      llvm::Value* codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) override {
         llvm::Function* function = builder.GetInsertBlock()->getParent();

         llvm::BasicBlock* loopBB = llvm::BasicBlock::Create(context, "loop", function);
         llvm::BasicBlock* exitBB = llvm::BasicBlock::Create(context, "exit", function);

         llvm::BasicBlock* continueBB = builder.GetInsertBlock();
         builder.CreateCondBr(condition->codegen(context, builder, module), loopBB, exitBB);

         builder.SetInsertPoint(loopBB);
         body->codegen(context, builder, module);
         builder.CreateBr(continueBB);

         builder.SetInsertPoint(exitBB);

         return nullptr;
      }

};

class Block : public ASTNode {
   public:
      std::vector<std::unique_ptr<ASTNode>> statements; 
      Block(): ASTNode{BLOCK} {}

      void addStatements(std::unique_ptr<ASTNode> stmt) {statements.emplace_back(std::move(stmt));}
      void print() override {
         std::cout << "Block" << std::endl;
         for (const auto& stmt : statements) {
            if (stmt) {
                  stmt->print(); // Call the print method of each statement
            }
         }
      }
      llvm::Value* codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) override {
         for (const auto& stmt : statements) {
               stmt->codegen(context, builder, module);
         }
         return nullptr;
      }
};

class Function : public ASTNode {
   public:
      std::string name;
      std::vector<std::string> params;
      std::unique_ptr<Block> body;
      Function(const std::string &name, std::vector<std::string> params, std::unique_ptr<Block> body): 
         ASTNode{FUNCTION}, name{name}, params{params}, body{std::move(body)} {}
      void print() override {
         std::cout << "Fn Name: " << name << std::endl << "Params: ";
         for (const auto& p : params) {
            std::cout << p << ", ";
         }
         body->print();
      }
      llvm::Value* codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) override {
         std::vector<llvm::Type*> paramTypes(params.size(), llvm::Type::getInt32Ty(context));
         llvm::FunctionType* funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), paramTypes, false);
         llvm::Function* function = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, name, &module);

         llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", function);
         builder.SetInsertPoint(entry);

         unsigned idx = 0;
         for (auto& arg : function->args()) {
               arg.setName(params[idx++]);
         }

         body->codegen(context, builder, module);
         builder.CreateRet(llvm::ConstantInt::get(context, llvm::APInt(32, 0)));

         return function;
      }
};

class TopLevel : public ASTNode {
   public:
      std::vector<std::unique_ptr<ASTNode>> decl; // declarations
      TopLevel(): ASTNode{TOP_LEVEL} {}
      
      void addDecl(std::unique_ptr<ASTNode> d) {
         decl.emplace_back(std::move(d));
      }
      
      void print() override {
         std::cout << "Program: " << std::endl;
         for (const auto& d : decl) {
            if (d) {
               d->print(); // Call the print method of each declaration
            }
         }
      }
      
      llvm::Value* codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) override {
         for (const auto& d : decl) {
            d->codegen(context, builder, module);
         }
         return nullptr;  // You can return nullptr or some value if needed
      }
};
