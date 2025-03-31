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
    int timeStampOn = timeStamp;
    int timeStampOff = timeStamp + durationInSamples;
    
    // Note On
    mScheduledMidiMessages.emplace_back(ScheduledMidi{
                                            channel,
                                            noteNumber,
                                            velocity,
                                            MidiType::Note,
                                            timeStampOn});
    
    // Note Off
    mScheduledMidiMessages.emplace_back(ScheduledMidi{
                                            channel,
                                            noteNumber,
                                            0,
                                            MidiType::Note,
                                            timeStampOff});
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
            if (static_cast<int>(message.velocity) > 0)
            {
                midiMessages.addEvent(juce::MidiMessage::noteOn(1, 64, (uint_8)120), relativePositionInBuffer);
            }
            else
            {
                midiMessages.addEvent(juce::MidiMessage::noteOff(1, 64), relativePositionInBuffer);
            }
            
            mScheduledMidiMessages.erase(mScheduledMidiMessages.begin() + i);
        }
    }
}
