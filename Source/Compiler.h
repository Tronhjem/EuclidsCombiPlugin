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
    bool Compile();
    std::vector<Instruction>& GetSetupInstructions() { return mSetupInstructions; }
    std::vector<Instruction>& GetRuntimeInstructions() { return mRuntimeInstructions; }

private:
    Token& Consume();
    Token& Peek();
    Token& PeekNext();
    inline void ThrowUnexpectedTokenError(Token& tokenForError);
    inline void MakeIdentifierGetter(Token& token, std::vector<Instruction>& instructions);
    inline void MakeConstant(Token& token, std::vector<Instruction>& instructions);

    bool CompileExpression(std::vector<Instruction>& instructions);
    bool CompileArray(uChar& outLength);
    bool CompileTrack();

    int mCurrentIndex = 0;
    std::vector<Token>& mTokens;
    ErrorReporting& mErrorReporting;
    std::vector<Instruction> mSetupInstructions;
    std::vector<Instruction> mRuntimeInstructions;
};
