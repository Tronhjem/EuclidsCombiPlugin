#include "LogicSequence.h"

LogicSequence::LogicSequence(const uint8_t* start, int length)
{
    SetSequence(start, length);
}

inline void LogicSequence::SetSequence(const uint8_t* start, const int length)
{
    if(length <= MaxSize)
    {
        for (int i = 0; i < length; ++i)
            mTriggers[i] = start[i];
        
        mLength = length;
    }
    else
    {
        mTriggers[0] = 0;
        mLength = 0;
    }
}
