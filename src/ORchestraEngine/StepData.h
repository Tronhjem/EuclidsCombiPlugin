#pragma once
#include "Types.h"

enum class StepType : uChar
{
    NOTE = 0,
    CC = 1
};

class SequenceStep
{
public:
    SequenceStep(StepType type, StepData shouldTrigger, const StepData firstData, const StepData secondData, const StepData channel, const int duration) :
        mType(type),
        mShouldTrigger(shouldTrigger),
        mFirstData(firstData),
        mSecondData(secondData),
        mChannel(channel),
        mDuration(duration)
    {
    }
    
    ~SequenceStep() {}
    
    StepType mType;
    StepData mShouldTrigger;
    StepData mFirstData;
    StepData mSecondData;
    StepData mChannel;
    int mDuration;
    
private:
    SequenceStep() = delete;
};
