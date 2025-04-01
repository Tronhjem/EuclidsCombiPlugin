#pragma once
#include <array>
#include "Types.h"

constexpr int MaxSize = sizeof(Trigger) * 4;

class LogicSequence 
{
public:
    LogicSequence(const uChar* start, int length);
    const uChar operator [](const int i) const;
   
private:
    inline void SetSequence(const uChar* start, const int length);
    int8_t mLength;
    Trigger mTrigger;
};
