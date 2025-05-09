#pragma once

#include <JuceHeader.h>
#include <memory>

#include "TransportData.h"
#include "MidiScheduler.h"
#include "VM.h"

class EuclidsCombinatorEngine
{
public:
    EuclidsCombinatorEngine();
    ~EuclidsCombinatorEngine();
    void Tick(const TransportData& transportData, const int bufferLength, juce::MidiBuffer& midiMessages);
    
private:
    double mBpmDivide = 1.0;
    bool mIsVMInit = false;
    MidiScheduler mMidiScheduler;
    std::unique_ptr<VM> mVM;
};
