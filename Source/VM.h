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
#include "CustomStack.h"

class Instruction;

class VM
{
public:
    VM();
    bool Prepare(char* data);
    bool Tick(std::vector<SequenceStep>& stepQueue, const int globalCount);
    void Reset();
    StepData GetTopStackValue() { return mStack.Top(); }
    std::vector<LogEntry>& GetErrors();

private:
    bool ProcessOpCodes(std::vector<Instruction>& setupInstructions);
    bool ProcessInstruction(const Instruction& instruction, const int stepCount);
    
    template<typename Operation>
    void PopDoOperationAndPush(Operation op)
    {
        static_assert(std::is_invocable_v<Operation, const int, const int>,
                             "Operation must be callable with two int parameters");
        
        const StepData& b = mStack.Pop();
        const StepData& a = mStack.Pop();
        const StepData result = a.ApplyOperation(b, op);
        mStack.Push(StepData{result});
    }
    
    std::unique_ptr<ErrorReporting> mErrorReporting;
    std::unordered_map<std::string, DataSequence> mVariables;
    std::vector<Instruction> mRuntimeInstructions;
    inline uChar RandomValue(const uChar low, const uChar high);

    Stack<StepData> mStack;
};
