#pragma once

#include "Types.h"
#include "LogicSequence.h"
#include <vector>

enum class Operation : uChar
{
    AND,
    OR,
    XOR
};

class LogicSequenceCombiner
{
public:
    LogicSequenceCombiner(LogicSequence logicSeq);
    void AddLogic(LogicSequence logicSeq, Operation operation);
    const uChar operator [](const int i) const;
    
private:
    LogicSequenceCombiner() = delete;
    
    uChar Evaluate(const uChar a, const uChar b, const Operation operation) const;
    
    std::vector<LogicSequence> mLogicSequences;
    std::vector<Operation> mOperations;
};

