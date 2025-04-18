#include <cmath>
#include "EuclidsCombinator.hpp"

EuclidsCombinatorEngine::EuclidsCombinatorEngine()
{
    std::array<uChar, 8> logic2 = {1,1,1,1,0,0,0,0};
    std::array<uChar, 8> logic =  {1,0,0,1,0,1,1,1};
    LogicSequence seq1 {&logic[0], 8};
    mTracks.emplace_back(Track{seq1, 1, 64});
    
    LogicSequence seq2 {&logic2[0], 8};
    mTracks.emplace_back(Track{seq2, 1, 64 + 12});
    
    mVM = new VM();
    mVM->Prepare("/Users/christiantronhjem/dev/EuclidsCombiPlugin/Source/Parsing/myFile.e");
    mVM->Run();
}

EuclidsCombinatorEngine::~EuclidsCombinatorEngine()
{
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
            for(Track& track : mTracks)
            {
                track.Tick(mMidiScheduler, nextTickTime, stepCount);
            }
        }
        
        // Process all Midi.
        mMidiScheduler.ProcessMidiPosts(midiMessages, bufferLength, endOfBufferPosition);
    }
    else
    {
        mMidiScheduler.ClearAllData(midiMessages);
    }
}
