#pragma once

#include <vector>

#include "Types.h"
#include "DataSequenceStep.h"

class DataSequence
{
public:
    DataSequence() {}
    DataSequence(const std::vector<StepData>& data);
    StepData GetValue(const int index) const;
    void SetValue(const int index, StepData value);
    
private:
    std::vector<StepData> mData;
};
