#pragma once

#include <vector>

#include "Types.h"

enum class DataSequenceType : uChar
{
    Note,
    Velocity,
    CC,
    CCValue
};

class DataSequence
{
public:
    DataSequence() {}
    DataSequence(const std::vector<uChar>& data);
    uChar GetValue(int index);
    
//    DataSequence(const DataSequence& copy)
//    {
//        mData = std::move(copy.mData);
//    }
//    
//    DataSequence(DataSequence&& copy)
//    {
//        mData = std::move(copy.mData);
//    }
//    
//    DataSequence& operator= (const DataSequence& other)
//    {
//        mData = std::move(other.mData);
//        return *this;
//    }
    
private:
    inline void SetSequence(uChar* start, int length);
    std::vector<uChar> mData;
};
