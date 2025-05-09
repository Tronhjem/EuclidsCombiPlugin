#include <string>

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

void Compiler::MakeIdentifierGetter(Token& token, std::vector<Instruction>& instructions)
{
    std::string name = std::string(token.mStart, token.mLength);
    if (Peek().mTokenType == TokenType::LEFT_BRACKET)
    {
        Consume(); // for consuming left bracket
        
        Token& indexToken = Consume();
        MakeConstant(indexToken, instructions);
        
        Consume(); // for consuming right bracket

        instructions.emplace_back(Instruction{OpCode::GET_IDENTIFIER_WITH_INDEX, name});
    }
    else
    {
        instructions.emplace_back(Instruction{OpCode::GET_IDENTIFIER_VALUE, name});
    }
}

void Compiler::MakeConstant(Token& token, std::vector<Instruction>& instructions)
{
    int value = std::stoi(std::string(token.mStart, token.mLength));
    if (value > 255)
    {
        value = 255;
        std::string message = std::string("Compiler: value can only be between 0 and 255, correcting to 255");
        mErrorReporting.LogWarning(message);
    }
    if (value < 0)
    {
        value = 0;
        std::string message = std::string("Compiler: value can only be between 0 and 255, correcting to 0");
        mErrorReporting.LogWarning(message);
    }
    
    instructions.emplace_back(Instruction{OpCode::CONSTANT, (uChar)value});
}

bool Compiler::CompileArray(uChar& outLength)
{
    Consume(); // For the Left Bracket

    if(Peek().mTokenType != TokenType::NUMBER && Peek().mTokenType != TokenType::IDENTIFIER)
    {
        ThrowUnexpectedTokenError(Peek());
        return false;
    }
    
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
                CompileExpression(mSetupInstructions);
                ++valueCounter;
                break;
            }

            case TokenType::EOL:
            case TokenType::END:
            default:
                ThrowUnexpectedTokenError(currentToken);
                return false;
        }
    }
    Consume(); // Consuming right bracket. 
    
    outLength = (uChar) valueCounter;
    return true;
}

bool Compiler::CompileExpression(std::vector<Instruction>& instructions)
{
    auto binaryExpression = [&](OpCode code) -> bool
    {
        Token& toDoBinary = Consume();
        if(toDoBinary.mTokenType == TokenType::IDENTIFIER)
        {
            MakeIdentifierGetter(toDoBinary, instructions);
        }
        else if (toDoBinary.mTokenType == TokenType::NUMBER)
        {
            MakeConstant(toDoBinary, instructions);
        }
        else
        {
            // ERROR
            ThrowUnexpectedTokenError(toDoBinary);
            return false;
        }

        instructions.emplace_back(Instruction{code});
        return true;
    };
    
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
            {
                MakeIdentifierGetter(currentToken, instructions);
                break;
            }

            case TokenType::NUMBER:
            {
                MakeConstant(currentToken, instructions);
                break;
            }

            // Operations
            case TokenType::PLUS:
            {
                binaryExpression(OpCode::ADD);
                break;
            }

            case TokenType::MINUS:
            {
                binaryExpression(OpCode::SUBTRACT);
                break;
            }

            case TokenType::STAR:
            {
                binaryExpression(OpCode::MULTIPLY);
                break;
            }
                
            case TokenType::SLASH:
            {
                binaryExpression(OpCode::DIVIDE);
                break;
            }
                
            case TokenType::AND:
            {
                binaryExpression(OpCode::AND);
                break;
            }
                
            case TokenType::OR:
            {
                binaryExpression(OpCode::OR);
                break;
            }
                
            case TokenType::XOR:
            {
                binaryExpression(OpCode::XOR);
                break;
            }
                
            default:
            {
                ThrowUnexpectedTokenError(currentToken);
                return false;
            }
        }
    }
    
    return true;
}

bool Compiler::CompileTrack()
{
    Consume(); // For the Left Brace
    
    if(Peek().mTokenType != TokenType::NUMBER && Peek().mTokenType != TokenType::IDENTIFIER)
    {
        ThrowUnexpectedTokenError(Peek());
        return false;
    }
        
    while (Peek().mTokenType != TokenType::RIGHT_PAREN)
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
                CompileExpression(mRuntimeInstructions);
                break;
            }

            case TokenType::EOL:
            case TokenType::END:
            default:
                ThrowUnexpectedTokenError(currentToken);
                return false;
        }
    }
    
    Consume(); // Consuming right bracket.
    return true;
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
                        uChar arrayLength = 0;
                        if(CompileArray(arrayLength))
                        {
                            mSetupInstructions.emplace_back(Instruction{OpCode::CONSTANT, arrayLength});
                            mSetupInstructions.emplace_back(Instruction{OpCode::SET_IDENTIFIER_ARRAY, name});
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else if (tokenType == TokenType::NUMBER || tokenType == TokenType::IDENTIFIER)
                    {
                        if(CompileExpression(mSetupInstructions))
                        {
                            mSetupInstructions.emplace_back(Instruction{OpCode::SET_IDENTIFIER_VALUE, name});
                        }
                        else
                        {
                            return false;
                        }
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
                    CompileExpression(mSetupInstructions);
                    mSetupInstructions.emplace_back(Instruction{OpCode::PRINT});
                }
                else
                {
                    ThrowUnexpectedTokenError(Peek());
                    return false;
                }
                break;
            }
                
            case TokenType::TRACK:
            {
                if(Peek().mTokenType == TokenType::LEFT_PAREN)
                {
                    if(CompileTrack())
                    {
                        mRuntimeInstructions.emplace_back(Instruction{OpCode::TRACK});
                    }
                    else
                    {
                        ThrowUnexpectedTokenError(Peek());
                        return false;
                    }
                }
                else
                {
                    ThrowUnexpectedTokenError(Peek());
                }
                break;
            }

            case TokenType::END:
                mSetupInstructions.emplace_back(Instruction{OpCode::END});
                mRuntimeInstructions.emplace_back(Instruction{OpCode::END});
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
