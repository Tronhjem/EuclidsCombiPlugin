#include <cmath>

#include "ORchestraEngine.h"
#include "ScopedTimer.h"

ORchestraEngine::ORchestraEngine() :
    mBpmDivide(1.0),
    mIsVMInit(false)
{
//    for(int i = 0; i < STEP_BUFFER_SIZE; ++i)
//    {
////        mStepRingBuffer[i] = std::vector<StepData>();
//        mStepRingBuffer[i].clear();
//        auto s = mStepRingBuffer[i].size();
//    }
    std::cout << &mStepRingBuffer << std::endl;
    
    mCurrentGlobalStep.store(0, std::memory_order_release);
    mVM = std::make_unique<VM>();
    mFileLoader = std::make_unique<FileLoader>();
    
//    std::string filePath {"/Users/christiantronhjem/dev/ORchestra/data/myFile.txt"};
//
//    bool fileLoaded = mFileLoader->LoadFile(filePath);
//    if(fileLoaded)
//        mIsVMInit = mVM->Prepare(mFileLoader->GetFileStart());
}

ORchestraEngine::~ORchestraEngine()
{
}

void ORchestraEngine::SaveFile(std::string& data)
{
    mVM->Reset();
    mReadySteps.store(0, std::memory_order_release);
    
    bool fileSaved = mFileLoader->SaveFile(data);
    
    if(fileSaved)
        mIsVMInit = mVM->Prepare(mFileLoader->GetFileStart());
    
    if(mIsVMInit)
    {
        PreProcessSteps();
    }
}

char* ORchestraEngine::LoadFile(std::string& filePath)
{
    mVM->Reset();
    mReadySteps.store(0, std::memory_order_release);
    
    bool loaded = mFileLoader->LoadFile(filePath);
    if (loaded)
    {
        mIsVMInit = mVM->Prepare(mFileLoader->GetFileStart());
        
        if(mIsVMInit)
        {
            PreProcessSteps();
        }
        
        return mFileLoader->GetFileStart();
    }
    
    return nullptr;
}

void ORchestraEngine::PreProcessSteps()
{
    if (!mIsVMInit)
        return;
    
    ScopedTimer timer {"Preprocess steps"};
    
    const int stepsToProcess = STEP_BUFFER_SIZE - mReadySteps;
    
    for(int i = 0; i < stepsToProcess; ++i)
    {
        int step = mCurrentGlobalStep - 1 - i; // always start behind the Global Step.
        if (step < 0)
            step += STEP_BUFFER_SIZE;
        
        const int stepWrapped = step % STEP_BUFFER_SIZE;
        
        std::vector<StepData>& currentData = mStepRingBuffer[stepWrapped];
        currentData.clear();
        
        mVM->Tick(currentData, step);
        
        mReadySteps.fetch_add(1, std::memory_order_acq_rel);
    }
}

char* ORchestraEngine::GetLoadedFileData()
{
    return mFileLoader->GetFileStart();
}

void ORchestraEngine::Tick(const TransportData& transportData,
                                   const int bufferLength,
                                   juce::MidiBuffer& midiMessages)
{
    if (transportData.isPlaying)
    {
        const double gridResolution = static_cast<double>(transportData.sampleRate) * (60.0 / (transportData.bpm * mBpmDivide));
        const int currentStep = static_cast<int>(ceil(static_cast<double>(transportData.timeInSamples) / gridResolution));
        
        // Check if we skipped count, to regenerate everything.
        const int stepDifference = currentStep - mCurrentGlobalStep.load();
        mCurrentGlobalStep.store(currentStep, std::memory_order_release);
        
        if (stepDifference > 1 || stepDifference < 0)
        {
            mReadySteps.store(1, std::memory_order_release);
            // should it be 1 or 0?
            // Do we want to move it entirely down to 0 or still keep the current step we might trigger?
        }
        
        const int nextTickTime = static_cast<int>(gridResolution * currentStep);
        const int endOfBufferPosition = static_cast<int>(transportData.timeInSamples + bufferLength);
        
        //TODO: Move to other thread.
        if(mReadySteps < STEP_BUFFER_SIZE)
            PreProcessSteps();
        
        // if the end of the buffer is longer than the next tick time
        // Check if we should tick in this buffer.
        if (mIsVMInit && endOfBufferPosition >= nextTickTime)
        {
            const int wrappedGlobalStep = currentStep % STEP_BUFFER_SIZE;
            const std::vector<StepData>& currentData = mStepRingBuffer[wrappedGlobalStep];
            
            for(const StepData& step : currentData)
            {
                mMidiScheduler.PostStepData(step, nextTickTime);
            }
            
            mReadySteps.fetch_sub(1, std::memory_order_acq_rel);
        }
    
        // Process all Midi.
        mMidiScheduler.ProcessMidiPosts(midiMessages, bufferLength, endOfBufferPosition);
    }
    else
    {
        mMidiScheduler.ClearAllData(midiMessages);
    }
}
