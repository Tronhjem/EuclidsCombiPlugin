/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EuclidCombinatorAudioProcessorEditor::EuclidCombinatorAudioProcessorEditor (EuclidCombinatorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    const int height = 20;
    const int space = 5;
    const int start = 10;
    
    isPlayingLabel.setBounds(10, start + (height * 0) + space, 200, height);
    addAndMakeVisible(&isPlayingLabel);
    
    barCountLabel.setBounds(10, start + (height * 1) + space, 200, height);
    addAndMakeVisible(&barCountLabel);
    
    beatCountLabel.setBounds(10, start + (height * 2) + space , 200, height);
    addAndMakeVisible(&beatCountLabel);
    
    ppqLabel.setBounds(10, start + (height * 3) + space, 200, height);
    addAndMakeVisible(&ppqLabel);
}

EuclidCombinatorAudioProcessorEditor::~EuclidCombinatorAudioProcessorEditor()
{
}

//==============================================================================
void EuclidCombinatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    
    const TransportData& data = audioProcessor.GetPositionData();
    
#if (JucePlugin_Build_Standalone == 0)
    isPlayingLabel.setText(data.isPlaying ? "TRUE" : "FALSE", juce::dontSendNotification);
#endif
    
//    int nextstep = audioProcessor.mStepCount * audioProcessor.mGridResolution;
//    auto stepCountString = juce::String(audioProcessor.mStepCount);
//    auto nextStepString = juce::String(nextstep);
//    auto sampleString = juce::String(data.timeInSamples);
    
//    barCountLabel.setText(stepCountString, juce::dontSendNotification);
//    beatCountLabel.setText(nextStepString, juce::dontSendNotification);
//    ppqLabel.setText(sampleString, juce::dontSendNotification);
}

void EuclidCombinatorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
