#include "Token.h"
#include <string>
#include <iostream>
#include <list>
#include "tree.h"
#include <sstream>

Node* parseExp(std::list<Token> &tokens) {
    if (tokens.empty() || tokens.front().type != TokenType::Intlit ) {
        std::cerr << "Missing return expression" << std::endl;
        return nullptr;
    }
    std::stringstream iss{tokens.front().value};
    int i;
    iss >> i;
    tokens.pop_front();
    
    return new Constant{i};
}   

Node* parseStatement(std::list<Token> &tokens) {
    if (tokens.empty() || tokens.front().type != TokenType::Return) {
        std::cerr << "Error, return missing" << std::endl;
        return nullptr;
    }

    tokens.pop_front(); // Consume the return keyword

    Node* exp = parseExp(tokens);
    if (exp == nullptr) return nullptr;

    if (tokens.empty() || tokens.front().type != TokenType::Semicolon) {
        std::cerr << "Missing ;" << std::endl;
        return nullptr;
    }

    tokens.pop_front(); // Consume the semicolon

    return new Return{exp};
}

Node* parseFunction(std::list<Token> &tokens) {
    // int
    if (tokens.empty() || tokens.front().type != TokenType::KeywordInt) {
        std::cerr << "Missing int keyword" << std::endl;
        return nullptr;
    }
    tokens.pop_front();
    
    // main
    if (tokens.empty() || tokens.front().type != TokenType::Identifier) {
        std::cerr << "Missing main keyword" << std::endl;
        return nullptr;
    }
    Token token_id = tokens.front();
    tokens.pop_front();

    // ( 
    if (tokens.empty() || tokens.front().type != TokenType::OpenParenthesis) {
        std::cerr << "Missing ( after main" << std::endl;
        return nullptr;
    }


    tokens.pop_front();

    if (tokens.front().type == TokenType::Identifier && tokens.front().value == "void") tokens.pop_front();

    if (tokens.empty() || tokens.front().type != TokenType::CloseParenthesis) {
        std::cerr << "Missing closing parenthesis" << std::endl;
        return nullptr;
    }

    tokens.pop_front();
    if (tokens.empty() || tokens.front().type != TokenType::OpenBrace) {
        std::cerr << "Missing { for function" << std::endl;
        return nullptr;
    }
    tokens.pop_front();

    Node* stmt = parseStatement(tokens);
    if (stmt == nullptr) return nullptr;

    if (tokens.empty() || tokens.front().type != TokenType::CloseBrace) {
        std::cerr << "Missing }" << std::endl;
        return nullptr;
    }
    tokens.pop_front(); // Consume the close brace

    return new Function{dynamic_cast<Statement*>(stmt),token_id.value};

}

Node* parseProgram(std::list<Token> &tokens) {
    return new Program{dynamic_cast<Function*>(parseFunction(tokens))};
}

Node* parser(std::list<Token> &tokens) {
    return parseProgram(tokens);
}
