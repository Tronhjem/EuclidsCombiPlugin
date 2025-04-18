#pragma once
#include <array>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

#include "ErrorReporting.h"
#include "Scanner.h"
#include "Compiler.h"
#include "FileLoader.h"

class Instruction;

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
    VM();
    void Prepare(const char* filePath);
    void Run();

private:
    std::unordered_map<std::string, double> mVariables;
    std::unique_ptr<ErrorReporting> mErrorReporting;
    std::unique_ptr<Scanner> mScanner;
    std::unique_ptr<Compiler> mCompiler;
    std::unique_ptr<FileLoader> mFileLoader;
    Stack mStack;
};
