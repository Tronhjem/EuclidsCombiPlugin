#pragma once

#include <vector>

#include "Token.h"
#include "Types.h"
#include "Instruction.h"

class ErrorReporting;

class Compiler
{
public:
    Compiler(std::vector<Token>& tokens, ErrorReporting& log) : mTokens(tokens), mErrorReporting(log) { }
    bool Compile(std::vector<Instruction>& runtimeInstructions);
//    std::vector<Instruction>& GetSetupInstructions() { return mSetupInstructions; }

private:
    Token& Consume();
    Token& Peek();
    Token& PeekNext();
    Token& Previous();
    
    inline void ThrowUnexpectedTokenError(Token& tokenForError);
    inline void ThrowMissingExpectedToken(std::string& missingToken);
    
    inline void MakeIdentifierGetter(Token& token, std::vector<Instruction>& instructions);
    inline void MakeConstant(Token& token, std::vector<Instruction>& instructions);

    bool CompileExpression(std::vector<Instruction>& instructions);
    bool CompileArray(std::vector<Instruction>& instructions, uChar& outLength);
    bool CompileEulclidSequence(std::vector<Instruction>& instructions);
    bool CompileTrack(std::vector<Instruction>& instructions);

    int mCurrentIndex = 0;
    std::vector<Token>& mTokens;
    ErrorReporting& mErrorReporting;
//    std::vector<Instruction> mSetupInstructions;
};
