#include "DataSequence.h"

DataSequence::DataSequence(const std::vector<uChar>& data) : mData(data)
{
}

void DataSequence::SetSequence(uChar *start, int length)
{
    for (int i = 0; i < length; ++i)
    {
        mData.emplace_back(start[i]);
    }
}

uChar DataSequence::GetValue(int index)
{
    int indexWrapped = index % mData.size();
    return mData[indexWrapped];
}
