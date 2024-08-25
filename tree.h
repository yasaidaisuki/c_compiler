#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <map>

class ASTNode {
public:
    virtual ~ASTNode() {}
    virtual void print() const = 0;
};

class Expression : public ASTNode {
};

class Constant : public Expression {
    int value;
public:
    Constant(int value) : value(value) {}
    void print() const override {
        std::cout << "Constant value: " << value << std::endl;
    }
};

class Statement :public ASTNode {
    public:
};

class Return : public Statement {
    Expression *exp;
    public :
        Return(Expression* ex): exp{ex} {}
        void print() const override {
            std::cout << "return "<< std::endl;
            exp->print();
        }
        ~Return() {
            delete exp;
        }
};

class Function : public ASTNode {
    std::string fn_name ;
    Statement* statement;
    public: 
        Function(std::string name, Statement* s): fn_name{name}, statement{s} {}
        void print() const override {
            std::cout << "Function name: " << fn_name << std::endl;
            statement->print();
        }
        ~Function() {
            delete statement;
        }
};

class Program : public ASTNode {
    Function* function;
    public:
        Program(Function * f): function{f} {}
        void print() const override {
            std::cout << "Program" << std::endl;
            function->print();
        }
        ~Program() {
            delete function;
        }
};
