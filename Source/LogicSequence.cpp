#include "LogicSequence.h"

LogicSequence::LogicSequence(const uChar* start, int length)
{
    SetSequence(start, length);
}

inline void LogicSequence::SetSequence(const uChar* start, const int length)
{
#if DEBUG
    assert(length <= MaxSize);
#endif
    
    if(length <= MaxSize)
    {
        mTrigger = 0;
        for (int i = 0; i < length; ++i)
        {
            mTrigger |= start[i] << i;
        }
        
        mLength = length;
    }
}

const uChar LogicSequence::operator [](const int i) const
{
    Trigger index = static_cast<Trigger>(i % mLength);
    uChar value = ((1 << index) & mTrigger) >> index;
    return value;
}
