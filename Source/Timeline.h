#pragma once

#include <JuceHeader.h>
#include "StepData.h"
#include "ORchestraEngine.h"
#include "PluginProcessor.h"

constexpr float trackHeight = 50.f;
constexpr float dotSize = trackHeight - 10.f;
constexpr float stepWidth = dotSize + 3.f;
constexpr float stepY = trackHeight + (trackHeight - dotSize) / 2.0f;
constexpr float stepX = stepWidth + stepWidth / 2.0f - dotSize / 2.0f;
constexpr int numberOfDrawnSteps = 17;
constexpr int numberOfDrawnStepRows = 5;

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
