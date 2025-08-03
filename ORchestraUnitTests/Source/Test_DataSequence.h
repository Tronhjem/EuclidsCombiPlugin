#pragma once

using namespace juce;
#include "../../Source/VM.h"
#include "../../Source/DataSequenceStep.h"

class Test_DataSequence  : public UnitTest
{
public:
    Test_DataSequence()  : UnitTest ("Test_DataSequence") {}
    
    void runTest() override
    {
        {
            beginTest ("Basic Get in Data Sequence");
            
            std::string file {"a = [1,0,0] \n test a[0]"};
            VM vm;
            expect(vm.Prepare(&file[0]));
            
            uChar result = vm.GetTopStackValue();
            expect (result == 1);
        }
        {
            beginTest ("Check Data Step Get");
            
            uChar data[3] {1,0,0};
            DataSequenceStep dataSeqStep{data, 3};
            expect (dataSeqStep.Get(0) == 1);
        }
        {
            beginTest ("Check Data Step Get");
            
            uChar dataOne[2] {1, 1};
            uChar dataTwo[4] {1, 0, 1, 0};
            
            DataSequenceStep dataStepOne {dataOne, 2};
            DataSequenceStep dataStepTwo {dataTwo, 4};
            
        }
    }
};
