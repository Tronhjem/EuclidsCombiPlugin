// #pragma once

// #include <vector>
// #include "Token.h"
// #include "Expr.h"

// class Parser
// {
// public:
//     Parser(std::vector<Token> tokens) : mTokens(tokens)
//     {
//     }


// private:
//     std::shared_ptr<Expr> Expression();
//     std::shared_ptr<Expr> Equality();
//     std::shared_ptr<Expr> Comparison();

//     bool Match(TokenType *start, int length);
//     bool Check();
//     bool IsAtEnd();
//     Token Peek();
//     Token Previous();
    
//     std::vector<Token> mTokens;
//     int current = 0;
// };
