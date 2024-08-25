#ifndef TOKEN
#define TOKEN

#include <string>
#include "TokenType.h"

class Token {
   public: 
      TokenType type;
      std::string value;

      Token(TokenType type, std::string value) : type{type}, value{value} {}
};

#endif