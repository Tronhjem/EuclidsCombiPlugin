//
//  MidiScheduler.h
//
//  Created by Christian Tronhjem on 30.03.25.
//

#pragma once
#include <JuceHeader.h>
#include <vector>

#include "Types.h"

struct ScheduledMidi
{
    uint_8 channel;
    uint_8 noteNumber;
    uint_8 velocity;
    int schuledTime;
};

class MidiScheduler
{
public:
    MidiScheduler();
    
    void PostMidi(uint_8 channel, uint_8 noteNumber, uint_8 velocity, int durationInSamples, int timeStamp);
    void ProcessMidiPosts(juce::MidiBuffer& midiMessages, int64_t positionInSamples);
    
private:
    std::vector<ScheduledMidi> mScheduledMidiMessages;
    
};
