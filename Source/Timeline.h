#pragma once

#include <JuceHeader.h>
#include "StepData.h"
#include "ORchestraEngine.h"
#include "PluginProcessor.h"

constexpr float stepMargin = 2.5f;
constexpr float trackHeight = 32.f;

constexpr float stepHeight = trackHeight;
constexpr float quaterStepHeight = trackHeight * 0.25f;
constexpr float stepWidth = (trackHeight * 1.5f);
constexpr float drawnStepHeight = stepHeight - stepMargin;
constexpr float drawnStepWidth = stepWidth - stepMargin;

constexpr float stepY = trackHeight + (trackHeight - stepHeight) / 2.0f;
constexpr float stepX = stepWidth + stepWidth / 2.0f - stepWidth / 2.0f;
constexpr int numberOfDrawnSteps = 17;
constexpr int numberOfDrawnStepRows = 5;
constexpr float indexStartFade = 6.f;

const juce::Colour minVelocity = Colour::fromFloatRGBA(0.f, 0.f, 1.f, 1.f);
const juce::Colour maxVelocity = Colour::fromFloatRGBA(1.f, 0.f, 0.f, 1.f);
const juce::Colour inactiveColor = Colour::fromFloatRGBA(0.3f, 0.3f, 0.3f, 1.f);

class Timeline : public juce::Component, public juce::Timer
{
public:
    Timeline() :
        mAudioProcessor(nullptr),
        mLastGlobalStep(-1),
        mPixelsPerBeat(0.f),
        mLastTimeInSamples(0)
    {
        startTimerHz(25);
    }
    
    ~Timeline() override
    {
        stopTimer();
    }
    
    void SetProcessor(ORchestraAudioProcessor* audioProcessor) { mAudioProcessor = audioProcessor; }
    void timerCallback() override;
    void paint(juce::Graphics& g) override;
    
private:
    ORchestraAudioProcessor* mAudioProcessor;
    int mLastGlobalStep;
    float mPixelsPerBeat;
    int64_t mLastTimeInSamples;
    std::array<std::array<int, numberOfDrawnStepRows>, numberOfDrawnSteps> mStepXPositions;
};
