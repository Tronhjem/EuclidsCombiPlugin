#include "Compiler.h"
#include "ErrorReporting.h"
#include "ScopedTimer.h"

Token& Compiler::Consume()
{
    return mTokens[mCurrentIndex++];
}

Token& Compiler::Peek()
{
    assert(mCurrentIndex < mTokens.size());
    return mTokens[mCurrentIndex];
}

void Compiler::MakeIdentifierGetter(Token& token)
{
    std::string name = std::string(token.mStart, token.mLength);
    if (Peek().mTokenType == TokenType::LEFT_BRACKET)
    {
        Consume(); // for consuming left bracket
        Token& indexToken = Consume();
        MakeConstant(indexToken);
        Consume(); // for consuming right bracket

        mInstructions.emplace_back(Instruction{OpCode::GET_IDENTIFIER_WITH_INDEX, name});
    }
    else
    {
        mInstructions.emplace_back(Instruction{OpCode::GET_IDENTIFIER, name});
    }
}

void Compiler::MakeConstant(Token& token)
{
    char* end = const_cast<char *>(token.mStart) + token.mLength;
    double value = std::strtod(token.mStart, &end);
    mInstructions.emplace_back(Instruction{OpCode::CONSTANT, value});
}

int Compiler::CompileArray()
{
    Consume(); // For the Left Bracket

    int valueCounter = 0;
    while (Peek().mTokenType != TokenType::RIGHT_BRACKET)
    {
        Token& currentToken = Peek();
        switch(currentToken.mTokenType)
        {
            case TokenType::COMMA:
            {
                Consume();
                break;
            }

            case TokenType::NUMBER:
            case TokenType::IDENTIFIER:
            {
                CompileExpression();
                ++valueCounter;
                break;
            }

            case TokenType::EOL:
            case TokenType::END:
            default:
                ThrowUnexpectedTokenError(currentToken);
                return 0;
        }
    }
    Consume(); // Consuming right bracket. 
    
    return valueCounter;
}

void Compiler::CompileExpression()
{
    auto binaryExpression = [&](OpCode code) 
    {
        Token& toDoBinary = Consume();
        if(toDoBinary.mTokenType == TokenType::IDENTIFIER)
        {
            MakeIdentifierGetter(toDoBinary);
        }
        else if (toDoBinary.mTokenType == TokenType::NUMBER)
        {
            MakeConstant(toDoBinary);
        }
        else
        {
            // ERROR
            ThrowUnexpectedTokenError(toDoBinary);
            return;
        }

        mInstructions.emplace_back(Instruction{code});
    };
    
    Token& debug = Peek();
    while (Peek().mTokenType != TokenType::EOL && 
           Peek().mTokenType != TokenType::END &&
           Peek().mTokenType != TokenType::COMMA &&
           Peek().mTokenType != TokenType::RIGHT_BRACKET &&
           Peek().mTokenType != TokenType::RIGHT_PAREN)
    {
        Token& currentToken = Consume();
        switch (currentToken.mTokenType)
        {
            case TokenType::IDENTIFIER:
                MakeIdentifierGetter(currentToken);
                break;

            case TokenType::NUMBER:
                MakeConstant(currentToken);
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
                ThrowUnexpectedTokenError(currentToken);
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
                        ThrowUnexpectedTokenError(token);
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
                    ThrowUnexpectedTokenError(token);
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
                ThrowUnexpectedTokenError(token);
                return false;
        }
    }
}

void Compiler::ThrowUnexpectedTokenError(Token& tokenForError)
{
    std::string token = std::string(tokenForError.mStart, tokenForError.mLength);
    std::string message = std::string("Compiler: Unexpected Token ") + token;
    mErrorReporting.LogError(Peek().mLine, message);
}
