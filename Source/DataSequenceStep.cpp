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
    mData[0] = i;
    mLength = 1;
}

uChar DataSequenceStep::GetValue(const int index) const
{
#if _DEBUG
    assert(index < mLength);
#endif
    
    return mData[index];
}

uChar DataSequenceStep::GetEquivalentValueAtIndex(const int index, const int otherLength) const
{
#if _DEBUG
    assert(index < MAX_SUB_DIVISION_LENGTH);
#endif
    
    if (mLength == 1)
        return mData[0];
    
    const int equivalentIndex = floor((static_cast<float>(index) / static_cast<float>(otherLength)) * mLength);
    
#if _DEBUG
    assert(equivalentIndex < mLength);
#endif
    
    return mData[equivalentIndex];
}

void DataSequenceStep::SetData(const uChar* data, const int length)
{
    memcpy(mData, data, length);
    mLength = length;
}
