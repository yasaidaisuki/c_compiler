#include "Token.h"
#include <string>
#include <iostream>
#include <list>
#include "tree.h"
#include <sstream>

ASTNode* parseExp(std::list<Token> &tokens) {
    if (tokens.empty() || tokens.front().type != TokenType::Intlit ) return nullptr;
    std::stringstream iss{tokens.front().value};
    int i;
    iss >> i;
    tokens.pop_front();
    
    return new Constant{i};
}   

ASTNode* parseStatement(std::list<Token> &tokens) {
    if (tokens.empty() || tokens.front().type != TokenType::Return) return nullptr;

    tokens.pop_front(); // Consume the return keyword

    ASTNode* exp = parseExp(tokens);
    if (exp == nullptr) return nullptr;

    if (tokens.empty() || tokens.front().type != TokenType::Semicolon) return nullptr;

    tokens.pop_front(); // Consume the semicolon

    return new Return{dynamic_cast<Expression*>(exp)};
}

ASTNode* parseFunction(std::list<Token> &tokens) {
    // int
    if (tokens.empty() || tokens.front().type != TokenType::KeywordInt) return nullptr;
    tokens.pop_front();
    
    // main
    if (tokens.empty() || tokens.front().type != TokenType::Identifier) return nullptr;
    Token token_id = tokens.front();
    tokens.pop_front();

    // ( 
    if (tokens.empty() || tokens.front().type != TokenType::OpenParenthesis) return nullptr;


    tokens.pop_front();

    if (tokens.front().type == TokenType::Identifier && tokens.front().value == "void") tokens.pop_front();

    if (tokens.empty() || tokens.front().type != TokenType::CloseParenthesis) return nullptr;

    tokens.pop_front();
    if (tokens.empty() || tokens.front().type != TokenType::OpenBrace) return nullptr;
    tokens.pop_front();

    ASTNode* stmt = parseStatement(tokens);
    if (stmt == nullptr) return nullptr;

    if (tokens.empty() || tokens.front().type != TokenType::CloseBrace) return nullptr;
    tokens.pop_front(); // Consume the close brace

    return new Function{token_id.value, dynamic_cast<Statement*>(stmt)};

}

ASTNode* parseProgram(std::list<Token> &tokens) {
    return new Program{dynamic_cast<Function*>(parseFunction(tokens))};
}

ASTNode* parser(std::list<Token> &tokens) {
    return parseProgram(tokens);
}
