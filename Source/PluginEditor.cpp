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
    setSize (800, 600);
    
    const int height = 20;
    const int space = 5;
    const int start = 10;
    
//    isPlayingLabel.setBounds(10, start + (height * 0) + space, 200, height);
//    addAndMakeVisible(&isPlayingLabel);
//
//    barCountLabel.setBounds(10, start + (height * 1) + space, 200, height);
//    addAndMakeVisible(&barCountLabel);
//
//    beatCountLabel.setBounds(10, start + (height * 2) + space , 200, height);
//    addAndMakeVisible(&beatCountLabel);
//
//    ppqLabel.setBounds(10, start + (height * 3) + space, 200, height);
//    addAndMakeVisible(&ppqLabel);
    
    codeEditor.setMultiLine(true);
    codeEditor.setReturnKeyStartsNewLine(true);
    codeEditor.setScrollbarsShown(true);
    codeEditor.setCaretVisible(true);
    codeEditor.setPopupMenuEnabled(true);
    codeEditor.setBounds(20, 60, 300, 200);
    codeEditor.addListener(this);
    addAndMakeVisible(codeEditor);
    
    
    togglePlay.setBounds(20, 20, 50, 20);
    togglePlay.addListener(this);
    addAndMakeVisible(togglePlay);
    
    saveFile.setBounds(20, 220+60, 50, 20);
    saveFile.addListener(this);
    addAndMakeVisible(saveFile);
    
    loadFile.setBounds(70+20, 220+60, 50, 20);
    loadFile.addListener(this);
    addAndMakeVisible(loadFile);
    buttonClicked(&loadFile);
}

EuclidCombinatorAudioProcessorEditor::~EuclidCombinatorAudioProcessorEditor()
{
}

void EuclidCombinatorAudioProcessorEditor::textEditorTextChanged(juce::TextEditor& editor)
{
}

void EuclidCombinatorAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    std::string filePath {"/Users/christiantronhjem/dev/EuclidsCombiPlugin/data/myFile.e"};
    
    if(button == &togglePlay)
    {
        audioProcessor.IsRunning = !audioProcessor.IsRunning;
        togglePlay.setButtonText(audioProcessor.IsRunning ? "Stop" : "Play");
    }
    
    if(button == &saveFile)
    {
        audioProcessor.IsRunning = false;
        togglePlay.setButtonText("Play");
        
        juce::String text = codeEditor.getText();
        std::string utf8Text = text.toRawUTF8(); // or toStdString() if you want std::string
        audioProcessor.SaveFile(utf8Text);
    }
    
    if(button == &loadFile)
    {
        char* data = audioProcessor.LoadFile(filePath);
        juce::String dataAsString {data};
        codeEditor.setText(dataAsString);
    }
}

//==============================================================================
void EuclidCombinatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    
//    const TransportData& data = audioProcessor.GetPositionData();
    
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
