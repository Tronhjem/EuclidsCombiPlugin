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
