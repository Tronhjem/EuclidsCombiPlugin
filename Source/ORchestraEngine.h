#pragma once

#include <JuceHeader.h>
#include <memory>

#include "TransportData.h"
#include "MidiScheduler.h"
#include "VM.h"
#include "FileLoader.h"
#include "StepData.h"

constexpr int STEP_BUFFER_SIZE = 32;

class ORchestraEngine
{
public:
    ORchestraEngine();
    ~ORchestraEngine();
    void Tick(const TransportData& transportData, const int bufferLength, juce::MidiBuffer& midiMessages);
    char* GetLoadedFileData();
    char* LoadFile(std::string& filePath);
    void SaveFile(std::string& data);
    std::array<std::vector<StepData>, STEP_BUFFER_SIZE>& GetStepData() { return mStepRingBuffer; }
    int GetGlobalStepCount() { return mCurrentGlobalStep.load(); }
    
private:
    void PreProcessSteps();
    double mBpmDivide = 1.0;
    bool mIsVMInit = false;
    std::atomic<int> mReadySteps;
    std::atomic<int> mCurrentGlobalStep;
    
    std::unique_ptr<VM> mVM;
    std::unique_ptr<FileLoader> mFileLoader;
    std::array<std::vector<StepData>,STEP_BUFFER_SIZE> mStepRingBuffer;
    MidiScheduler mMidiScheduler;
};
