#pragma once

#include <vector>
#include "Token.h"
#include "Types.h"

class ErrorReporting;

enum class OpCode : uChar
{
    CONSTANT = 0,
    GET_IDENTIFIER_VALUE,
    GET_IDENTIFIER_WITH_INDEX,
    SET_IDENTIFIER_VALUE,
    SET_IDENTIFIER_ARRAY,

    // Operations
    // Math
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    // Logical
    AND,
    OR,
    XOR,

    // Specific
    TRACK,
    // Built in Functions
    PRINT,
    END
};

class Instruction {
public:
    explicit Instruction(OpCode code) : opCode(code) {}
    explicit Instruction(OpCode code, uChar value) : opCode(code), mDataValue(value) {}
    explicit Instruction(OpCode code, std::string name) : opCode(code), mNameValue(name) {}

    OpCode opCode;
    uChar mDataValue = 0.0;
    std::string mNameValue;
};

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
