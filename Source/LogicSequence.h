#pragma once
#include <array>
#include "Types.h"

constexpr int MaxSize = 32;

class LogicSequence 
{
public:
    LogicSequence(const uChar* start, int length);
    
	const uChar& operator [](const int i) const
	{
		return mTriggers[i % mLength];
	}

private:
    inline void SetSequence(const uChar* start, const int length);
    
    int mLength;
	std::array<uChar, MaxSize> mTriggers;
};
