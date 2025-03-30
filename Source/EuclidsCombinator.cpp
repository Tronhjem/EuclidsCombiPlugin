//
//  EuclidsCombinator.cpp
//
//  Created by Christian Tronhjem on 29.03.25.
//

#include "EuclidsCombinator.hpp"
#include <cmath>

EuclidsCombinatorEngine::EuclidsCombinatorEngine()
{
    mOutput = new Output(1);
}

EuclidsCombinatorEngine::~EuclidsCombinatorEngine()
{
    if(mOutput != nullptr)
        delete mOutput;
}

void EuclidsCombinatorEngine::Tick(const TransportData& transportData,
                                   juce::AudioBuffer<float>& buffer,
                                   juce::MidiBuffer& midiMessages)
{
    const int bufferLength = buffer.getNumSamples();
    
  
    
    if (transportData.isPlaying)
    {
        const double gridResolution = static_cast<double>(transportData.sampleRate) * (60.0 / (transportData.bpm * mBpmDivide));
        const int stepCount = static_cast<int>(ceil(static_cast<double>(transportData.timeInSamples) / gridResolution));
        const int nextSample = static_cast<int>(gridResolution * stepCount);
        
        if (transportData.timeInSamples + bufferLength >= nextSample)
        {
            int scheduledTime = static_cast<int>(nextSample - transportData.timeInSamples);

            // Fire MIDI notes.
            auto on = juce::MidiMessage::noteOn(1, 64, (uint_8)120);
            midiMessages.addEvent(on, scheduledTime);

            mTimeSincePost = 0;
        }
        
        mTimeSincePost += bufferLength;
    }
    
    if(mTimeSincePost >= 11025)
    {
        auto off = juce::MidiMessage::noteOff(1, 64);
        midiMessages.addEvent(off, 0);
        mTimeSincePost = 0;
    }
}
