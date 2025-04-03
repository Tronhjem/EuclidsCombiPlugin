#pragma once
using namespace juce;
#include "../../Source/LogicSequence.h"

constexpr int testLength = 8;
class Test_LogicSequence  : public UnitTest
{
public:
    Test_LogicSequence()  : UnitTest ("Test_LogicSequence") {}
 
    void runTest() override
    {
        // Init
        uChar seq[testLength] = {1,0,1,0,1,0,1,0};
        LogicSequence sequence {&seq[0], testLength};
        
        beginTest ("Length Getter Getter");
        expect (sequence.GetLength() == testLength);
        
        beginTest ("Get Sequence entry");
        for (int i = 0; i < testLength; ++i)
        {
            expect (sequence[i] == seq[i]);
        }
    
        beginTest ("Get Sequence Wrap Around");
        expect (sequence[testLength + 3] == seq[(testLength + 3) % testLength]);
        
        for (int i = 0; i < testLength * 3; ++i)
        {
            expect (sequence[i] == seq[i % testLength]);
        }
    }
};
