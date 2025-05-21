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
        
        CompileExpression(instructions);
        
//        Token& indexToken = Consume();
//        if(indexToken.mTokenType == TokenType::NUMBER)
//        {
//            MakeConstant(indexToken, instructions);
//        }
//        else if (indexToken.mTokenType == TokenType::IDENTIFIER)
//        {
//            MakeIdentifierGetter(indexToken, instructions);
//        }
//        else
//        {
//            ThrowUnexpectedTokenError(indexToken);
//            return;
//        }
        
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

void Compiler::MakeOperation(TokenType tokenType, std::vector<Instruction> &instructions)
{
    OpCode code = OpCode::END;
    switch (tokenType)
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
        case TokenType::GREATER:
            code = OpCode::GREATER;
            break;
        case TokenType::GREATER_EQUAL:
            code = OpCode::GREATER_EQUAL;
            break;
        case TokenType::LESS:
            code = OpCode::LESS;
            break;
        case TokenType::LESS_EQUAL:
            code = OpCode::LESS_EQUAL;
            break;
        case TokenType::EQUAL_EQUAL:
            code = OpCode::EQUAL;
        default:
            break;
    }
        
    instructions.emplace_back(Instruction{code});
}

bool Compiler::CompileArray(std::vector<Instruction>& instructions, uChar& outLength)
{
    Consume(); // For the Left Bracket

    if(Peek().mTokenType != TokenType::NUMBER &&
       Peek().mTokenType != TokenType::IDENTIFIER &&
       Peek().mTokenType != TokenType::RANDOM)
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
                CompileExpression(instructions);
                ++valueCounter;
                break;
            }
                
            case TokenType::RANDOM:
            {
                CompileRandom(instructions);
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

bool Compiler::CompileEulclidSequence(std::vector<Instruction>& instructions)
{
    Consume(); // for Left brace

    if(Peek().mTokenType != TokenType::NUMBER && Peek().mTokenType != TokenType::IDENTIFIER)
    {
        ThrowUnexpectedTokenError(Peek());
        return false;
    }

    // expression for hits
    CompileExpression(instructions);

    if(Consume().mTokenType != TokenType::COMMA)
    {
        ThrowUnexpectedTokenError(Peek());
        return false;
    }

    // expression for Length
    CompileExpression(instructions);

    if(Peek().mTokenType != TokenType::RIGHT_BRACE)
    {
        std::string missingToken{"}"};
        ThrowMissingExpectedToken(missingToken);
        return false;
    }
    
    Consume(); // For Right Brace
    
    return true;
}

bool Compiler::CompileRandom(std::vector<Instruction>& instructions)
{
    Consume(); // for Left brace

    if(Peek().mTokenType != TokenType::NUMBER &&
       Peek().mTokenType != TokenType::IDENTIFIER)
    {
        ThrowUnexpectedTokenError(Peek());
        return false;
    }

    // expression for hits
    CompileExpression(instructions);

    if(Consume().mTokenType != TokenType::COMMA)
    {
        ThrowUnexpectedTokenError(Peek());
        return false;
    }

    // expression for Length
    CompileExpression(instructions);

    if(Peek().mTokenType != TokenType::RIGHT_BRACE)
    {
        std::string missingToken{"}"};
        ThrowMissingExpectedToken(missingToken);
        return false;
    }
    
    Consume(); // For Right Brace
    
    instructions.emplace_back(Instruction{OpCode::GET_RANDOM_IN_RANGE});
    return true;
}

bool Compiler::CompileExpression(std::vector<Instruction>& instructions)
{
    auto isOperator = [&](TokenType t) -> bool
    {
        return  t == TokenType::PLUS ||
            t == TokenType::MINUS ||
            t == TokenType::STAR  ||
            t == TokenType::SLASH ||
            t == TokenType::AND   ||
            t == TokenType::OR    ||
            t == TokenType::XOR   ||
            t == TokenType::GREATER ||
            t == TokenType::GREATER_EQUAL ||
            t == TokenType::LESS ||
            t == TokenType::LESS_EQUAL ||
            t == TokenType::EQUAL_EQUAL;
    };
    
    auto precedence = [&](TokenType t) -> int
    {
        if(t == TokenType::PLUS || t == TokenType::MINUS)
            return 1;
        if(t == TokenType::STAR || t == TokenType::SLASH)
            return 2;
        
        return 0;
    };
    
    std::stack<TokenType> ops;
    
    while (Peek().mTokenType != TokenType::EOL &&
           Peek().mTokenType != TokenType::END &&
           Peek().mTokenType != TokenType::COMMA &&
           Peek().mTokenType != TokenType::RIGHT_BRACKET &&
           Peek().mTokenType != TokenType::RIGHT_BRACE)
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
        
        else if(tType == TokenType::RANDOM)
        {
            CompileRandom(instructions);
        }
        
        else if(isOperator(tType))
        {
            while (!ops.empty() && isOperator(ops.top()))
            {
                TokenType top = ops.top();
                
                if ((precedence(top) > precedence(tType)) ||
                    (precedence(top) == precedence(tType)))
                {
                    MakeOperation(top, instructions);
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
                MakeOperation(ops.top(), instructions);
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
        MakeOperation(ops.top(), instructions);
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
        
    int paramCounter = 0;
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
                ++paramCounter;
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
    
    if(paramCounter < 4 || paramCounter > 4)
    {
        ThrowMissingParamCount(4, paramCounter);
        return false;
    }
    
    if(Previous().mTokenType != TokenType::RIGHT_PAREN)
    {
        std::string missingToken{")"};
        ThrowMissingExpectedToken(missingToken);
        return false;
    }
        
    return true;
}

bool Compiler::Compile(std::vector<Instruction>& instructions)
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
                        if(CompileArray(instructions, arrayLength))
                        {
                            instructions.emplace_back(Instruction{OpCode::CONSTANT, arrayLength});
                            instructions.emplace_back(Instruction{OpCode::SET_IDENTIFIER_ARRAY, name});
                        }
                        else
                        {
                            return false;
                        }
                    }
                    // Euclid Sequence
                    else if (tokenType == TokenType::LEFT_BRACE)
                    {
                        if(CompileEulclidSequence(instructions))
                        {
                            instructions.emplace_back(Instruction{OpCode::GENERATE_EUCLID_SEQUENCE, name});
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else if (tokenType == TokenType::RANDOM)
                    {
                        Consume();
                        if(CompileRandom(instructions))
                        {
                            instructions.emplace_back(Instruction{OpCode::SET_IDENTIFIER_VALUE, name});
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else if (tokenType == TokenType::NUMBER ||
                             tokenType == TokenType::IDENTIFIER)
                    {
                        if(CompileExpression(instructions))
                        {
                            instructions.emplace_back(Instruction{OpCode::SET_IDENTIFIER_VALUE, name});
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
                    CompileExpression(instructions);
                    instructions.emplace_back(Instruction{OpCode::PRINT});
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
                    if(CompileTrack(instructions))
                    {
                        instructions.emplace_back(Instruction{OpCode::NOTE});
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
                    if(CompileTrack(instructions))
                    {
                        instructions.emplace_back(Instruction{OpCode::CC});
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
                instructions.emplace_back(Instruction{OpCode::END});
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

void Compiler::ThrowMissingParamCount(int expected, int received)
{
    std::string message =   std::string("Expected ") +
                            std::to_string(expected) +
                            std::string(" but received ") +
                            std::to_string(received);
    
    mErrorReporting.LogError(Peek().mLine, message);
}
