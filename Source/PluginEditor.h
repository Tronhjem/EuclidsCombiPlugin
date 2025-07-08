/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "Timeline.h"

#include "GeneralLookAndFeel.h"
#include "ButtonsLookAndFeel.h"
#include "TextEditorLookAndFeel.h"

//==============================================================================
/**
*/
class ORchestraAudioProcessorEditor  :  public juce::AudioProcessorEditor,
                                        public juce::TextEditor::Listener,
                                        public juce::Button::Listener,
                                        public juce::ComboBox::Listener,
                                        public juce::ChangeListener,
										public juce::Slider::Listener
{
public:
    ORchestraAudioProcessorEditor (ORchestraAudioProcessor&);
    ~ORchestraAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void textEditorTextChanged(juce::TextEditor& editor) override;
    void extracted();
    void buttonClicked(juce::Button* button) override;
	void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged(juce::Slider* slider) override;
    
private:
    ORchestraAudioProcessor& audioProcessor;
    void changeListenerCallback(juce::ChangeBroadcaster* broadCaster) override;
    
    std::unique_ptr<GeneralLookAndFeel> mGeneralLookAndFeel;
    std::unique_ptr<ButtonLookAndFeel> mButtonLookAndFeel;
    std::unique_ptr<TextEditorLookAndFeel> mTextEditorLookAndFeel;
    
    //juce::Label isPlayingLabel;
    //juce::Label barCountLabel;
    //juce::Label beatCountLabel;
    //juce::Label ppqLabel;
    juce::TextButton togglePlay {"Play"};
    
    juce::FileChooser chooser {"Select a file to load...", juce::File{}, "*.txt"};
    int folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
    juce::TextButton loadFile {"Load"};
    juce::TextButton saveFile {"Save"};
    juce::ComboBox mNoteDivisonBox;
    juce::StringArray mNoteDivisonStrings{ "1n", "2n","4n", "8n", "16n", "32n", "64n"};
    juce::Slider mBpmBox;
    
    Timeline timeline;
    juce::TextEditor codeEditor;
    juce::TextEditor errorBox;

//    juce::CodeDocument codeDocument;
//       juce::CodeTokeniser tokeniser; // You can subclass this or use CppTokeniser, LuaTokeniser, etc.
//       std::unique_ptr<juce::CodeEditorComponent> codeEditor;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ORchestraAudioProcessorEditor)
};
