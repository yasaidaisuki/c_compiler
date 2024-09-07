%{
    #include <string>
    #include <cstdlib>
    #include <variant>

    #include "scanner.h"
    #include "parser.hh"

    using Token = Mreow::Parser::token;

    #define yyterminate() return( Token::YYEOF )

    #define YY_USER_ACTION yylloc->step(); yylloc->columns(yyleng);
    #define YY_VERBOSE
%}

/* Target the C++ implementation */
%option c++
/* Leave buffer-switching to us */
%option	noyywrap
/* Don't generate a default rule on our behalf */
%option nodefault
/* Don't try to #include unistd */
%option nounistd
/* Don't try to push tokens back into the input stream */
%option noinput
%option nounput
%option stack
%option verbose
%option noyylineno
%option noyyget_lineno yyset_lineno yyget_out yyset_out yyget_in yyset_in
%option warn
%option noyymore
%option ecs
%option align
%option read
/* We're not writing an interpreter */
%option never-interactive batch
/* Write a source file, but not a header file */
%option outfile="scanner.cc"

%{

int yyFlexLexer::yylex() { abort(); }

%}


digit   [0-9]
alpha   [a-zA-Z_]
comment					("#"[^\n]*)
whitespace				([ \t\r])
newline					(\n)

%%

%{
	yylloc->step();
%}

{comment}+				yylloc->step();
{whitespace}+			yylloc->step();
{newline}+				yylloc->lines (yyleng); yylloc->step();

"{"                   { return Token::OPEN_BRACE; }
"}"                   { return Token::CLOSE_BRACE; }
"("                   { return Token::OPEN_PAR; }
")"                   { return Token::CLOSE_PAR; }
"~"                   { return Token::NOT; }
"!"                   { return Token::NEGATION; }
"+"                   { return Token::ADD; }
"-"                   { return Token::SUB; }
"*"                   { return Token::MULT; }
"/"                   { return Token::DIV; }
"<"                   { return Token::LESS; }
"<="                  { return Token::LESS_OR_EQUAL; }
"="                   { return Token::EQUAL; }
";"                   { return Token::SEMI_COL; }
","                   { return Token::COMMA; }
"int"                 { return Token::KEYWORD_INT; }
"return"              { return Token::RETURN; }    
"if"                  { return Token::IF; }
"while"               { return Token::WHILE; }
{digit}+              { yylval->emplace<int>(std::atoi(yytext)); return Token::INT_LITERAL; }  
{alpha}({alpha}|{digit})* { yylval->emplace<std::string>(yytext); return Token::IDENTIFIER; }

<<EOF>>					return Token::YYEOF;
%%

