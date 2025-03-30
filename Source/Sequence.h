//
//  Sequence.h
//
//  Created by Christian Tronhjem on 29.03.25.
//

#pragma once
#include <array>
#include "Types.h"

template <int Size> 
class Sequence 
{
public:
	Sequence(std::array<uint_8, Size> triggers)
	{
		mTriggers = triggers;
	}

	Sequence()
	{
		for (int i = 0; i < Size; ++i) 
			mTriggers[i] = 0;
	}

	const uint_8& operator [](const int i) const
	{
		return mTriggers[i];
	}

private:
	std::array<uint_8, Size>	mTriggers;
};
