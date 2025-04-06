#include "DataSequence.h"

DataSequence::DataSequence(uChar* start, int length, DataSequenceType seqType) :
    mType(seqType)
{
    SetSequence(start, length);
}

void DataSequence::SetSequence(uChar *start, int length)
{
    for (int i = 0; i < length; ++i)
    {
        mData.emplace_back(start[i]);
    }
}

uChar DataSequence::GetNextValue()
{
    uChar temp = mData[mInternalPosition];
    mInternalPosition = ++mInternalPosition % mData.size();
    return temp;
}
