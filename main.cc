#include "Lexer.cc"
#include <iostream>
#include <vector>
using namespace std;

int main() {
   Lexer mreow{"int_test.c"};
   vector<Token> tokens = mreow.getToken();
   for (Token& t: tokens) {
      std::cout << "Value: " << t.value << endl;
   }
}