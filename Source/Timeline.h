#pragma once

#include <JuceHeader.h>
#include "StepData.h"
#include "ORchestraEngine.h"
#include <chrono>

constexpr float trackHeight = 50.f;
constexpr float dotSize = trackHeight - 10.f;
constexpr float stepWidth = dotSize + 3.f;

class Timeline : public juce::Component, public juce::Timer
{
public:
    float dotXpos = 0;
    std::chrono::high_resolution_clock::time_point start;
    
    Timeline() : mAudioProcessor(nullptr)
    {
        startTimerHz(60);
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
       
//       dotXpos += 0.5;
//       if(dotXpos > 400)
//           dotXpos = 0;
//       
//       g.setColour(juce::Colours::white);
//       for(int i = 0; i < 2; ++i)
//       {
//           for(int j = 0; j < 1; ++j)
//               g.fillEllipse(dotXpos + j * dotSize, 20 + (i * dotSize), dotSize, dotSize);
//       }
       
       if(mAudioProcessor == nullptr)
           return;

       // Paint the Trigger Step
//       int triggerStepSize = static_cast<int>(mTriggerSteps.size());
//
//       g.setColour(juce::Colours::black);
//       g.fillRect(0.f, 0.f, stepWidth, triggerStepSize * trackHeight);
//
//       for (int i = 0; i < triggerStepSize; ++i)
//       {
//           const StepData& stepData = mTriggerSteps[i];
//           float y = i * trackHeight + (trackHeight - dotSize) / 2.0f;
//
//           if (stepData.mShouldTrigger > 0)
//           {
//               g.setColour(juce::Colours::white);
//           }
//           else
//               g.setColour(juce::Colours::grey);
//
//           float x = stepWidth / 2.0f - dotSize / 2.0f;
//           g.fillEllipse(x, y, dotSize, dotSize);
//           std::string first {std::to_string((int)stepData.mFirstData)};
//           std::string second {std::to_string((int)stepData.mSecondData)};
//           g.setColour(juce::Colours::black);
//           g.drawText(first, x, y + 4.f, dotSize, 15.f, juce::Justification::centred);
//           g.drawText(second, x, y + 19.f, dotSize, 15.f, juce::Justification::centred);
//       }
//
//       mTriggerSteps.clear();

       const int numberOfDrawnSteps = 17;
       for (int step = 0; step < numberOfDrawnSteps; ++step)
       {
           // We start behind the global step, as it's always one ahead and we
           // want to paint the current step being triggered.
           int stepWrapped = (mLastGlobalStep - 1 + step) % STEP_BUFFER_SIZE;
           if(stepWrapped < 0)
               stepWrapped = 0;

           std::vector<StepData>& stepDatas = mAudioProcessor->GetStepData()[stepWrapped];

           const int size = static_cast<int>(stepDatas.size());
           for (int i = 0; i < size; ++i)
           {
               const StepData& stepData = stepDatas[i];
               const float y = i * trackHeight + (trackHeight - dotSize) / 2.0f;

               if (stepData.mShouldTrigger > 0)
               {
                   if (step == 0)
                       g.setColour(juce::Colours::white);
                   else
                       g.setColour(juce::Colours::orange);
               }
               else
                   g.setColour(juce::Colours::grey);

               const float x = step * stepWidth + stepWidth / 2.0f - dotSize / 2.0f;

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
