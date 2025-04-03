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

//class LogicCombiner
//{
//public:
//    LogicCombiner(Trigger trigger, Operation operation) :
//        mTrigger(trigger),
//        mOperation(operation)
//    {
//    }
//
//private:
//    Trigger mTrigger;
//    Operation mOperation;
//};

class LogicSequenceCombiner
{
public:
    LogicSequenceCombiner(LogicSequence logicSeq)
    {
        mLogicSequences.emplace_back(logicSeq);
    }
    
    void AddTrigger(LogicSequence logicSeq, Operation operation)
    {
        mLogicSequences.emplace_back(logicSeq);
        mOperations.emplace_back(operation);
    }
    
    const uChar operator [](const int i) const;
    
private:
    uChar Evaluate(const uChar a, const uChar b, const Operation operation) const;
    LogicSequenceCombiner() = delete;
    
    std::vector<LogicSequence> mLogicSequences;
    std::vector<Operation> mOperations;
};

