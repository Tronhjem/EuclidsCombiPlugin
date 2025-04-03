#include <JuceHeader.h>
#include "test_LogicSequence.h"
#include "test_LogicSequenceCombiner.h"

using namespace juce;

int main (int argc, char* argv[])
{
    UnitTestRunner runner;
    Test_LogicSequence testLogicSequence;
    Test_LogicSequenceCombiner testLogicSequenceCombiner;
    
    runner.runAllTests();
    return 0;
}
