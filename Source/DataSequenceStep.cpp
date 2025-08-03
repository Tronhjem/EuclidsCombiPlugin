#include <algorithm>

#include "DataSequenceStep.h"

DataSequenceStep::DataSequenceStep(const uChar* data, const int length)
    : mLength(length)
{
#if _DEBUG
    assert(length < MAX_SUB_DIVISION);
#endif
    
    for(int i = 0; i < length; ++i)
    {
        mData[i] = data[i];
    }
}

DataSequenceStep::DataSequenceStep(const int i)
{
    for(int i = 0; i < MAX_SUB_DIVISION; ++i)
    {
        mData[i] = 0;
    }
    
    mData[0] = i;
    mLength = 1;
}

uChar DataSequenceStep::GetActiveValueAtIndex(const int index) const
{
#if _DEBUG
    assert(index < mLength);
#endif
    
    return mData[index];
}

bool DataSequenceStep::IsActiveAtSubStep(const int index)
{
    return false;
}
