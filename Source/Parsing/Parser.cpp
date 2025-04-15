// #include "Parser.h"
// #include <memory>

// std::shared_ptr<Expr> Parser::Expression()
// {
//     return Equality();
// }

// std::shared_ptr<Expr> Parser::Equality()
// {
//     std::shared_ptr<Expr> expr = Comparison();

//     const int length = 2;
//     TokenType tokenTypes[length] = { TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL };

//     while (Match(tokenTypes, length))
//     {
//         Token operation = Previous();
//         std::shared_ptr<Expr> right = Comparison();
//         expr = std::make_shared<Binary>();
//     }
// }

// std::shared_ptr<Expr> Parser::Comparison()
// {

// }

// bool Parser::Match(TokenType* start, int length)
// {

// }

// bool Parser::Check()
// {

// }

// bool Parser::IsAtEnd()
// {

// }

// Token Parser::Peek()
// {

// }

// Token Parser::Previous()
// {

// }