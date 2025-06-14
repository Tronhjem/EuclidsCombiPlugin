#include "Timeline.h"

inline juce::Colour lerpColour(const juce::Colour& c1, const juce::Colour& c2, float t)
{
//    t = juce::jlimit(0.0f, 1.0f, t); // clamp t to [0, 1]
    auto lerp = [t](uint8 a, uint8 b) -> uint8
    {
        return static_cast<uint8>(a + (b - a) * t);
    };
    return juce::Colour(
        lerp(c1.getRed(),   c2.getRed()),
        lerp(c1.getGreen(), c2.getGreen()),
        lerp(c1.getBlue(),  c2.getBlue()),
        lerp(c1.getAlpha(), c2.getAlpha())
    );
}

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

        const int size = static_cast<int>(stepDatas.size());
        for (int i = 0; i < size; ++i)
        {
            const StepData& stepData = stepDatas[i];

            if (stepData.mShouldTrigger > 0)
            {
                if (step == 0)
                    g.setColour(juce::Colours::white);
                else
                {
                    float t = static_cast<float>(stepData.mSecondData) / 127.f;
                    juce::Colour c = lerpColour(minVelocity, maxVelocity, t);
                    g.setColour(c);
                }
            }
            else
                g.setColour(juce::Colours::grey);

            const float y = i * stepY;
            const float x = step * stepX;
            
//            mStepXPositions[step][i] = (mStepXPositions[step][i] - x) + deltaPixels;
            g.fillRect(x, y, drawnStepWidth, drawnStepHeight);
            
            g.setColour(juce::Colours::black);
            std::string first {std::to_string((int)stepData.mFirstData)};
            g.drawText(first, x, y + quaterStepHeight, stepWidth, 15.f, juce::Justification::centred);
        }
    }
}
