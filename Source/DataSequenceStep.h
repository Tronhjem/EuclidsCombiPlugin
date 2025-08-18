#pragma once

#include <cassert>
#include <cmath>

typedef unsigned char uChar;

constexpr int MAX_SUB_DIVISION_LENGTH = 4;

class DataSequenceStep
{
public:
    DataSequenceStep() : mLength(0) {};
    DataSequenceStep(const uChar* data, const int length);
    DataSequenceStep(const int i);
    uChar GetValue(const int index) const;
    uChar GetEquivalentValueAtIndex(const int index, const int otherLength) const;
    void SetData(const uChar* data, const int length);
    uChar GetLength() const { return mLength; };
    
private:
    uChar mLength;
    uChar mData[MAX_SUB_DIVISION_LENGTH];
};
