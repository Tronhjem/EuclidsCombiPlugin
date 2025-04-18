#pragma once
#include <array>
#include <vector>
#include <unordered_map>
#include <string>

class Instruction;
class Logger;

class ManagedMemory
{
public:
    ManagedMemory() : mFreeIndex(0) {}

    int* Write(int* start, int length)
    {
        int* startIndex = &mFreeIndex;
        for (int i = mFreeIndex; i < mFreeIndex + length; ++i)
        {
            mMemory[i] = *(start + i);
        }
        mFreeIndex += length;

        return startIndex;
    }

    int* Get(int index)
    {
        return &mMemory[index];
    }

private:
    int mFreeIndex;
    std::array<int, 512> mMemory;
};

class Stack
{
public:
    Stack() {};
    double Pop() 
    {
        assert(stackPointer > 0);
        return mStack[--stackPointer];
    }
    void Push(double i)
    {
        assert(stackPointer + 1 <= 64);
        mStack[stackPointer++] = i;
    }

private:
    std::array<double, 64> mStack;
    int stackPointer = 0;
};

class VM
{
public:
    VM(std::vector<Instruction>& instructions, Logger& log) : mInstructions(instructions), mLogger(log) {}
    void Run();

private:
    Logger& mLogger;
    ManagedMemory mMemory;
    Stack mStack;
    std::vector<Instruction>& mInstructions;
    std::unordered_map<std::string, double> mVariables;
    int mCurrentIndex = 0;
};