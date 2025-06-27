#pragma once

using namespace juce;
#include "../../Source/VM.h"

class Test_ErrorHandling  : public UnitTest
{
public:
    Test_ErrorHandling()  : UnitTest ("Test_ErrorHandling") {}
    
    void runTest() override
    {
        {
            beginTest ("Array: No end to array");
            
            std::string file {"a = [2, \n"};
            VM vm;
            bool result = vm.Prepare(&file[0]);
            
            expect (result == false);
        }
        {
            beginTest ("Array: two commas in a row");
            
            std::string file {"a = [2,,] \n"};
            VM vm;
            bool result = vm.Prepare(&file[0]);
            
            expect (result == false);
        }
        {
            beginTest ("Array: Number without comma seperation");
            
            std::string file {"a = [2 3,] \n"};
            VM vm;
            bool result = vm.Prepare(&file[0]);
            
            expect (result == false);
        }
        {
            beginTest ("Expression: value and operator");
            
            std::string file {"a = 2 + \n"};
            VM vm;
            bool result = vm.Prepare(&file[0]);
            
            expect (result == false);
        }
        {
            beginTest ("Expression: value and operator and parenteses");
            
            std::string file {"a = (2 + 3) + \n"};
            VM vm;
            bool result = vm.Prepare(&file[0]);
            
            expect (result == false);
        }
        {
            beginTest ("Expression: parenteses again");
            
            std::string file {"a = ( + 3) + \n"};
            VM vm;
            bool result = vm.Prepare(&file[0]);
            
            expect (result == false);
        }
        {
            beginTest ("Expression: parenteses again");
            
            std::string file {"a = () + 2 \n"};
            VM vm;
            bool result = vm.Prepare(&file[0]);
            
            expect (result == false);
        }
        {
            beginTest ("Expression: parenteses again");
            
            std::string file {"a = (2 - 3) 2 \n"};
            VM vm;
            bool result = vm.Prepare(&file[0]);
            
            expect (result == false);
        }
        {
            beginTest ("Expression: parenteses again");
            
            std::string file {"a = + (2 - 3) \n"};
            VM vm;
            bool result = vm.Prepare(&file[0]);
            
            expect (result == false);
        }
        {
            beginTest ("Expression: matching parenteses");
            
            std::string file {"a = (2 - 3 \n"};
            VM vm;
            bool result = vm.Prepare(&file[0]);
            
            expect (result == false);
        }
        {
            beginTest ("Expression: matching parenteses");
            
            std::string file {"a = 2 - 3) \n"};
            VM vm;
            bool result = vm.Prepare(&file[0]);
            
            expect (result == false);
        }
        {
            beginTest ("Expression: matching parenteses");
            
            std::string file {"a = (2 - 3) + 2) \n"};
            VM vm;
            bool result = vm.Prepare(&file[0]);
            
            expect (result == false);
        }
        {
            beginTest ("Random: two numbers no comma");
            
            std::string file {"a = ran{2 3} \n"};
            VM vm;
            bool result = vm.Prepare(&file[0]);
            
            expect (result == false);
        }
        {
            beginTest ("Random: two numbers no comma");
            
            std::string file {"a = ran{2,} \n"};
            VM vm;
            bool result = vm.Prepare(&file[0]);
            
            expect (result == false);
        }
    }
};
