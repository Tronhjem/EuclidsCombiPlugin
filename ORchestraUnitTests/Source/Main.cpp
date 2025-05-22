#include <JuceHeader.h>
#include "OpCodeResultTests.h"

using namespace juce;

int main (int argc, char* argv[])
{
    UnitTestRunner runner;
    
    Test_OpCodeResults opcodeResults;
    
    runner.runAllTests();
    return 0;
}
