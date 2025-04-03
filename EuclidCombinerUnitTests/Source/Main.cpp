#include <JuceHeader.h>
#include "test_LogicSequence.h"

using namespace juce;

int main (int argc, char* argv[])
{
    UnitTestRunner runner;
    Test_LogicSequence testLogicSequence;
    
    
    runner.runAllTests();
    return 0;
}
