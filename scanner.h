#pragma once
#ifndef __FLEX_LEXER_H
#include "FlexLexer.h"
#endif

// Substitute the default lex entry point with one that understands our types.
#include "parser.hh"  
#include "scanner.h" 
#define YY_DECL \
    Mreow::Parser::token_type Mreow::Scanner::lex( \
            Mreow::Parser::semantic_type* yylval, Mreow::Parser::location_type* yylloc \
    )


namespace Mreow
{

class Scanner : public yyFlexLexer
{
public:
    Scanner() {}

    virtual Parser::token_type lex(Parser::semantic_type* yylval, Parser::location_type* yylloc);
};

}
