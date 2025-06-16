/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//#include <juce_gui_basics/windows/juce_DocumentWindow.h>

#include "LookAndFeelConstants.h"
#include "Colours.h"
#include "TitleBarComponent.h"
#include "ErrorReporting.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr int COMPONENT_MARGIN = 15;
constexpr int OUTER_MARGIN = 20;
const int buttonWidth = 50;

constexpr int buttonHeight = 20;
//constexpr int buttonMargin = 15;

constexpr int codeEditorWidth = 400;
constexpr int codeEditorHeight = 300;


//==============================================================================
ORchestraAudioProcessorEditor::ORchestraAudioProcessorEditor (ORchestraAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (WINDOW_WIDTH, WINDOW_HEIGHT);
    
    audioProcessor.addChangeListener(this);
    
    mGeneralLookAndFeel = std::make_unique<GeneralLookAndFeel>();
    mButtonLookAndFeel = std::make_unique<ButtonLookAndFeel>();
    mTextEditorLookAndFeel = std::make_unique<TextEditorLookAndFeel>();
    
//    codeEditor.reset(new juce::CodeEditorComponent(codeDocument, &tokeniser));
//    codeEditor->setTabSize(4, true);
//    codeEditor->setLineNumbersShown(true);
//    codeEditor->loadContent("print(\"Hello, world!\")");
    
    int buttonXStart = OUTER_MARGIN;
    int nextLineY = 20;
    togglePlay.setBounds(buttonXStart, nextLineY, buttonWidth, buttonHeight);
    
    buttonXStart += buttonWidth + COMPONENT_MARGIN;
    saveFile.setBounds(buttonXStart, nextLineY, buttonWidth, buttonHeight);
    
    buttonXStart += buttonWidth + COMPONENT_MARGIN;
    loadFile.setBounds(buttonXStart, nextLineY, buttonWidth, buttonHeight);
    
    nextLineY += buttonHeight + COMPONENT_MARGIN;
    codeEditor.setBounds(OUTER_MARGIN, nextLineY, codeEditorWidth, codeEditorHeight);
    
    errorBox.setBounds(OUTER_MARGIN + codeEditorWidth + 2,
                       nextLineY, WINDOW_WIDTH - codeEditorWidth - OUTER_MARGIN - OUTER_MARGIN,
                       codeEditorHeight);
    
    nextLineY += codeEditorHeight + COMPONENT_MARGIN;
    timeline.setBounds(OUTER_MARGIN, nextLineY, 760, 260);
    
    saveFile.addListener(this);
    codeEditor.addListener(this);
    togglePlay.addListener(this);
    loadFile.addListener(this);
    
    juce::LookAndFeel::setDefaultLookAndFeel(mGeneralLookAndFeel.get());
    togglePlay.setLookAndFeel(mButtonLookAndFeel.get());
    saveFile.setLookAndFeel(mButtonLookAndFeel.get());
    loadFile.setLookAndFeel(mButtonLookAndFeel.get());
    codeEditor.setLookAndFeel(mTextEditorLookAndFeel.get());
    errorBox.setLookAndFeel(mTextEditorLookAndFeel.get());
    
    codeEditor.setReturnKeyStartsNewLine(true);
    codeEditor.setMultiLine(true);
    codeEditor.setScrollbarsShown(true);
    codeEditor.setCaretVisible(true);
    codeEditor.setFont(MONOSPACE_FONT_OPTIONS);
    codeEditor.setColour(juce::TextEditor::textColourId, ORchestraColours::TextColor);
    
    errorBox.setFont(MONOSPACE_FONT_OPTIONS);
    errorBox.setColour(juce::TextEditor::textColourId, ORchestraColours::TextColor);
    errorBox.setMultiLine(true);
    
    timeline.SetProcessor(&audioProcessor);
//    codeEditor.setPopupMenuEnabled(true);
    
    addAndMakeVisible(togglePlay);
    addAndMakeVisible(saveFile);
    addAndMakeVisible(timeline);
    addAndMakeVisible(loadFile);
    addAndMakeVisible(codeEditor);
    addAndMakeVisible(errorBox);
    
    char* data = audioProcessor.GetFileText();
    if(data != nullptr)
    {
        juce::String dataAsString {data};
        codeEditor.setText(dataAsString);
    }
}

ORchestraAudioProcessorEditor::~ORchestraAudioProcessorEditor()
{
    togglePlay.setLookAndFeel(nullptr);
    saveFile.setLookAndFeel(nullptr);
    loadFile.setLookAndFeel(nullptr);
    codeEditor.setLookAndFeel(nullptr);
    
    audioProcessor.removeChangeListener(this);
}

void ORchestraAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster* broadCaster)
{
    char* data = audioProcessor.GetFileText();
    if(data != nullptr)
    {
        juce::String dataAsString {data};
        codeEditor.setText(dataAsString);
    }
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
        std::string utf8Text = text.toRawUTF8();
        audioProcessor.SaveFile(utf8Text);
        
        std::vector<LogEntry>& errors = audioProcessor.GetErrors();
        if(errors.size() > 0)
            errorBox.setText(errors[0].mMessage);
        else
            errorBox.setText("");

    }
    else if(button == &loadFile)
    {
        chooser.launchAsync(folderChooserFlags, [this] (const juce::FileChooser& fc)
        {
            juce::File file = chooser.getResult();
            std::string filePath {file.getFullPathName().toRawUTF8()};
            char* data = audioProcessor.LoadFile(filePath);
            if(data != nullptr)
            {
                juce::String dataAsString {data};
                codeEditor.setText(dataAsString);
            }
            
            std::vector<LogEntry>& errors = audioProcessor.GetErrors();
            if(errors.size() > 0)
                errorBox.setText(errors[0].mMessage);
            else
                errorBox.setText("");
        });
    }
}

//==============================================================================
void ORchestraAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (ORchestraColours::Background);
}

void ORchestraAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
