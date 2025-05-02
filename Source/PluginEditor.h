/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"

//==============================================================================
/**
*/
class EuclidCombinatorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    EuclidCombinatorAudioProcessorEditor (EuclidCombinatorAudioProcessor&);
    ~EuclidCombinatorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    EuclidCombinatorAudioProcessor& audioProcessor;
    juce::Label isPlayingLabel;
    juce::Label barCountLabel;
    juce::Label beatCountLabel;
    juce::Label ppqLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EuclidCombinatorAudioProcessorEditor)
};
