#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Token.cc"

class Lexer {
   public :
      std::vector<Token> tokens;
      Lexer(std::string filename) {
         std::fstream readfile{filename};
         std::string getWord; 
         while (getline(readfile,getWord)) {
           std::cout << readfile.is_open() << std::endl;
           // tokenize each line
           for (int i = 0 ; i < (int)getWord.size(); i++) {
               if (!(getWord[i] == ' ')) {
                  // parenthesis && braces
                  if (getWord[i] == '{') {
                     tokens.emplace_back(Token(TokenType::OpenBrace, "{"));
                  }
                  else if (getWord[i] == '}') {
                     tokens.emplace_back(Token(TokenType::CloseBrace, "}"));
                  }
                  else if (getWord[i] == '(') {
                     tokens.emplace_back(Token(TokenType::OpenParenthesis, "("));
                  }
                  else if (getWord[i] == ')') {
                     tokens.emplace_back(Token(TokenType::CloseParenthesis, ")"));
                  }
                  // keyword int
                  else if (i+2 < (int)getWord.size() && getWord.substr(i,i+3) == "int") {
                     tokens.emplace_back(Token(TokenType::KeywordInt, "int"));
                     i +=2;
                  } 
                  // return 
                  else if (i+5 < (int)getWord.size() && getWord.substr(i,i+6) == "return") {
                     tokens.emplace_back(Token(TokenType::Return, "return"));
                     i +=5;
                  } 
                  // Indentifiers and Int literal 
                  else if (std::isalpha(getWord[i]) || std::isdigit(getWord[i])) {
                     int j = i+1;
                     if 
                        (std::isalpha(getWord[i])) while (j < (int)getWord.size() && std::isalpha(getWord[j])) j++;
                     else 
                        while (j < (int)getWord.size() && std::isdigit(getWord[j])) j++;
                     tokens.emplace_back(Token(TokenType::Identifier, getWord.substr(i,j)));
                     i = j-1;
                  }
               }
           }
         }
      }
      std::vector<Token> getToken() {return tokens;}
};
