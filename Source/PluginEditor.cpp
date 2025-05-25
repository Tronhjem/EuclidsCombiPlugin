/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr int MARGIN = 20;
constexpr int MARGIN_X2 = MARGIN * 2;

//==============================================================================
ORchestraAudioProcessorEditor::ORchestraAudioProcessorEditor (ORchestraAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (WINDOW_WIDTH, WINDOW_HEIGHT);
    
//    codeEditor.reset(new juce::CodeEditorComponent(codeDocument, &tokeniser));
//    codeEditor->setTabSize(4, true);
//    codeEditor->setLineNumbersShown(true);
//    codeEditor->loadContent("print(\"Hello, world!\")");
    
    togglePlay.setBounds(20, 20, 50, 20);
    togglePlay.addListener(this);
    addAndMakeVisible(togglePlay);
    
    codeEditor.setMultiLine(true);
    
    codeEditor.setReturnKeyStartsNewLine(true);
    codeEditor.setScrollbarsShown(true);
    codeEditor.setCaretVisible(true);
//    codeEditor.setPopupMenuEnabled(true);
    codeEditor.setBounds(20, 60, 300, 200);
    codeEditor.addListener(this);
    addAndMakeVisible(codeEditor);
    
    saveFile.setBounds(20, 220+60, 50, 20);
    saveFile.addListener(this);
    addAndMakeVisible(saveFile);
    
    loadFile.setBounds(70+20, 220+60, 50, 20);
    loadFile.addListener(this);
    addAndMakeVisible(loadFile);
    
    timeline.setBounds(20, 320, 760, 260);
    addAndMakeVisible(timeline);
    timeline.SetProcessor(&audioProcessor);
}

ORchestraAudioProcessorEditor::~ORchestraAudioProcessorEditor()
{
}

void ORchestraAudioProcessorEditor::textEditorTextChanged(juce::TextEditor& editor)
{
}

void ORchestraAudioProcessorEditor::buttonClicked(juce::Button* button)
{
//    std::string filePath {"/Users/christiantronhjem/dev/EuclidsCombiPlugin/data/myFile.e"};
    
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
        chooser.launchAsync(folderChooserFlags, [this] (const juce::FileChooser& fc)
        {
            juce::File file = chooser.getResult();
            std::string filePath {file.getFullPathName().toRawUTF8()};
            char* data = audioProcessor.LoadFile(filePath);
            juce::String dataAsString {data};
            codeEditor.setText(dataAsString);
        });
    }
}

//==============================================================================
void ORchestraAudioProcessorEditor::paint (juce::Graphics& g)
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

void ORchestraAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
