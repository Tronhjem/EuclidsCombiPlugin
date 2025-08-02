#include "DataSequence.h"

DataSequence::DataSequence(const std::vector<uChar>& data) : mData(data)
{
}

uChar DataSequence::GetValue(const int index) const
{
    const int indexWrapped = index % mData.size();
    return mData[indexWrapped];
}

void DataSequence::SetValue(int index, uChar value)
{
    mData[index] = value;
}
