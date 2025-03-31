//
//  MidiScheduler.cpp
//
//  Created by Christian Tronhjem on 30.03.25.
//

#include "MidiScheduler.h"

MidiScheduler::MidiScheduler()
{
//    mScheduledMidiMessages.resize(10);
}

void MidiScheduler::PostMidiNote(uint_8 channel,
                             uint_8 noteNumber,
                             uint_8 velocity,
                             int durationInSamples,
                             int timeStamp)
{
    // Note On
    mScheduledMidiMessages.emplace_back(ScheduledMidi{juce::MidiMessage::noteOn(channel, noteNumber, velocity), timeStamp});
    
    int timeStampOff = timeStamp + durationInSamples;
    // Note Off
    mScheduledMidiMessages.emplace_back(ScheduledMidi{juce::MidiMessage::noteOff(channel, noteNumber), timeStampOff});
}

void MidiScheduler::ProcessMidiPosts(juce::MidiBuffer& midiMessages,
                                     int nextTickTime,
                                     int bufferLength,
                                     int64_t endOfBufferPosition)
{
    for(int i = (int)mScheduledMidiMessages.size() - 1; i >= 0; --i)
    {
        const ScheduledMidi& message = mScheduledMidiMessages[i];
        if (message.schuledTime <= endOfBufferPosition)
        {
            const int relativePositionInBuffer = static_cast<int>(message.schuledTime - (endOfBufferPosition - bufferLength));
            midiMessages.addEvent(message.midiData, relativePositionInBuffer);
            
            mScheduledMidiMessages.erase(mScheduledMidiMessages.begin() + i);
        }
    }
}
