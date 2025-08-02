#pragma once

#include <cassert>
#include <cmath>
#include "Types.h"

constexpr int MAX_SUB_DIVISION = 8;

class DataSequenceStep
{
public:
    DataSequenceStep(const uChar* data, const int length) : mLength(length)
    {
#if _DEBUG
        assert(length < MAX_SUB_DIVISION);
#endif
        for(int i = 0; i < length; ++i)
        {
            mData[i] = data[i];
        }
    }
    
    uChar GetDataAtSubDivision(const int subDiv)
    {
        const int index = std::floor((static_cast<float>(subDiv) /
                                      static_cast<float>(MAX_SUB_DIVISION)) * mLength);
        return mData[index];
    }
    
    // COMBINE
    //
    // a = [[1, 0]]
    // b = [[1, 1, 1, 1]]
    
    // length = max (a.length,b.length)
    // for x in length:
    //      a.length / length // 4/2 = 2
    //      2
    // c = a & b       // [[1,1,0,0]]
    //
    //
    
private:
    DataSequenceStep() = delete;
    
    int mLength;
    uChar mData[MAX_SUB_DIVISION];
};
