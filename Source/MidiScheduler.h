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
//    uint_8 channel;
//    uint_8 noteNumber;
//    uint_8 velocity;
//    MidiType type;
    juce::MidiMessage midiData;
    int schuledTime;
};

class MidiScheduler
{
public:
    MidiScheduler();
    
    void PostMidiNote(const uint_8 channel, const uint_8 noteNumber, const uint_8 velocity, const int durationInSamples, const int timeStamp);
    void ProcessMidiPosts(juce::MidiBuffer& midiMessages, int bufferLength, int64_t endOfBufferPosition);
    void ClearAllData(juce::MidiBuffer& midiMessages);
    
private:
    std::vector<ScheduledMidi> mScheduledMidiMessages;
};
