#include "Timeline.h"

void Timeline::timerCallback()
{
#if _DEBUG
    assert(mAudioProcessor != nullptr);
#endif
    
//    const float animationTimeInSamples = 500.f/**ms**/ * transportData.sampleRate * 0.001f;
//    const int currentStep = static_cast<int>(ceil(static_cast<double>(transportData.timeInSamples) / samplesPerStep));
//    const int nextStepInSamples = static_cast<int>(samplesPerStep * currentStep);
    
//    const int globalStep = mAudioProcessor->GetGlobalStepCount();
//        if(globalStep == mLastGlobalStep)
//            return;
//    mLastGlobalStep = globalStep;
   
    repaint();
}

void Timeline::paint(juce::Graphics& g)
{
#if _DEBUG
    assert(mAudioProcessor != nullptr);
#endif
    
    const TransportData& transportData = mAudioProcessor->GetTransportData();
    const int64_t timeInSamples = transportData.timeInSamples;
    
    const double samplesPerStep = static_cast<double>(transportData.sampleRate) * (60.0 / (transportData.bpm * 2.0));
    const int currentStep = static_cast<int>(ceil(static_cast<double>(timeInSamples) / samplesPerStep));
    if(currentStep != mLastGlobalStep)
    {
        mLastGlobalStep = currentStep;
        mLastTimeInSamples = timeInSamples;
    }
    const double pixelPerSample = dotSize / samplesPerStep;
    const int64_t samplesSinceLast = timeInSamples - mLastTimeInSamples;
//    if (samplesSinceLast >= samplesPerStep + 512)
//    {
//        DBG(samplesSinceLast);
//    }V
    const double deltaPixels = samplesSinceLast * pixelPerSample;
    
//    if(samplesSinceLast >= samplesPerStep)
    
    const int globalStepOffset = mLastGlobalStep - 1 + STEP_BUFFER_SIZE;
    
    for (int step = 0; step < numberOfDrawnSteps; ++step)
    {
        // We start behind the global step, as it's always one ahead and we
        // want to paint the current step being triggered.
        const int stepWrapped = (globalStepOffset + step) % STEP_BUFFER_SIZE;

        std::vector<StepData>& stepDatas = mAudioProcessor->GetStepData()[stepWrapped];

        const int size = static_cast<int>(stepDatas.size());
        for (int i = 0; i < size; ++i)
        {
            const StepData& stepData = stepDatas[i];

            if (stepData.mShouldTrigger > 0)
            {
                if (step == 0)
                    g.setColour(juce::Colours::white);
                else
                    g.setColour(juce::Colours::orange);
            }
            else
                g.setColour(juce::Colours::grey);

            const float y = i * stepY;
            const float x = step * stepX - deltaPixels + dotSize;
            
//            mStepXPositions[step][i] = (mStepXPositions[step][i] - x) + deltaPixels;
            g.fillEllipse(x, y, dotSize, dotSize);
            
            g.setColour(juce::Colours::black);
            std::string first {std::to_string((int)stepData.mFirstData)};
            std::string second {std::to_string((int)stepData.mSecondData)};
            g.drawText(first, x, y + 4.f, dotSize, 15.f, juce::Justification::centred);
            g.drawText(second, x, y + 19.f, dotSize, 15.f, juce::Justification::centred);
        }
    }
}
