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
        mFunctions["print"] = StoredFunction("print", 1, printInstructions);
    }
    
    bool Compile(std::vector<Instruction>& runtimeInstructions);
//    std::vector<Instruction>& GetSetupInstructions() { return mSetupInstructions; }

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

    bool MakeFunctionCall(std::vector<Instruction>& instructions, int expectedParamCount);

    bool CompileExpression(std::vector<Instruction>& instructions);
    bool CompileArray(std::vector<Instruction>& instructions, uChar& outLength);
    bool CompileEulclidSequence(std::vector<Instruction>& instructions);
    bool CompileTrack(std::vector<Instruction>& instructions);
    bool CompileRandom(std::vector<Instruction>& instructions);

    int mCurrentIndex = 0;
    std::vector<Token>& mTokens;
    ErrorReporting& mErrorReporting;
    std::unordered_map<std::string, StoredFunction> mFunctions;
//    std::vector<Instruction> mSetupInstructions;
};
