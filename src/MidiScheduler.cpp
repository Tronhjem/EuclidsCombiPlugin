#include "MidiScheduler.h"
#include <algorithm>

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
    const int clampedChannel = std::clamp(static_cast<int>(channel), 0, 16);
    const int clampedNote = std::clamp(static_cast<int>(noteNumber), 0, 127);
    const unsigned char clampedVelocity = static_cast<unsigned char>(std::clamp(static_cast<int>(velocity), 0, 127));

    mScheduledMidiMessages.emplace_back(ScheduledMidi{juce::MidiMessage::noteOn(clampedChannel, clampedNote, clampedVelocity), timeStamp});
    mScheduledMidiMessages.emplace_back(ScheduledMidi{juce::MidiMessage::noteOff(clampedChannel, clampedNote), timeStampOff});
}

void MidiScheduler::PostMidiCC(const uChar channel,
                               const uChar cc,
                               const uChar value,
                               const int timeStamp)
{
    const int clampedChannel = std::clamp(static_cast<int>(channel), 0, 16);
    const int clampedCC = std::clamp(static_cast<int>(cc), 0, 127);
    const unsigned char clampedValue = static_cast<unsigned char>(std::clamp(static_cast<int>(value), 0, 127));

    mScheduledMidiMessages.emplace_back(ScheduledMidi{juce::MidiMessage::controllerEvent(clampedChannel, clampedCC, clampedValue), timeStamp});
}

void MidiScheduler::PostStepData(const SequenceStep& data, const int nextTickTime, const int currentNoteLength)
{
    if (data.mShouldTrigger.GetValue(0) < 1)
        return;
    
    if(data.mType == StepType::NOTE)
    {
        PostMidiNote(data.mChannel.GetValue(0), data.mFirstData.GetValue(0), data.mSecondData.GetValue(0), currentNoteLength, nextTickTime);
    }
    else if(data.mType == StepType::CC)
    {
        PostMidiCC(data.mChannel.GetValue(0), data.mFirstData.GetValue(0), data.mSecondData.GetValue(0), nextTickTime);
    }
}

void MidiScheduler::PostStepData(const SequenceStep& data, const int nextTickTime)
{
    if (data.mShouldTrigger.GetValue(0) < 1)
        return;
    
    if(data.mType == StepType::NOTE)
    {
        PostMidiNote(data.mChannel.GetValue(0), data.mFirstData.GetValue(0), data.mSecondData.GetValue(0), data.mDuration, nextTickTime);
    }
    else if(data.mType == StepType::CC)
    {
        PostMidiCC(data.mChannel.GetValue(0), data.mFirstData.GetValue(0), data.mSecondData.GetValue(0), nextTickTime);
    }
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
