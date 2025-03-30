//
//  EuclidsCombinator.hpp
//
//  Created by Christian Tronhjem on 29.03.25.
//

#pragma once

#include <JuceHeader.h>

#include "Output.hpp"
#include "TransportData.h"
#include "MidiScheduler.h"

class EuclidsCombinatorEngine
{
public:
    EuclidsCombinatorEngine();
    ~EuclidsCombinatorEngine();
    void Tick(const TransportData& transportData, juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    
private:
    double mBpmDivide = 1.0;
    int mTimeSincePost = 0;
    Output* mOutput;
};
