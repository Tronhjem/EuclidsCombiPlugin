#pragma once

#include <array>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <cassert>

#include "ErrorReporting.h"
#include "Scanner.h"
#include "Compiler.h"
#include "Types.h"
#include "MidiScheduler.h"
#include "DataSequence.h"
#include "StepData.h"

class Instruction;

class Stack
{
public:
    Stack() {};
    uChar Pop()
    {
        assert(stackPointer > 0);
        return mStack[--stackPointer];
    }
    
    void Push(uChar i)
    {
        assert(stackPointer + 1 <= 64);
        mStack[stackPointer++] = i;
    }
    
    void Clear()
    {
        stackPointer = 0;
    }
    
    uChar Top()
    {
        return mStack[stackPointer];
    }

private:
    std::array<uChar, 64> mStack;
    int stackPointer = 0;
};

class VM
{
public:
    VM();
    bool Prepare(char* data);
    bool Tick(std::vector<StepData>& stepQueue, const int globalCount);
    void Reset();
    uChar GetTopStackValue() { return mStack.Top(); }
    std::vector<LogEntry>& GetErrors();

private:
    bool ProcessOpCodes(std::vector<Instruction>& setupInstructions);
    bool ProcessInstruction(const Instruction& instruction, const int stepCount);
    
    std::unique_ptr<ErrorReporting> mErrorReporting;
    std::unordered_map<std::string, DataSequence> mVariables;
    std::vector<Instruction> mRuntimeInstructions;
    inline uChar RandomValue(uChar low, uChar high);

    Stack mStack;
};
