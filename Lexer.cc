#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include "Token.h"

std::list<Token> lexer(const std::string& filename)
{
    std::list<Token> tokens;
    std::fstream readfile{filename};

    // Check if the file was successfully opened
    if (!readfile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return tokens;  // Return empty list if the file cannot be opened
    }

    std::string line;
    while (getline(readfile, line))
    {
        // tokenize each line
        for (int i = 0; i < (int)line.size(); i++) {
            // ignore comments
            if (i + 1 < (int)line.size() && line[i] == '/' && line[i + 1] == '/') 
                break;

            if (std::isspace(line[i])) continue;

            // parenthesis && braces
            if (line[i] == '{')
                tokens.emplace_back(Token(TokenType::OpenBrace, "{"));
            else if (line[i] == '}')
                tokens.emplace_back(Token(TokenType::CloseBrace, "}"));
            else if (line[i] == '(')
                tokens.emplace_back(Token(TokenType::OpenParenthesis, "("));
            else if (line[i] == ')')
                tokens.emplace_back(Token(TokenType::CloseParenthesis, ")"));
            else if (line[i]== ';') 
                tokens.emplace_back(Token(TokenType::Semicolon, ";"));
            // Keyword int
            else if (i + 2 < (int)line.size() && line.substr(i, 3) == "int" &&
                     (i + 3 == (int)line.size() || !std::isalnum(line[i + 3])))
            {
                tokens.emplace_back(Token(TokenType::KeywordInt, "int"));
                i += 2;
            }
            // Keyword return
            else if (i + 5 < (int)line.size() && line.substr(i, 6) == "return" &&
                     (i + 6 == (int)line.size() || !std::isalnum(line[i + 6])))
            {
                tokens.emplace_back(Token(TokenType::Return, "return"));
                i += 5;
            }
            // Identifiers and Int literals
            else if (std::isalpha(line[i]) || std::isdigit(line[i])) {
                int j = i;
                if (std::isalpha(line[i])) {
                    while (j < (int)line.size() && std::isalnum(line[j])) j++;
                    tokens.emplace_back(Token(TokenType::Identifier, line.substr(i, j - i)));
                }
                else {
                    while (j < (int)line.size() && std::isdigit(line[j])) j++;
                    tokens.emplace_back(Token(TokenType::Intlit, line.substr(i, j - i)));
                }
                i = j - 1;
            }
        }
    }
    return tokens;
}