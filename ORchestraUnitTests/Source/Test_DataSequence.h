#pragma once

using namespace juce;
#include "../../Source/VM.h"

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
    }
};
