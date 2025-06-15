#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class TitleBar : public juce::Component
{
public:
    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::cornflowerblue); // Your custom color
        g.setColour(juce::Colours::white);
        g.setFont(juce::Font(18.0f, juce::Font::bold));
        g.drawText("My Plugin", getLocalBounds(), juce::Justification::centredLeft, true);
    }
};
