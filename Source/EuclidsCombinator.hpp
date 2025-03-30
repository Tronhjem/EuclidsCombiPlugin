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
    void Tick(const TransportData& transportData, const int bufferLength, juce::MidiBuffer& midiMessages);
    
private:
    double mBpmDivide = 1.0;
    MidiScheduler mMidiScheduler;
    Output* mOutput;
};
