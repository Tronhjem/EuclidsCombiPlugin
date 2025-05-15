#pragma once

#include <JuceHeader.h>
#include <memory>

#include "TransportData.h"
#include "MidiScheduler.h"
#include "VM.h"
#include "FileLoader.h"

class ORchestraEngine
{
public:
    ORchestraEngine();
    ~ORchestraEngine();
    void Tick(const TransportData& transportData, const int bufferLength, juce::MidiBuffer& midiMessages);
    char* GetLoadedFileData();
    char* LoadFile(std::string& filePath);
    void SaveFile(std::string& data);
    
private:
    double mBpmDivide = 1.0;
    bool mIsVMInit = false;
    MidiScheduler mMidiScheduler;
    std::unique_ptr<VM> mVM;
    std::unique_ptr<FileLoader> mFileLoader;
};
