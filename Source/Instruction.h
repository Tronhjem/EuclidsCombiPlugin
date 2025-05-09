#pragma once

enum class OpCode : uChar
{
    CONSTANT = 0,
    GET_IDENTIFIER_VALUE,
    GET_IDENTIFIER_WITH_INDEX,
    SET_IDENTIFIER_VALUE,
    SET_IDENTIFIER_ARRAY,

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
