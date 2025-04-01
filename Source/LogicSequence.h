//
//  Sequence.h
//
//  Created by Christian Tronhjem on 29.03.25.
//

#pragma once
#include <array>
#include "Types.h"

constexpr int MaxSize = 32;

class LogicSequence 
{
public:
    LogicSequence(const uint8_t* start, int length);
    
	const uint_8& operator [](const int i) const
	{
		return mTriggers[i % mLength];
	}

private:
    inline void SetSequence(const uint8_t* start, const int length);
    
    int mLength;
	std::array<uint_8, MaxSize> mTriggers;
};
