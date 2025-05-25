#pragma once

#include <JuceHeader.h>
#include "StepData.h"
#include "ORchestraEngine.h"

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
//       g.fillAll(juce::Colours::black);
       const int globalStep = mAudioProcessor->GetGlobalStepCount();

       const float trackHeight = 40.f;
       const float stepWidth = 23.f;
       const float dotSize = trackHeight * 0.5f;

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
       }
       
       mTriggerSteps.clear();
       
       for (int step = 0; step < STEP_BUFFER_SIZE; ++step)
       {
           int stepWrapped = (globalStep + step) % STEP_BUFFER_SIZE;
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
           }
       }
   }
    
private:
    std::vector<StepData> mTriggerSteps;
    ORchestraAudioProcessor* mAudioProcessor;
    int mLastGlobalStep = -1;
};
