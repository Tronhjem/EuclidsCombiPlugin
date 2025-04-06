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
    DataSequence(uChar* start, int length, DataSequenceType seqType);
    uChar GetNextValue();
    
private:
    DataSequence& operator= (const DataSequence&) = delete;
    
    inline void SetSequence(uChar* start, int length);
    DataSequenceType mType;
    uChar mInternalPosition;
    std::vector<uChar> mData;
};
