#include "DataSequence.h"

DataSequence::DataSequence(const std::vector<uChar>& data) : mData(data)
{
}

uChar DataSequence::GetValue(int index)
{
    int indexWrapped = index % mData.size();
    return mData[indexWrapped];
}
