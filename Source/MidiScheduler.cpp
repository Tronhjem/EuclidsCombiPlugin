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

void MidiScheduler::PostMidiNote(const uint_8 channel,
                             const uint_8 noteNumber,
                             const uint_8 velocity,
                             const int durationInSamples,
                             const int timeStamp)
{
    const int timeStampOff = timeStamp + durationInSamples;
    mScheduledMidiMessages.emplace_back(ScheduledMidi{juce::MidiMessage::noteOn(channel, noteNumber, velocity), timeStamp});
    mScheduledMidiMessages.emplace_back(ScheduledMidi{juce::MidiMessage::noteOff(channel, noteNumber), timeStampOff});
}

void MidiScheduler::ProcessMidiPosts(juce::MidiBuffer& midiMessages,
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

void MidiScheduler::ClearAllData(juce::MidiBuffer& midiMessages)
{
    for(int i = (int)mScheduledMidiMessages.size() - 1; i >= 0; --i)
    {
        const ScheduledMidi& message = mScheduledMidiMessages[i];
        if (message.midiData.isNoteOff())
            midiMessages.addEvent(message.midiData, 0);
        if (message.midiData.isNoteOn())
            midiMessages.addEvent(juce::MidiMessage::noteOff(message.midiData.getChannel(),
                                                             message.midiData.getNoteNumber()), 0);
        
        mScheduledMidiMessages.erase(mScheduledMidiMessages.begin() + i);
    }
}
