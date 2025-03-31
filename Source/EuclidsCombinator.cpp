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
        const int endOfBufferPosition = static_cast<int>(transportData.timeInSamples + bufferLength);
        
        // if the end of the buffer is longer than the next tick time
        // Check if we should tick in this buffer.
        if (endOfBufferPosition >= nextTickTime)
        {
            // Evaluate outputs.
            //...for output in outpus:
                // output.tick();
            
            
            // Dummy post
            mMidiScheduler.PostMidi(MidiType::Note, 1, 64, 127, 11025, nextTickTime);
            
            // Process all Midi.
        }
        
        mMidiScheduler.ProcessMidiPosts(midiMessages, nextTickTime, bufferLength, endOfBufferPosition);
    }
}
