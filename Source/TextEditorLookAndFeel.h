#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class TextEditorLookAndFeel : public juce::LookAndFeel_V4
{
public:
    TextEditorLookAndFeel(juce::Colour bgColour = juce::Colours::white)
        : background(bgColour) {}

    void fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& editor) override
    {
        g.fillAll (background);
    }

private:
    juce::Colour background;
};
