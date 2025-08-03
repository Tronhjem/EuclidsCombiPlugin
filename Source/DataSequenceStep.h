#pragma once

#include <cassert>
#include <cmath>

//#include "Types.h"

typedef unsigned char uChar;

constexpr int MAX_SUB_DIVISION = 8;

class DataSequenceStep
{
public:
    DataSequenceStep() {};
    DataSequenceStep(const uChar* data, const int length);
    DataSequenceStep(const int i);
    bool IsActiveAtSubStep(const int index);
    uChar GetActiveValueAtIndex(const int index) const;
    
//    uChar Get(const int index)
//    {
//#if _DEBUG
//        assert(index < MAX_SUB_DIVISION);
//#endif
//        return mData[index];
//    }
    
//    uChar GetDataAtSubDivision(const int subDiv)
//    {
//        const int index = std::floor((static_cast<float>(subDiv) /
//                                      static_cast<float>(MAX_SUB_DIVISION)) * mLength);
//        return mData[index];
//    }
    
private:
    int mLength;
    uChar mData[MAX_SUB_DIVISION];
};
