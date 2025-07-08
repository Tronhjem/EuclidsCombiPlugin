#pragma once

#include <JuceHeader.h>
#include <vector>

#include "Types.h"
#include "StepData.h"

enum class MidiType : uChar
{
    Note = 0,
    CC = 1
};

struct ScheduledMidi
{
    juce::MidiMessage midiData;
    int schuledTime;
};

class MidiScheduler
{
public:
    MidiScheduler();
    
    void PostMidiNote(const uChar channel, const uChar noteNumber, const uChar velocity, const int durationInSamples, const int timeStamp);
    void PostMidiCC(const uChar channel, const uChar cc, const uChar value, const int timeStamp);
    void PostStepData(const StepData& data, const int nextTickTime, const int currentNoteLength);
    void ProcessMidiPosts(juce::MidiBuffer& midiMessages, int bufferLength, int64_t endOfBufferPosition);
    void ClearAllData(juce::MidiBuffer& midiMessages);
    
private:
    std::vector<ScheduledMidi> mScheduledMidiMessages;
};
