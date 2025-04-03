#pragma once

#include <JuceHeader.h>

#include "Track.hpp"
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
    std::vector<Track> mTracks;
    
    juce::UnitTestRunner testRunner;
};
