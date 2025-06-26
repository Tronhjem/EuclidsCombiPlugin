#include <JuceHeader.h>
#include "Test_Operators.h"
#include "Test_EuclidAndRandom.h"
#include "Test_ErrorHandling.h"

using namespace juce;

int main (int argc, char* argv[])
{
    UnitTestRunner runner;
    
    Test_Operators opcodeResults;
    Test_EuclidAndRandom euclidAndRandom;
    Test_ErrorHandling errorHandling;
    
    runner.runAllTests();
    return 0;
}
