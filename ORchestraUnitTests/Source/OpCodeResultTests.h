#pragma once

using namespace juce;
#include "../../Source/ORchestraEngine.h"
#include "../../Source/VM.h"

class Test_OpCodeResults  : public UnitTest
{
public:
    Test_OpCodeResults()  : UnitTest ("Test_OpCodeResults") {}
    
    void runTest() override
    {
        {
            beginTest ("Test Setting single var value");
            
            std::string file {"a = 74 \n test a"};
            VM vm;
            vm.Prepare(&file[0]);
            
            uChar result = vm.GetTopStackValue();
            expect (result == 74);
        }
        {
            beginTest ("Test Setting single expression addition value");
            
            std::string file {"a = 74+1 \n test a"};
            VM vm;
            vm.Prepare(&file[0]);
            
            uChar result = vm.GetTopStackValue();
            expect (result == 74 + 1);
        }
        {
            beginTest ("Test Setting single expression subtraction value");
            
            std::string file {"a = 74-1 \n test a"};
            VM vm;
            vm.Prepare(&file[0]);
            
            uChar result = vm.GetTopStackValue();
            expect (result == 74 - 1);
        }
        {
            beginTest ("Test Setting single expression multiplication value");
            
            std::string file {"a = 2*2 \n test a"};
            VM vm;
            vm.Prepare(&file[0]);
            
            uChar result = vm.GetTopStackValue();
            expect (result == 2 * 2);
        }
        {
            beginTest ("Test Setting single expression multiplication value");
            
            std::string file {"a = 2/2 \n test a"};
            VM vm;
            vm.Prepare(&file[0]);
            
            uChar result = vm.GetTopStackValue();
            expect (result == 2 / 2);
        }
        {
            beginTest ("Test Setting single expression math precedence");
            
            std::string file {"a = 2 + 2 * 5 \n test a"};
            VM vm;
            vm.Prepare(&file[0]);
            
            uChar result = vm.GetTopStackValue();
            expect (result == 12 /* (2 + 2) * 5 */);
        }
        {
            beginTest ("Test Setting identifier from other identifier");
            
            std::string file {"a = 5 \n b = a \n test b"};
            VM vm;
            vm.Prepare(&file[0]);
            
            uChar result = vm.GetTopStackValue();
            expect (result == 5);
        }
        {
            beginTest ("Test Setting identifier from other plus math identifier");
            
            std::string file {"a = 5 \n b = a + 2 * 5 \n test b"};
            VM vm;
            vm.Prepare(&file[0]);
            
            uChar result = vm.GetTopStackValue();
            expect (result == 5 + (2 * 5));
        }
        {
            beginTest ("Test Setting getting array index from identifier");
            
            std::string file {"a = [64,65,63] \n test a[0]"};
            VM vm;
            vm.Prepare(&file[0]);
            
            uChar result = vm.GetTopStackValue();
            expect (result == 64);
        }
        {
            beginTest ("Test Setting getting array index from identifier");
            
            std::string file {"a = [64,65,63] \n b = a[0] + 2 \n test b"};
            VM vm;
            vm.Prepare(&file[0]);
            
            uChar result = vm.GetTopStackValue();
            expect (result == (64 + 2));
        }
        {
            beginTest ("Test Setting setting identifier with getting global index");
            
            // Global index when not running is 0
            std::string file {"a = [64,65,63] \n b = a \n test b"};
            VM vm;
            vm.Prepare(&file[0]);
            
            uChar result = vm.GetTopStackValue();
            expect (result == 64);
        }
    }
};
