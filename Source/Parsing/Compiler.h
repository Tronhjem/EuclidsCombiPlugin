#pragma once
#include <vector>
#include "Token.h"

class ErrorReporting;

enum class OpCode : int
{
    CONSTANT = 0,
    GET_IDENTIFIER,
    GET_IDENTIFIER_WITH_INDEX,
    SET_IDENTIFIER_VALUE,
    SET_IDENTIFIER_ARRAY,

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
    explicit Instruction(OpCode code, double value) : opCode(code), mDoubleValue(value) {}
    explicit Instruction(OpCode code, std::string name) : opCode(code), mNameValue(name) {}

    OpCode opCode;
    double mDoubleValue = 0.0;
    std::string mNameValue;
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
    Token& PeekNext();
    inline void ThrowUnexpectedTokenError(Token& tokenForError);
    inline void MakeIdentifierGetter(Token& token);
    inline void MakeConstant(Token& token);

    void CompileExpression();
    int CompileArray();

    int mCurrentIndex = 0;
    std::vector<Token>& mTokens;
    ErrorReporting& mErrorReporting;
    std::vector<Instruction> mInstructions;
};
