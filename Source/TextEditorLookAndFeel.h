#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "Colours.h"

class TextEditorLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& editor) override
    {
        g.fillAll (ORchestraColours::Background);
    }
};
