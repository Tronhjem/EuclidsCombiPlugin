#pragma once
#include "Types.h"

enum class StepType
{
    NOTE,
    CC
};

struct StepData
{
public:
    StepData(StepType type, uChar shouldTrigger, const uChar firstData, const uChar secondData, const uChar channel, const int duration) :
        mType(type),
        mShouldTrigger(shouldTrigger),
        mFirstData(firstData),
        mSecondData(secondData),
        mChannel(channel),
        mDuration(duration)
    {
    }
    
    ~StepData() {}
    StepData() = delete;
    
    StepType mType;
    uChar mShouldTrigger;
    uChar mFirstData;
    uChar mSecondData;
    uChar mChannel;
    int mDuration;
};
