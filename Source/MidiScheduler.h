//
//  MidiScheduler.h
//
//  Created by Christian Tronhjem on 30.03.25.
//

#pragma once
#include <JuceHeader.h>

class MidiScheduler
{
public:
    void PostMidi(const juce::MidiMessage& message);
    void Tick(juce::MidiBuffer& midiMessages, int64_t positionInSamples);
    
private:
    
};
