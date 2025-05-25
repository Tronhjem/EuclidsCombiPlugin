#pragma once

#include <JuceHeader.h>
#include "StepData.h"
#include "ORchestraEngine.h"

constexpr float trackHeight = 50.f;
constexpr float dotSize = trackHeight - 10.f;
constexpr float stepWidth = dotSize + 3.f;

class Timeline : public juce::Component, public juce::Timer
{
public:
    Timeline() : mAudioProcessor(nullptr)
    {
        startTimerHz(40);
    }
    
    ~Timeline() override
    {
        stopTimer();
    }
    
    void SetProcessor(ORchestraAudioProcessor* audioProcessor)
    {
        mAudioProcessor = audioProcessor;
    }
    
    void timerCallback() override
    {
       if(mAudioProcessor == nullptr)
           return;
        
       const int globalStep = mAudioProcessor->GetGlobalStepCount();
       if(globalStep == mLastGlobalStep)
           return;
       
       mLastGlobalStep = globalStep;
       repaint();
    }

   void paint(juce::Graphics& g) override
   {
       if(mAudioProcessor == nullptr)
           return;
       
       // Paint the Trigger Step
       int triggerStepSize = static_cast<int>(mTriggerSteps.size());
       
       g.setColour(juce::Colours::black);
       g.fillRect(0.f, 0.f, stepWidth, triggerStepSize * trackHeight);
       
       for (int i = 0; i < triggerStepSize; ++i)
       {
           const StepData& stepData = mTriggerSteps[i];
           float y = i * trackHeight + (trackHeight - dotSize) / 2.0f;
           
           if (stepData.mShouldTrigger > 0)
           {
               g.setColour(juce::Colours::white);
           }
           else
               g.setColour(juce::Colours::grey);
               
           float x = stepWidth / 2.0f - dotSize / 2.0f;

           g.fillEllipse(x, y, dotSize, dotSize);
           std::string first {std::to_string((int)stepData.mFirstData)};
           std::string second {std::to_string((int)stepData.mSecondData)};
           g.setColour(juce::Colours::black);
           g.drawText(first, x, y + 4.f, dotSize, 15.f, juce::Justification::centred);
           g.drawText(second, x, y + 19.f, dotSize, 15.f, juce::Justification::centred);
       }
       
       mTriggerSteps.clear();
       
       const int numberOfDrawnSteps = 16;
       for (int step = 0; step < numberOfDrawnSteps; ++step)
       {
           int stepWrapped = (mLastGlobalStep + step) % STEP_BUFFER_SIZE;
           if(stepWrapped < 0)
               stepWrapped = 0;
           
           std::vector<StepData>& stepDatas = mAudioProcessor->GetStepData()[stepWrapped];

           const int size = static_cast<int>(stepDatas.size());
           for (int i = 0; i < size; ++i)
           {
               const StepData& stepData = stepDatas[i];
               float y = i * trackHeight + (trackHeight - dotSize) / 2.0f;
               
               if(step == 0)
                   mTriggerSteps.emplace_back(stepData);
               
               if (stepData.mShouldTrigger > 0)
                   g.setColour(juce::Colours::orange);
               else
                   g.setColour(juce::Colours::grey);
                   
               float x = (step + 1) * stepWidth + stepWidth / 2.0f - dotSize / 2.0f;

               g.fillEllipse(x, y, dotSize, dotSize);
               std::string first {std::to_string((int)stepData.mFirstData)};
               std::string second {std::to_string((int)stepData.mSecondData)};
               g.setColour(juce::Colours::black);
               g.drawText(first, x, y + 4.f, dotSize, 15.f, juce::Justification::centred);
               g.drawText(second, x, y + 19.f, dotSize, 15.f, juce::Justification::centred);
           }
       }
   }
    
private:
    std::vector<StepData> mTriggerSteps;
    ORchestraAudioProcessor* mAudioProcessor;
    int mLastGlobalStep = -1;
};
