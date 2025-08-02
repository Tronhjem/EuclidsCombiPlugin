#pragma once

#include <vector>

#include "Types.h"
#include "DataSequenceStep.h"

class DataSequence
{
public:
    DataSequence() {}
    DataSequence(const std::vector<uChar>& data);
    uChar GetValue(const int index) const;
    void SetValue(const int index, uChar value);
    
private:
    std::vector<uChar> mData;
};
