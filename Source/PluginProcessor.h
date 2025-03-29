/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

constexpr int LengthOfTrigger = 8;

struct TransportData
{
    double bpm = 0.0;
    double timeInSamples = 0;
    bool isPlaying = false;

//    double time = 0.0;
//    double ppq = 0.0;
//    int bar = 0;
//    int beat = 0;
};

class EuclidCombinatorAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    EuclidCombinatorAudioProcessor();
    ~EuclidCombinatorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    const TransportData& GetPositionData() { return mTransportData; };
    
    TransportData mTransportData;
    int mGridResolution = (int)((44100.f / 1000.f) * 500.f);
    double mSampleRate = 44100.0;
    double mBpmDivide = 4.0;
    
    int mTriggers[LengthOfTrigger] = {1, 0, 0, 0, 1, 0, 1, 1};
    const int mNoteLength = 11025;
    
    // ================
    // UNUSED
    int mStepCount = 0;
    // ================
    
private:
    //==============================================================================
    void FillPositionData(TransportData& data);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EuclidCombinatorAudioProcessor)
};
