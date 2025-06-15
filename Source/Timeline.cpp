#include "Timeline.h"
#include "Utility.h"

void Timeline::timerCallback()
{
#if _DEBUG
    assert(mAudioProcessor != nullptr);
#endif
    
    const TransportData& transportData = mAudioProcessor->GetTransportData();
    const int64_t timeInSamples = transportData.timeInSamples;
    
    const double samplesPerStep = static_cast<double>(transportData.sampleRate) * (60.0 / (transportData.bpm * 2.0));
    const int currentStep = static_cast<int>(ceil(static_cast<double>(timeInSamples) / samplesPerStep));
    
//    const double pixelPerSample = dotSize / samplesPerStep;
//    const int64_t samplesSinceLast = timeInSamples - mLastTimeInSamples;
//    const double deltaPixels = samplesSinceLast * pixelPerSample;
    
    if(currentStep == mLastGlobalStep)
    {
        return;
    }
    
    mLastGlobalStep = currentStep;
    mLastTimeInSamples = timeInSamples;
   
    repaint();
}

void Timeline::paint(juce::Graphics& g)
{
    
    const int globalStepOffset = mLastGlobalStep - 1 + STEP_BUFFER_SIZE;
    
    for (int step = 0; step < numberOfDrawnSteps; ++step)
    {
        // We start behind the global step, as it's always one ahead and we
        // want to paint the current step being triggered.
        const int stepWrapped = (globalStepOffset + step) % STEP_BUFFER_SIZE;
        std::vector<StepData>& stepDatas = mAudioProcessor->GetStepData()[stepWrapped];
        
        float alpha = 1.f;
        if(step >= numberOfDrawnSteps - indexStartFade)
        {
            float alphaValue = 1.f - (static_cast<float>(step - numberOfDrawnSteps + indexStartFade) / static_cast<float>(indexStartFade));
            alpha = alphaValue * alphaValue;
        }

        const int size = static_cast<int>(stepDatas.size());
        for (int i = 0; i < size; ++i)
        {
            const StepData& stepData = stepDatas[i];

            juce::Colour colorToSet;
            if (stepData.mShouldTrigger > 0)
            {
                float t = static_cast<float>(stepData.mSecondData) / 127.f;
                colorToSet = lerpColour(minVelocity, maxVelocity, t);
            }
            else
                colorToSet = inactiveColor;

            g.setColour(colorToSet.withAlpha(alpha));
            
            const float y = i * stepY;
            const float x = step * stepX;
            
//            mStepXPositions[step][i] = (mStepXPositions[step][i] - x) + deltaPixels;
            g.fillRect(x, y, drawnStepWidth, drawnStepHeight);
            if (step == 0 && stepData.mShouldTrigger)
            {
                g.setColour(juce::Colours::black);
                g.drawRect(x, y, drawnStepWidth, drawnStepHeight, 2.f);
            }
            
            g.setColour(juce::Colours::black);
            std::string first {std::to_string((int)stepData.mFirstData)};
            g.drawText(first, x, y + quaterStepHeight, stepWidth, 15.f, juce::Justification::centred);
        }
    }
}
