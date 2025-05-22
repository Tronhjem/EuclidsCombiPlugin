#pragma once

using namespace juce;
#include "../../Source/VM.h"

class Test_EuclidAndRandom  : public UnitTest
{
public:
    Test_EuclidAndRandom()  : UnitTest ("Test_OpCodeResults") {}
    
    void runTest() override
    {
        {
            beginTest ("Test Euclid sequence wrong defined");
            
            std::string file {"a = {2,4 \n test a"};
            VM vm;
            bool result = vm.Prepare(&file[0]);
            
            expect (result == false);
        }
        {
            beginTest ("Test Euclid sequence wrong defined ");
            
            std::string file {"a = {2} \n test a"};
            VM vm;
            bool result = vm.Prepare(&file[0]);
            
            expect (result == false);
        }
        {
            beginTest ("Test Euclid sequence");
            
            std::string file {"a = {2,4} \n test a"};
            VM vm;
            vm.Prepare(&file[0]);
            
            uChar result = vm.GetTopStackValue();
            expect (result == 1);
        }
        {
            beginTest ("Test Euclid sequence");
            
            std::string file {"a = {2,4} \n test a[1]"};
            VM vm;
            vm.Prepare(&file[0]);
            
            uChar result = vm.GetTopStackValue();
            expect (result == 0);
        }
        {
            beginTest ("Test Euclid sequence");
            
            std::string file {"a = {2,4} \n test a[2]"};
            VM vm;
            vm.Prepare(&file[0]);
            
            uChar result = vm.GetTopStackValue();
            expect (result == 1);
        }
        {
            beginTest ("Test Euclid sequence");
            
            std::string file {"a = {2,4} \n test a[3]"};
            VM vm;
            vm.Prepare(&file[0]);
            
            uChar result = vm.GetTopStackValue();
            expect (result == 0);
        }
    }
};
