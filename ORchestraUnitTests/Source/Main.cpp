#include <JuceHeader.h>
#include "Test_Operators.h"

using namespace juce;

int main (int argc, char* argv[])
{
    UnitTestRunner runner;
    
    Test_Operators opcodeResults;
    
    runner.runAllTests();
    return 0;
}
