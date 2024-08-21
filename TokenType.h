#ifndef TokenType_H
#define TokenType_H

enum class TokenType {
   OpenBrace,           // {
   CloseBrace,          // } 
   OpenParenthesis,           // (
   CloseParenthesis,          // )
   Semicolon,           // ;  
   KeywordInt,          // int
   Intlit,              // [0-9]
   Return,              // return
   Identifier,          // [a-zA-Z]\w*
   Unknown,             // for unknown type
};

#endif