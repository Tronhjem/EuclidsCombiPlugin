//
//  Sequence.h
//
//  Created by Christian Tronhjem on 29.03.25.
//

#pragma once
#include <array>
#include "Types.h"

//template <int Size>
constexpr int MaxSize = 32;
class Sequence 
{
public:
	Sequence(const uint8_t* start, int length)
    {
        SetSequence(start, length);
    }
    
    void SetSequence(const uint8_t* start, int length)
    {
        if(length <= MaxSize)
        {
            for (int i = 0; i < length; ++i)
                mTriggers[i] = start[i];
            
            mLength = length;
        }
        else
        {
            mLength = 0;
        }
    }

	const uint_8& operator [](const int i) const
	{
		return mTriggers[i % mLength];
	}

private:
    int mLength;
	std::array<uint_8, MaxSize> mTriggers;
};
