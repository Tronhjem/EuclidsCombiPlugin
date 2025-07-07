#pragma once

#include <vector>
#include <unordered_map>

#include "Token.h"
#include "Types.h"
#include "Instruction.h"
#include "StoredFunction.h"

class ErrorReporting;

class Compiler
{
public:
    Compiler(std::vector<Token>& tokens, ErrorReporting& log) : mTokens(tokens), mErrorReporting(log)
    {
        // populate built in functions
        std::vector<Instruction> printInstructions;
        printInstructions.emplace_back(Instruction{ OpCode::PRINT });
        mFunctions["print"] = StoredFunction(1, printInstructions);
        
        std::vector<Instruction> noteInstructions;
        noteInstructions.emplace_back(Instruction{ OpCode::NOTE });
        mFunctions["note"] = StoredFunction(4, noteInstructions);
        
        std::vector<Instruction> ccInstructions;
        ccInstructions.emplace_back(Instruction{ OpCode::CC});
        mFunctions["cc"] = StoredFunction(4, ccInstructions);
        
        std::vector<Instruction> ranInstructions;
        ranInstructions.emplace_back(Instruction{OpCode::GET_RANDOM_IN_RANGE});
        mFunctions[ranFunctionName] = StoredFunction(2, ranInstructions);
        
        std::vector<Instruction> eucInstructions;
        eucInstructions.emplace_back(Instruction{OpCode::GENERATE_EUCLID_SEQUENCE});
        mFunctions[eucFunctionName] = StoredFunction(2, eucInstructions);
    }
    
    bool Compile(std::vector<Instruction>& runtimeInstructions);

private:
    Token& Consume();
    Token& Peek();
    Token& PeekNext();
    Token& Previous();
    
    inline void ThrowUnexpectedTokenError(Token& tokenForError);
    inline void ThrowMissingExpectedToken(std::string& missingToken);
    inline void ThrowMissingParamCount(int expected, int received);
    inline void ThrowUnexpectedEnd(std::string& missingToken);
    
    inline bool MakeIdentifierGetter(Token& token, std::vector<Instruction>& instructions);
    inline void MakeConstant(Token& token, std::vector<Instruction>& instructions);
    inline void MakeOperation(TokenType tokenType, std::vector<Instruction>& instructions);

    bool CompileExpression(std::vector<Instruction>& instructions);
    bool CompileArray(std::vector<Instruction>& instructions, uChar& outLength);
    bool CompileFunctionCall(std::vector<Instruction>& instructions, std::string& functionName);

    int mCurrentIndex = 0;
    std::vector<Token>& mTokens;
    ErrorReporting& mErrorReporting;
    std::unordered_map<std::string, StoredFunction> mFunctions;
    std::string ranFunctionName = "ran";
    std::string eucFunctionName = "euc";
};
