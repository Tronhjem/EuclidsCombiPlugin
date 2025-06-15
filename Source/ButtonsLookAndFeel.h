#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class ButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ButtonLookAndFeel(juce::Colour buttonColour = juce::Colours::black)
        : customButtonColour(buttonColour) {}

    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                             const juce::Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override
    {
        auto bounds = button.getLocalBounds().toFloat();

        juce::Colour fillColour = customButtonColour;

        if (isButtonDown)
            fillColour = fillColour.darker(0.15f);
        else if (isMouseOverButton)
            fillColour = fillColour.brighter(0.15f);

        // No rounded corners: cornerSize = 0
        g.setColour(fillColour);
        g.fillRect(bounds);

        // Optional: draw border
        g.setColour(fillColour.darker(0.3f));
        g.drawRect(bounds, 1.0f);
    }
    
    juce::Font getTextButtonFont (juce::TextButton&, int buttonHeight) override
    {
        return mFont;
    }

private:
    juce::Colour customButtonColour;
    const float mFontSize = 15.f;
    FontOptions mFontOptions {"Courier New", mFontSize, juce::Font::plain};
    Font mFont {mFontOptions};
};
