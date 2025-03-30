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
                                   const int bufferLength,
                                   juce::MidiBuffer& midiMessages)
{
    if (transportData.isPlaying)
    {
        const double gridResolution = static_cast<double>(transportData.sampleRate) * (60.0 / (transportData.bpm * mBpmDivide));
        const int stepCount = static_cast<int>(ceil(static_cast<double>(transportData.timeInSamples) / gridResolution));
        const int nextTickTime = static_cast<int>(gridResolution * stepCount);
        
        if (transportData.timeInSamples + bufferLength >= nextTickTime)
        {
            int scheduledTime = static_cast<int>(nextTickTime - transportData.timeInSamples);
            mMidiScheduler.PostMidi(1, 64, 127, 11025, scheduledTime);
            
            mMidiScheduler.ProcessMidiPosts(midiMessages, transportData.timeInSamples + bufferLength);
        }
    }
}
