#include <cmath>

#include "ORchestraEngine.h"
#include "ScopedTimer.h"

ORchestraEngine::ORchestraEngine() :
    mIsVMInit(false)
{
    mCurrentGlobalStep.store(0, std::memory_order_release);
    mVM = std::make_unique<VM>();
    mFileLoader = std::make_unique<FileLoader>();
    
    workerThread = std::thread([this]() { WorkerThreadLoop(); });
}

ORchestraEngine::~ORchestraEngine()
{
    shouldExit.store(true);
    if (workerThread.joinable())
        workerThread.join();
}

void ORchestraEngine::SaveFile(std::string& data)
{
    const bool fileSaved = mFileLoader->SaveFile(data);
    
    if(fileSaved)
    {
        mIsVMInit.store(false, std::memory_order_release);
        mReadySteps.store(0, std::memory_order_release);
        mVM->Reset();
        
        mIsVMInit.store(mVM->Prepare(mFileLoader->GetFileStart()));
    }
}

char* ORchestraEngine::LoadFile(std::string& filePath)
{
    const bool loaded = mFileLoader->LoadFile(filePath);
    
    if (loaded)
    {
        mIsVMInit.store(false, std::memory_order_release);
        mReadySteps.store(0, std::memory_order_release);
        mVM->Reset();
        
        mIsVMInit.store(mVM->Prepare(mFileLoader->GetFileStart()));
        return mFileLoader->GetFileStart();
    }
    
    return nullptr;
}

std::vector<LogEntry>& ORchestraEngine::GetErrors()
{
    return mVM->GetErrors();
}

void ORchestraEngine::WorkerThreadLoop()
{
    while (!shouldExit.load())
    {
        PreProcessSteps();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void ORchestraEngine::PreProcessSteps()
{
    if (!mIsVMInit.load())
        return;
    
    const int readySteps = mReadySteps.load();
    const int stepsToProcess = STEP_BUFFER_SIZE - 1 - readySteps; // leave the last step unprocessed.
    
    if(stepsToProcess < HALF_STEP_BUFFER_SIZE)
        return;
    
    ScopedTimer timer {"PreProcess"};
    
    int currentGlobalStep = mCurrentGlobalStep.load();
    for(int i = 0; i < stepsToProcess; ++i)
    {
        const int step = currentGlobalStep + readySteps + i;
        const int stepWrapped = step % STEP_BUFFER_SIZE;
        // tick needs global step and StepData needs it wrapped for ring buffer.
        
        std::vector<SequenceStep>& currentData = mStepRingBuffer[stepWrapped];
        currentData.clear();
        
        mVM->Tick(currentData, step);
        
        mReadySteps.fetch_add(1, std::memory_order_acq_rel);
    }
}

char* ORchestraEngine::GetLoadedFileData()
{
    return mFileLoader->GetFileStart();
}

std::string ORchestraEngine::GetSavedFilePath()
{
    return mFileLoader->GetSavedFilePath();
}

void ORchestraEngine::Tick(const TransportData& transportData,
                           const int bufferLength,
                           juce::MidiBuffer& midiMessages)
{
    if (transportData.isPlaying)
    {
        const double samplesPerStep = static_cast<double>(transportData.sampleRate) * (60.0 / (transportData.bpm * transportData.bpmDivision));
        const int currentStep = static_cast<int>(ceil(static_cast<double>(transportData.timeInSamples) / samplesPerStep));
        
        // Check if we skipped count, to regenerate everything.
        const int stepDifference = currentStep - mCurrentGlobalStep.load();
        mCurrentGlobalStep.store(currentStep, std::memory_order_release);
        
        if (stepDifference > 1 || stepDifference < 0)
        {
            mReadySteps.store(1, std::memory_order_release);
            // should it be 1 or 0?
            // Do we want to move it entirely down to 0 or still keep the current step we might trigger?
        }
        
        const int nextStepInSamples = static_cast<int>(samplesPerStep * currentStep);
        const int endOfBufferInSamples = static_cast<int>(transportData.timeInSamples + bufferLength);
        
        // if the end of the buffer is longer than the next tick time
        // Check if we should tick in this buffer.
        if (mIsVMInit && endOfBufferInSamples >= nextStepInSamples)
        {
            samplesSinceLastStep = transportData.timeInSamples;
            const int wrappedGlobalStep = currentStep % STEP_BUFFER_SIZE;
            const std::vector<SequenceStep>& currentData = mStepRingBuffer[wrappedGlobalStep];
            
            for(const SequenceStep& step : currentData)
            {
                const int length = static_cast<int>(step.mShouldTrigger.GetLength());
                for(int i = 0; i < length; ++i)
                {
                    const uChar shouldTrigger = step.mShouldTrigger.GetActiveValueAtIndex(i);
                    if(!shouldTrigger)
                        continue;
                    
                    const int timeStamp = nextStepInSamples + i * (samplesPerStep / length);
                    const uChar channel = step.mChannel.GetActiveValueAtIndex(i);
                    
                    if(step.mType == StepType::NOTE)
                    {
                        const uChar noteNumber = step.mFirstData.GetActiveValueAtIndex(i);
                        const uChar velocity = step.mSecondData.GetActiveValueAtIndex(i);
                        mMidiScheduler.PostMidiNote(channel, noteNumber, velocity, step.mDuration, timeStamp);
                    }
                    else if(step.mType == StepType::CC)
                    {
                        const uChar ccNumber = step.mFirstData.GetActiveValueAtIndex(i);
                        const uChar ccValue = step.mSecondData.GetActiveValueAtIndex(i);
                        mMidiScheduler.PostMidiCC(channel, ccNumber, ccValue, timeStamp);
                    }
                }
            }
            
            mReadySteps.fetch_sub(1, std::memory_order_acq_rel);
        }
    
        // Process all Midi.
        mMidiScheduler.ProcessMidiPosts(midiMessages, bufferLength, endOfBufferInSamples);
    }
    else
    {
        mMidiScheduler.ClearAllData(midiMessages);
    }
}
