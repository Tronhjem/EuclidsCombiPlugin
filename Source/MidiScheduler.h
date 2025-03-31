//
//  MidiScheduler.h
//
//  Created by Christian Tronhjem on 30.03.25.
//

#pragma once
#include <JuceHeader.h>
#include <vector>

#include "Types.h"

enum class MidiType : uint_8
{
    Note = 0,
    CC = 1
};

struct ScheduledMidi
{
    uint_8 channel;
    uint_8 noteNumber;
    uint_8 velocity;
    MidiType type;
    int schuledTime;
};

class MidiScheduler
{
public:
    MidiScheduler();
    
    void PostMidiNote(uint_8 channel, uint_8 noteNumber, uint_8 velocity, int durationInSamples, int timeStamp);
    void ProcessMidiPosts(juce::MidiBuffer& midiMessages, int nextTickTime, int bufferLength, int64_t endOfBufferPosition);
    
private:
    std::vector<ScheduledMidi> mScheduledMidiMessages;
    
};
