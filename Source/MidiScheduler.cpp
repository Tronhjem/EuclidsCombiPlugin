#include "MidiScheduler.h"

MidiScheduler::MidiScheduler()
{
}

void MidiScheduler::PostMidiNote(const uChar channel,
                                 const uChar noteNumber,
                                 const uChar velocity,
                                 const int durationInSamples,
                                 const int timeStamp)
{
    const int timeStampOff = timeStamp + durationInSamples;
    mScheduledMidiMessages.emplace_back(ScheduledMidi{juce::MidiMessage::noteOn(channel, noteNumber, velocity), timeStamp});
    mScheduledMidiMessages.emplace_back(ScheduledMidi{juce::MidiMessage::noteOff(channel, noteNumber), timeStampOff});
}

void MidiScheduler::PostMidiCC(const uChar channel,
                               const uChar cc,
                               const uChar value,
                               const int timeStamp)
{
    mScheduledMidiMessages.emplace_back(ScheduledMidi{juce::MidiMessage::controllerEvent(channel, cc, value), timeStamp});
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
