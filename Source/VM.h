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
#include "Track.h"
#include "Types.h"
#include "MidiScheduler.h"
#include "DataSequence.h"

class Instruction;

//class ManagedMemory
//{
//public:
//    ManagedMemory() : mFreeIndex(0) {}
//
//    int* Write(int* start, int length)
//    {
//        int* startIndex = &mFreeIndex;
//        for (int i = mFreeIndex; i < mFreeIndex + length; ++i)
//        {
//            mMemory[i] = *(start + i);
//        }
//        mFreeIndex += length;
//
//        return startIndex;
//    }
//
//    int* Get(int index)
//    {
//        return &mMemory[index];
//    }
//
//private:
//    int mFreeIndex;
//    std::array<int, 512> mMemory;
//};

class Stack
{
public:
    Stack() {};
    double Pop() 
    {
        assert(stackPointer > 0);
        return mStack[--stackPointer];
    }
    void Push(uChar i)
    {
        assert(stackPointer + 1 <= 64);
        mStack[stackPointer++] = i;
    }

private:
    std::array<uChar, 64> mStack;
    int stackPointer = 0;
};


class VM
{
public:
    VM();
    bool Prepare(const char* filePath);
    bool ProcessOpCodes();
    void Tick(MidiScheduler& midiScheduler, int nextTickTime, int globalCount);

private:
    std::unordered_map<std::string, DataSequence> mVariables;
    std::unique_ptr<ErrorReporting> mErrorReporting;
    std::unique_ptr<Scanner> mScanner;
    std::unique_ptr<Compiler> mCompiler;
    std::unique_ptr<FileLoader> mFileLoader;

    std::vector<Track> tracks;
    Stack mStack;
};
