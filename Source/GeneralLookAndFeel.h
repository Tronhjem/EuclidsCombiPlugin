#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "LookAndFeelConstants.h"

class GeneralLookAndFeel : public juce::LookAndFeel_V4
{
public:
    juce::Typeface::Ptr getTypefaceForFont (const juce::Font&) override
    {
        return juce::Typeface::createSystemTypefaceFor(MONOSPACE_FONT);
    }
};
