#pragma once
#include <string>

class StoredFunction
{
public:
	StoredFunction() {};
	StoredFunction(std::string name, int numOfParams, std::vector<Instruction>& instructions) :
		mName(name),
		mNumOfParams(numOfParams),
		mInstructions(instructions) 
	{
	}

	void SetInstructions(std::vector<Instruction>& instructions)
	{
		mInstructions = instructions;
	}

	std::string mName;
	int mNumOfParams;
	std::vector<Instruction> mInstructions;
};