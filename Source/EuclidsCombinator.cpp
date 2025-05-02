#include <cmath>

#include "EuclidsCombinator.h"

EuclidsCombinatorEngine::EuclidsCombinatorEngine() :
    mBpmDivide(1.0),
    mIsVMInit(false)
{
    
    mVM = std::make_unique<VM>();
    
    if(mVM->Prepare("/Users/christiantronhjem/dev/EuclidsCombiPlugin/data/myFile.e"))
    {
        mIsVMInit = mVM->ProcessOpCodes();
    }
}

EuclidsCombinatorEngine::~EuclidsCombinatorEngine()
{
}

void EuclidsCombinatorEngine::Tick(const TransportData& transportData,
                                   const int bufferLength,
                                   juce::MidiBuffer& midiMessages)
{
    
//    if (transportData.isPlaying)
//    {
    
        const double gridResolution = static_cast<double>(transportData.sampleRate) * (60.0 / (transportData.bpm * mBpmDivide));
        const int stepCount = static_cast<int>(ceil(static_cast<double>(transportData.timeInSamples) / gridResolution));
        const int nextTickTime = static_cast<int>(gridResolution * stepCount);
        const int endOfBufferPosition = static_cast<int>(transportData.timeInSamples + bufferLength);
        
        // if the end of the buffer is longer than the next tick time
        // Check if we should tick in this buffer.
        if (endOfBufferPosition >= nextTickTime)
        {
            mVM->Tick(mMidiScheduler, nextTickTime, stepCount);
        }
    
        // Process all Midi.
        mMidiScheduler.ProcessMidiPosts(midiMessages, bufferLength, endOfBufferPosition);
    
//    }
//    else
//    {
//        mMidiScheduler.ClearAllData(midiMessages);
//    }
}
