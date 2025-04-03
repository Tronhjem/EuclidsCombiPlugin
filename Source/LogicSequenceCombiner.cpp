#include "LogicSequenceCombiner.h"

const uChar LogicSequenceCombiner::operator [](const int index) const
{
    const int length = static_cast<int>(mLogicSequences.size());
    
    uChar result = mLogicSequences[0][index];
    for (int i = 1; i < length; ++i)
    {
        result = Evaluate(result, mLogicSequences[i][index], mOperations[i - 1]);
    }
    
    return result;
}

uChar LogicSequenceCombiner::Evaluate(const uChar a, const uChar b, const Operation operation) const
{
    switch (operation)
    {
        case Operation::AND:
        {
            return a & b;
        }
        case Operation::OR:
        {
            return a | b;
        }
        case Operation::XOR:
        {
            return a ^ b;
        }
            
        default:
            return 0;
    }
}
