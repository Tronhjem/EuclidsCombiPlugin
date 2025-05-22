#include <JuceHeader.h>
#include "Test_Operators.h"
#include "Test_EuclidAndRandom.h"

using namespace juce;

int main (int argc, char* argv[])
{
    UnitTestRunner runner;
    
    Test_Operators opcodeResults;
    Test_EuclidAndRandom euclidAndRandom;
    
    runner.runAllTests();
    return 0;
}
