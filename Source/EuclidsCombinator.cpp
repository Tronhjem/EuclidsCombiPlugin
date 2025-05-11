#include <cmath>

#include "EuclidsCombinator.h"

EuclidsCombinatorEngine::EuclidsCombinatorEngine() :
    mBpmDivide(1.0),
    mIsVMInit(false)
{
    
    mVM = std::make_unique<VM>();
    mFileLoader = std::make_unique<FileLoader>();
    
    std::string filePath {"/Users/christiantronhjem/dev/EuclidsCombiPlugin/data/myFile.e"};
    
    bool fileLoaded = mFileLoader->LoadFile(filePath);
    if(fileLoaded)
        mIsVMInit = mVM->Prepare(mFileLoader->GetFileStart());
}

EuclidsCombinatorEngine::~EuclidsCombinatorEngine()
{
}

void EuclidsCombinatorEngine::SaveFile(std::string& data)
{
    mVM->Reset();
    bool fileSaved = mFileLoader->SaveFile(data);
    
    if(fileSaved)
        mIsVMInit = mVM->Prepare(mFileLoader->GetFileStart());
}

char* EuclidsCombinatorEngine::LoadFile(std::string& filePath)
{
    mFileLoader->LoadFile(filePath);
    return mFileLoader->GetFileStart();
}

char* EuclidsCombinatorEngine::GetLoadedFileData()
{
    return mFileLoader->GetFileStart();
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
        if (mIsVMInit && endOfBufferPosition >= nextTickTime)
        {
            mVM->Tick(mMidiScheduler, nextTickTime, stepCount);
        }
    
        // Process all Midi.
        mMidiScheduler.ProcessMidiPosts(midiMessages, bufferLength, endOfBufferPosition);
    
    }
    else
    {
        mMidiScheduler.ClearAllData(midiMessages);
    }
}
