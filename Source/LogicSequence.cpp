#include "LogicSequence.h"

LogicSequence::LogicSequence(const uChar* start, int length)
{
    SetSequence(start, length);
}

inline void LogicSequence::SetSequence(const uChar* start, const int length)
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
