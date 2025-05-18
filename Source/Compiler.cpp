#include <string>
#include <cassert>
#include <stack>

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

Token& Compiler::Previous()
{
    assert(mCurrentIndex < mTokens.size());
    return mTokens[mCurrentIndex - 1];
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
            case TokenType::LEFT_PAREN:
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
    
    if(Consume().mTokenType != TokenType::RIGHT_BRACKET)
    {
        std::string missingToken {"]"};
        ThrowMissingExpectedToken(missingToken);
        return false;
    }
    
    outLength = (uChar) valueCounter;
    return true;
}

bool Compiler::CompileEulclidSequence()
{
    Consume(); // for Left Paren
    
    if(Peek().mTokenType != TokenType::NUMBER && Peek().mTokenType != TokenType::IDENTIFIER)
    {
        ThrowUnexpectedTokenError(Peek());
        return false;
    }
    
    // expression for hits
    CompileExpression(mSetupInstructions);
    
    if(Consume().mTokenType != TokenType::COMMA)
    {
        ThrowUnexpectedTokenError(Peek());
        return false;
    }
    
    // expression for Length
    CompileExpression(mSetupInstructions);
    
    if(Consume().mTokenType != TokenType::RIGHT_PAREN)
    {
        std::string missingToken{")"};
        ThrowMissingExpectedToken(missingToken);
        return false;
    }
    
    return true;
}

bool Compiler::CompileExpression(std::vector<Instruction>& instructions)
{
    // LAMDAS
    auto makeOperation = [&](TokenType t)
    {
        OpCode code = OpCode::END;
        switch (t)
        {
            case TokenType::PLUS:
                code = OpCode::ADD;
                break;
            case TokenType::MINUS:
                code = OpCode::SUBTRACT;
                break;
            case TokenType::STAR:
                code = OpCode::MULTIPLY;
                break;
            case TokenType::SLASH:
                code = OpCode::DIVIDE;
                break;
            case TokenType::AND:
                code = OpCode::AND;
                break;
            case TokenType::OR:
                code = OpCode::OR;
                break;
            case TokenType::XOR:
                code = OpCode::XOR;
                break;
            default:
                break;
        }
        
        instructions.emplace_back(Instruction{code});
    };
    
    auto isOperator = [&](TokenType t) -> bool
    {
        return  t == TokenType::PLUS  ||
                t == TokenType::MINUS ||
                t == TokenType::STAR  ||
                t == TokenType::SLASH ||
                t == TokenType::AND   ||
                t == TokenType::OR    ||
                t == TokenType::XOR;
    };
    
    auto precedence = [&](TokenType t) -> int
    {
        if(t == TokenType::PLUS || t == TokenType::MINUS)
            return 1;
        if(t == TokenType::STAR || t == TokenType::SLASH)
            return 2;
        
        return 0;
    };
    
    //LAMDAS END
    
    std::stack<TokenType> ops;
    
    while (Peek().mTokenType != TokenType::EOL &&
           Peek().mTokenType != TokenType::END &&
           Peek().mTokenType != TokenType::COMMA &&
           Peek().mTokenType != TokenType::RIGHT_BRACKET)
    {
        Token& currentToken = Consume();
        TokenType tType = currentToken.mTokenType;
        
        if(tType == TokenType::IDENTIFIER)
        {
            MakeIdentifierGetter(currentToken, instructions);
        }
        
        else if(tType == TokenType::NUMBER)
        {
            MakeConstant(currentToken, instructions);
        }
        
        else if(isOperator(tType))
        {
            while (!ops.empty() && isOperator(ops.top()))
            {
                TokenType top = ops.top();
                
                if ((precedence(top) > precedence(tType)) ||
                    (precedence(top) == precedence(tType)))
                {
                    makeOperation(top);
                    ops.pop();
                }
                else
                {
                    break;
                }
            }
            
            ops.push(tType);
        }
        
        else if(tType == TokenType::LEFT_PAREN)
        {
            ops.push(tType);
        }
        
        else if(tType == TokenType::RIGHT_PAREN)
        {
            while (!ops.empty() && ops.top() != TokenType::LEFT_PAREN)
            {
                makeOperation(ops.top());
                ops.pop();
            }
            if (!ops.empty() && ops.top() == TokenType::LEFT_PAREN)
            {
                ops.pop(); // discard left paren
            }
        }
        else
        {
            ThrowUnexpectedTokenError(currentToken);
            return false;
        }
    }
    
    while (!ops.empty())
    {
        makeOperation(ops.top());
        ops.pop();
    }
    
    return true;
}

bool Compiler::CompileTrack(std::vector<Instruction>& runtimeInstructions)
{
    Consume(); // For the Left Brace
    
    if(Peek().mTokenType != TokenType::NUMBER && Peek().mTokenType != TokenType::IDENTIFIER)
    {
        ThrowUnexpectedTokenError(Peek());
        return false;
    }
        
    while (Peek().mTokenType != TokenType::RIGHT_PAREN &&
           Peek().mTokenType != TokenType::EOL &&
           Peek().mTokenType != TokenType::END)
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
            case TokenType::LEFT_PAREN:
            {
                CompileExpression(runtimeInstructions);
                break;
            }

            case TokenType::END:
            {
                if(Previous().mTokenType != TokenType::RIGHT_PAREN)
                {
                    std::string missingToken{")"};
                    ThrowMissingExpectedToken(missingToken);
                    return false;
                }
                break;
            }
                
            default:
            {
                ThrowUnexpectedTokenError(Peek());
                return false;
            }
        }
    }
    
    if(Previous().mTokenType != TokenType::RIGHT_PAREN)
    {
        std::string missingToken{")"};
        ThrowMissingExpectedToken(missingToken);
        return false;
    }
        
    return true;
}

bool Compiler::Compile(std::vector<Instruction>& runtimeInstructions)
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
                    // Data Array
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
                    // Euclid Sequence
                    else if (tokenType == TokenType::LEFT_PAREN)
                    {
                        if(CompileEulclidSequence())
                        {
                            mSetupInstructions.emplace_back(Instruction{OpCode::GENERATE_EUCLID_SEQUENCE, name});
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
                        ThrowUnexpectedTokenError(Peek());
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
                
            case TokenType::NOTE:
            {
                if(Peek().mTokenType == TokenType::LEFT_PAREN)
                {
                    if(CompileTrack(runtimeInstructions))
                    {
                        runtimeInstructions.emplace_back(Instruction{OpCode::NOTE});
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
                
            case TokenType::CC:
            {
                if(Peek().mTokenType == TokenType::LEFT_PAREN)
                {
                    if(CompileTrack(runtimeInstructions))
                    {
                        runtimeInstructions.emplace_back(Instruction{OpCode::CC});
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
                runtimeInstructions.emplace_back(Instruction{OpCode::END});
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
    std::string message = std::string("Compiler: Unexpected Character ") + token;
    mErrorReporting.LogError(Peek().mLine, message);
}

void Compiler::ThrowMissingExpectedToken(std::string& missingToken)
{
    std::string message = std::string("Compiler: Missing a ") + missingToken;
    mErrorReporting.LogError(Peek().mLine, message);
}
