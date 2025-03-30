//
//  MidiScheduler.cpp
//
//  Created by Christian Tronhjem on 30.03.25.
//

#include "MidiScheduler.h"

MidiScheduler::MidiScheduler()
{
    mScheduledMidiMessages.resize(10);
}

void MidiScheduler::PostMidi(uint_8 channel, uint_8 noteNumber, uint_8 velocity, int durationInSamples, int timeStamp)
{
    int timeStampOn = timeStamp;
    int timeStampOff = timeStamp + durationInSamples;
    
    // Note On
    mScheduledMidiMessages.emplace_back(ScheduledMidi{
                                            channel,
                                            noteNumber,
                                            velocity,
                                            timeStampOn});
    
    // Note Off
    mScheduledMidiMessages.emplace_back(ScheduledMidi{
                                            channel,
                                            noteNumber,
                                            0,
                                            timeStampOff});
}

void MidiScheduler::ProcessMidiPosts(juce::MidiBuffer &midiMessages, int64_t positionInSamples)
{
    for(unsigned long i = mScheduledMidiMessages.size() - 1; i >= 0; --i)
    {
        int scheduledTime = 0;
        if (mScheduledMidiMessages[i].schuledTime) // ?????? check here.
        {
            
            if (mScheduledMidiMessages[i].velocity > 0)
            {
                auto on = juce::MidiMessage::noteOn(1, 64, (uint_8)120);
                midiMessages.addEvent(on, scheduledTime);
            }
            else
            {
                auto on = juce::MidiMessage::noteOff(1, 64);
                midiMessages.addEvent(on, scheduledTime);
            }
            
            mScheduledMidiMessages.erase(mScheduledMidiMessages.begin() + i);
        }
    }
}
