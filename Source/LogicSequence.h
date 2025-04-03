#pragma once
#include <array>
#include "Types.h"

class LogicSequence 
{
public:
    LogicSequence(const uChar* start, int length);
    const uChar operator [](const int i) const;
    int8_t GetLength() { return mLength; }
   
private:
    inline void SetSequence(const uChar* start, const int length);
    int8_t mLength;
    Trigger mTrigger;
};
