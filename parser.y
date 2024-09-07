%language "C++"             // Use the C++ model
%skeleton "lalr1.cc"        // Use the LALR(1) parser because we're demented
%require "3.8.2"            // Bison version

%locations                  // Track the line/column for us

%defines "parser.hh"        // Emit a header file with tokens etc
%output  "parser.cc"        // Name of the parser source

%define         api.location.file   "location.hh"

%define         api.namespace       {Mreow}

/* Call the parser ... 'Parser' */
%define         api.parser.class    {Parser}

%define         api.token.raw

/* Use our custom YYLVal type for storying yylval */
%define         api.value.type      variant

/* Tell the parser to assert if it gets into a bad state */
%define         parse.assert
%define         parse.trace

/* "unexpected" is unhelpful. "unexpected IDENTIFIER, expected NUMBER or STRING_LITERAL" is better. */
%define         parse.error         detailed

/* Try and do lookahead correction so it can say "did you mean 'response?'" */
%define         parse.lac           full

%code requires {
   #include <string>
   #include <iostream>
   #include <memory>
   #include <vector>
   #include "ast.h"

    namespace Mreow
    {
        class Driver;
        class Scanner;
    }

   extern std::unique_ptr<TopLevel> root;
}

%parse-param    { Mreow::Scanner& scanner }
%parse-param    { Mreow::Driver& driver }

%code
{
    #include "driver.h"
    #include "scanner.h"

    #define yylex scanner.lex
}

%token YYEOF    0   "end of file"
%token OPEN_BRACE CLOSE_BRACE
%token OPEN_PAR CLOSE_PAR
%token IF WHILE LESS_OR_EQUAL
%token KEYWORD_INT RETURN
%token SEMI_COL EQUAL COMMA

%left ADD SUB
%left MULT DIV
%left LESS LESS_OR_EQUAL
%left NOT NEGATION

%token<int> INT_LITERAL
%token<std::string> IDENTIFIER

%%

%nterm <std::unique_ptr<TopLevel>> program;
%nterm <std::unique_ptr<ASTNode>> function;
%nterm <std::vector<std::string>> parameter_list;
%nterm <std::unique_ptr<Block>> block;
%nterm <std::unique_ptr<ASTNode>> statement;
%nterm <std::vector<std::unique_ptr<ASTNode>>> statement_list;
%nterm <std::unique_ptr<ASTNode>> expression;

program:
   function {
      root = std::make_unique<TopLevel>();
      root->addDecl(std::move($1));
   }
   ;

function: 
    KEYWORD_INT IDENTIFIER OPEN_PAR parameter_list CLOSE_PAR OPEN_BRACE block CLOSE_BRACE
    {
        $$ = std::make_unique<Function>(
            std::string($2), 
            $4, 
            std::move($7)
        );
    }
    ;

parameter_list:
   %empty 
   {
      $$ = std::vector<std::string>();
   }
   |
   parameter_list COMMA KEYWORD_INT IDENTIFIER
   {
      $$ = std::move($1);
      $$.emplace_back($4);
   }
   ;

block:
   statement_list 
   {
      $$ = std::make_unique<Block>();
      for (auto &stmt : $1) {
         $$->addStatements(std::move(stmt));
      }
   }
   ;

statement_list:
   statement
   {
      $$ = std::vector<std::unique_ptr<ASTNode>>();
      $$.emplace_back(std::move($1));
   }
   |
   statement_list statement
   {
      $$ = std::move($1);
      $$.emplace_back(std::move($2));
   }
   ;

statement: 
   RETURN expression SEMI_COL
   {
      $$ = std::make_unique<ReturnStatement>(std::move($2));
   }
   |
   IF OPEN_PAR expression CLOSE_PAR OPEN_BRACE block CLOSE_BRACE 
   {
      $$ = std::make_unique<IfStatement>(std::move($3), std::move($6));
   }
   |
   WHILE OPEN_PAR expression CLOSE_PAR OPEN_BRACE block CLOSE_BRACE
   {
      $$ = std::make_unique<WhileStatement>(std::move($3), std::move($6));
   }
   |
   KEYWORD_INT IDENTIFIER EQUAL expression SEMI_COL
   {
      $$ = std::make_unique<Assignment>(std::string($2), std::move($4));
   }
   |
   expression SEMI_COL
   {
      $$ = std::move($1);
   }
   ;

expression: 
   INT_LITERAL 
   {
      $$ = std::make_unique<Immediate>($1);
   }
   |
   IDENTIFIER
   {
      //$$ = std::make_unique<Variable>(std::string($1));
      std::cout << "Hi";
   }
   |
   expression ADD expression
   {
      $$ = std::make_unique<BinaryExp>(std::move($1),std::move($3),BinaryExpType::ADD);
   }
   |
   expression SUB expression 
   {
      $$ = std::make_unique<BinaryExp>(std::move($1),std::move($3),BinaryExpType::SUB);
   }
   |
   expression MULT expression
   {
      $$ = std::make_unique<BinaryExp>(std::move($1),std::move($3),BinaryExpType::MULT);
   }
   |
   expression LESS expression
   {
      $$ = std::make_unique<BinaryExp>(std::move($1),std::move($3),BinaryExpType::LESS_THAN);
   }
   |
   expression LESS_OR_EQUAL expression
   {
      $$ = std::make_unique<BinaryExp>(std::move($1),std::move($3),BinaryExpType::LESS_THAN_EQUAL);
   }
   |
   SUB expression %prec SUB
   {
      $$ = std::make_unique<UnaryExp>(std::move($2),UnaryExpType::NEGATION);
   }
   |
   NOT expression
   {
      $$ = std::make_unique<UnaryExp>(std::move($2),UnaryExpType::LOGICAL_NEGATION);
   }
   ;
%%

#include <iostream>

void
Mreow::Parser::error(const location_type& l, const std::string& m)
{
    std::cerr << "filename" << ":" << l << ": " << m << "\n";
}
