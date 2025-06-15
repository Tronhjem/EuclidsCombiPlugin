#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class GeneralLookAndFeel : public juce::LookAndFeel_V4
{
public:
    juce::Typeface::Ptr getTypefaceForFont (const juce::Font&) override
    {
        return juce::Typeface::createSystemTypefaceFor(mFont);
    }
    
private:
    const float mFontSize = 25.f;
    FontOptions mFontOptions {"Courier New", mFontSize, juce::Font::plain};
    Font mFont {mFontOptions};
};
