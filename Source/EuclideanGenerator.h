#pragma once
#include "DataSequence.h"

void GenerateEuclideanSequence(uChar* sequence, const int hits, const int length)
{
    int count = length;
    for(int i = 0; i < length; ++i)
    {
        if (count >= length)
        {
            sequence[i] = static_cast<uChar>(1);
            count = count - length;
        }
        else
        {
            sequence[i] = static_cast<uChar>(0);
        }
        count += hits;
    }
}
