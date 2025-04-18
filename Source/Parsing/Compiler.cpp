#include "Compiler.h"
#include "ErrorReporting.h"
#include "ScopedTimer.h"

Token& Compiler::Consume()
{
    return mTokens[mCurrentIndex++];
}

Token& Compiler::Peek()
{
    return mTokens[mCurrentIndex];
}

void Compiler::ParseExpression()
{
    auto makeIdentifier = [&](Token& token)
    {
        std::string name = std::string(token.mStart, token.mLength);
        mInstructions.emplace_back(Instruction{OpCode::GET_IDENTIFIER, name});
    };

    auto binaryExpression = [&](OpCode code) 
    {
        Token& toDoBinary = Consume();
        if(toDoBinary.mTokenType == TokenType::IDENTIFIER)
            makeIdentifier(toDoBinary);

        mInstructions.emplace_back(Instruction{code});
    };

    while (Peek().mTokenType != TokenType::EOL && Peek().mTokenType != TokenType::END)
    {
        Token& currentToken = Consume();
        switch (currentToken.mTokenType)
        {
            case TokenType::IDENTIFIER:
                makeIdentifier(currentToken);
                break;

            case TokenType::PLUS:
                binaryExpression(OpCode::ADD);
                break;

            case TokenType::MINUS:
                binaryExpression(OpCode::SUBTRACT);
                break;

            case TokenType::STAR:
                binaryExpression(OpCode::MULTIPLY);
                break;

            case TokenType::SLASH:
                binaryExpression(OpCode::DIVIDE);
                break;

            default:
                break;
        }
    }
}

bool Compiler::Compile()
{
    ScopedTimer timer("Compile");

    for (;;)
    {
        Token& token = Consume();

        switch(token.mTokenType)
        {
            case TokenType::IDENTIFIER:
            {
                std::string name = std::string(token.mStart, token.mLength);

                if (Peek().mTokenType == TokenType::EQUAL)
                {
                    Consume(); // consumes the equal sign

                    TokenType tokenType = Peek().mTokenType;
                    if (tokenType == TokenType::NUMBER)
                    {
                        Token& numberToken = Consume();
                        char *end = const_cast<char *>(numberToken.mStart) + numberToken.mLength;
                        double value = std::strtod(numberToken.mStart, &end);

                        mInstructions.emplace_back(Instruction{OpCode::CONSTANT, value});
                        mInstructions.emplace_back(Instruction{OpCode::SET_IDENTIFIER, name});

                    }
                    else if (tokenType == TokenType::IDENTIFIER)
                    {
                        ParseExpression();
                        mInstructions.emplace_back(Instruction{OpCode::SET_IDENTIFIER, name});
                    }
                    else
                    {
                        ThrowUnexpectedCharError();
                        return false;
                    }
                }

                break;
            }

            case TokenType::PRINT:
            {
                if(Peek().mTokenType == TokenType::IDENTIFIER)
                {
                    ParseExpression();
                    mInstructions.emplace_back(Instruction{OpCode::PRINT});
                }
                else
                {
                    ThrowUnexpectedCharError();
                    return false;
                }
                break;
            }

            case TokenType::END:
                mInstructions.emplace_back(Instruction{OpCode::END});
                return true;

            case TokenType::EOL:
                break;

            default:
                ThrowUnexpectedCharError();
                return false;
        }
    }
}

void Compiler::ThrowUnexpectedCharError()
{
    const Token &tokenForError = Peek();
    std::string token = std::string(tokenForError.mStart, tokenForError.mLength);
    std::string message = std::string("Unexpected Char ") + token;
    mErrorReporting.LogError(Peek().mLine, message);
}
