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


void Compiler::MakeIdentifier(Token& token)
{
    std::string name = std::string(token.mStart, token.mLength);
    mInstructions.emplace_back(Instruction{OpCode::GET_IDENTIFIER, name});
}

void Compiler::MakeNumber(Token& token)
{
    char *end = const_cast<char *>(token.mStart) + token.mLength;
    double value = std::strtod(token.mStart, &end);
    mInstructions.emplace_back(Instruction{OpCode::CONSTANT, value});
}

int Compiler::CompileArray()
{
    Consume(); // For the Left Bracket

    int valueCounter = 0;
    while (Peek().mTokenType != TokenType::RIGHT_BRACKET)
    {
        Token& currentToken = Consume();
        switch(currentToken.mTokenType)
        {
            case TokenType::COMMA:
            {
                break;
            }
            case TokenType::NUMBER:
            {
                MakeNumber(currentToken);
                ++valueCounter;
                break;
            }

            case TokenType::EOL:
            case TokenType::END:
            default:
                ThrowUnexpectedCharError();
                return 0;
        }
    }
    return valueCounter;
}

void Compiler::CompileExpression()
{
    auto binaryExpression = [&](OpCode code) 
    {
        Token& toDoBinary = Consume();
        if(toDoBinary.mTokenType == TokenType::IDENTIFIER)
        {
            MakeIdentifier(toDoBinary);
        }
        else if (toDoBinary.mTokenType == TokenType::NUMBER)
        {
            MakeNumber(toDoBinary);
        }
        else
        {
            // ERROR
            ThrowUnexpectedCharError();
            return;
        }

        mInstructions.emplace_back(Instruction{code});

    };

    while (Peek().mTokenType != TokenType::EOL && Peek().mTokenType != TokenType::END)
    {
        Token& currentToken = Consume();
        switch (currentToken.mTokenType)
        {
            case TokenType::IDENTIFIER:
                MakeIdentifier(currentToken);
                break;

            case TokenType::NUMBER:
                MakeNumber(currentToken);
                break;

            // Operations
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
            {
                ThrowUnexpectedCharError();
                return;
            }
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
                    if(tokenType == TokenType::LEFT_BRACKET)
                    {
                        int arrayLength = CompileArray();
                        mInstructions.emplace_back(Instruction{OpCode::CONSTANT, (double)arrayLength});
                        mInstructions.emplace_back(Instruction{OpCode::SET_IDENTIFIER_ARRAY, name});
                    }
                    else if (tokenType == TokenType::NUMBER || tokenType == TokenType::IDENTIFIER)
                    {
                        CompileExpression();
                        mInstructions.emplace_back(Instruction{OpCode::SET_IDENTIFIER_VALUE, name});
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
                if(Peek().mTokenType == TokenType::IDENTIFIER || Peek().mTokenType == TokenType::NUMBER)
                {
                    CompileExpression();
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
    std::string message = std::string("Compiler: Unexpected Char ") + token;
    mErrorReporting.LogError(Peek().mLine, message);
}
