#pragma once
#include <vector>
#include "Token.h"

class ErrorReporting;

enum class OpCode : int
{
    CONSTANT = 0,
    GET_IDENTIFIER = 1,
    SET_IDENTIFIER = 2,

    // Operations
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,

    // Built in Functions
    PRINT,
    END
};

class Instruction {
public:
    explicit Instruction(OpCode code) : opCode(code) {}
    explicit Instruction(OpCode code, int value) : opCode(code), mIntValue(value) {}
    explicit Instruction(OpCode code, double value) : opCode(code), mDoubleValue(value) {}
    explicit Instruction(OpCode code, std::string name) : opCode(code), mNameValue(name) {}

    OpCode opCode;
    // union {
    int mIntValue = 0;
    double mDoubleValue = 0.0;
    std::string mNameValue;
    // };

    // Instruction(Instruction& instruction)
    // {
    // }

    // Instruction(Instruction&& instruction)
    // {
    //     opCode = instruction.opCode;
    //     mCharData = instruction.mCharData;
    //     mIntValue = instruction.mIntValue;
    //     mDoubleValue = instruction.mDoubleValue;

    //     instruction.mCharData = nullptr;
    // }
};

class Compiler
{
public:
    Compiler(std::vector<Token>& tokens, ErrorReporting& log) : mTokens(tokens), mErrorReporting(log) { }
    bool Compile();
    std::vector<Instruction>& GetInstructions() { return mInstructions; }

private:
    Token& Consume();
    Token& Peek();
    void CompileExpression();
    void ThrowUnexpectedCharError();

    int mCurrentIndex = 0;
    std::vector<Token>& mTokens;
    ErrorReporting& mErrorReporting;
    std::vector<Instruction> mInstructions;
};
