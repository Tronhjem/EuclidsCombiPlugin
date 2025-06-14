/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
//constexpr int MARGIN = 20;

//==============================================================================
ORchestraAudioProcessorEditor::ORchestraAudioProcessorEditor (ORchestraAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (WINDOW_WIDTH, WINDOW_HEIGHT);
    
    mGeneralLookAndFeel = std::make_unique<GeneralLookAndFeel>();
    mButtonLookAndFeel = std::make_unique<ButtonLookAndFeel>();
    mTextEditorLookAndFeel = std::make_unique<TextEditorLookAndFeel>();
    
//    codeEditor.reset(new juce::CodeEditorComponent(codeDocument, &tokeniser));
//    codeEditor->setTabSize(4, true);
//    codeEditor->setLineNumbersShown(true);
//    codeEditor->loadContent("print(\"Hello, world!\")");
    
    togglePlay.setBounds(20, 20, 50, 20);
    codeEditor.setBounds(20, 60, 400, 200);
    saveFile.setBounds(20, 220+60, 50, 20);
    timeline.setBounds(20, 320, 760, 260);
    loadFile.setBounds(70+20, 220+60, 50, 20);
    
    saveFile.addListener(this);
    codeEditor.addListener(this);
    togglePlay.addListener(this);
    loadFile.addListener(this);
    
    juce::LookAndFeel::setDefaultLookAndFeel(mGeneralLookAndFeel.get());
    togglePlay.setLookAndFeel(mButtonLookAndFeel.get());
    saveFile.setLookAndFeel(mButtonLookAndFeel.get());
    loadFile.setLookAndFeel(mButtonLookAndFeel.get());
    codeEditor.setLookAndFeel(mTextEditorLookAndFeel.get());
    
    codeEditor.setReturnKeyStartsNewLine(true);
    codeEditor.setMultiLine(true);
    codeEditor.setScrollbarsShown(true);
    codeEditor.setCaretVisible(true);
    
    const float fontSize = 20.f;
    FontOptions fontOptions {"Courier New", fontSize, juce::Font::plain};
    Font font {fontOptions};
    codeEditor.setFont(font);
    codeEditor.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    
    timeline.SetProcessor(&audioProcessor);
//    codeEditor.setPopupMenuEnabled(true);
    
    addAndMakeVisible(codeEditor);
    addAndMakeVisible(togglePlay);
    addAndMakeVisible(saveFile);
    addAndMakeVisible(timeline);
    addAndMakeVisible(loadFile);
    
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
    else if(button == &saveFile)
    {
        juce::String text = codeEditor.getText();
        std::string utf8Text = text.toRawUTF8(); // or toStdString() if you want std::string
        audioProcessor.SaveFile(utf8Text);
    }
    else if(button == &loadFile)
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
    g.fillAll (juce::Colours::white);
//    g.setColour (juce::Colours::white);
    
//    const TransportData& data = audioProcessor.GetPositionData();
    
//#if (JucePlugin_Build_Standalone == 0)
//    isPlayingLabel.setText(data.isPlaying ? "TRUE" : "FALSE", juce::dontSendNotification);
//#endif
    
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
