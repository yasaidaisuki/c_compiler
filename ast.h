#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

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
      virtual Value *codegen() = 0;
};

class Immediate : public ASTNode {
   int val;
   public:
      Immediate(int val): ASTNode{IMMEDIATE}, val{val} {}
      void print() override {
         std::cout << val << std::endl;
      }
};

class Variable : public ASTNode {
   std::string var_name;
   public:
      Variable(const std::string &name) : ASTNode{VARIABLE}, var_name{name} {}
      void print() override {
         std::cout << "var name: " << var_name << std::endl;
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
};
