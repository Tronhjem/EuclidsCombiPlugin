#pragma once

#include <vector>

#include "Types.h"

enum class DataSequenceType : uChar
{
    Note,
    Velocity,
    CC,
    CCValue
};

class DataSequence
{
public:
    DataSequence() {}
    DataSequence(const std::vector<uChar>& data);
    uChar GetValue(int index);
    
private:
    std::vector<uChar> mData;
};
