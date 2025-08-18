#include <algorithm>

#include "DataSequenceStep.h"

DataSequenceStep::DataSequenceStep(const uChar* data, const int length)
    : mLength(length)
{
#if _DEBUG
    assert(length <= MAX_SUB_DIVISION_LENGTH);
#endif
    
    for(int i = 0; i < length; ++i)
    {
        mData[i] = data[i];
    }
}

DataSequenceStep::DataSequenceStep(const int i)
{
    for(int i = 0; i < MAX_SUB_DIVISION_LENGTH; ++i)
    {
        mData[i] = 0;
    }
    
    mData[0] = i;
    mLength = 1;
}

uChar DataSequenceStep::GetActiveValueAtIndex(const int index) const
{
#if _DEBUG
    assert(index < MAX_SUB_DIVISION_LENGTH);
#endif
    
    if (mLength == 1)
        return mData[0];
    
    const int realIndex = index;
    
#if _DEBUG
    assert(realIndex < mLength);
#endif
    
    return mData[index];
}

bool DataSequenceStep::IsActiveAtSubStep(const int index)
{
    return false;
}

void DataSequenceStep::SetData(const uChar* data, const int length)
{
    memcpy(mData, data, length);
    mLength = length;
}
