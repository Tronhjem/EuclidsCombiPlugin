#pragma once
#include <array>

typedef unsigned short int UINT;

class Test
{
	int s = 90;
};

template <int Size> 
class Sequence 
{
public:
	Sequence(std::array<UINT, Size> triggers)
	{
		mTriggers = triggers;
	}

	Sequence()
	{
		for (int i = 0; i < Size; ++i) 
			mTriggers[i] = 0;
	}

	const UINT& operator [](const int i) const
	{
		return mTriggers[i];
	}

private:
	std::array<UINT, Size>	mTriggers;
};