#include "lexer.cc"
#include "parser.cc"
#include <iostream>
#include <list>
using namespace std;

int main(void) {
   list<Token> tokens = lexer("int_test.c");
   // for (auto t : tokens) {
   //    cout << t.value << endl;
   // }
   ASTNode* tree = parser(tokens);
   tree->print();
}