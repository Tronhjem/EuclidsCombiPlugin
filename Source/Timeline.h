#pragma once

#include <JuceHeader.h>
using namespace ::juce::gl;

class Timeline : public juce::Component
{
public:
    Timeline()
    {
    }
    
    ~Timeline() override
    {
    }
    
    void setTriggers(const std::vector<std::vector<bool>>& newTriggers)
    {
        triggers = newTriggers;
        repaint();
    }

   void paint(juce::Graphics& g) override
   {
       g.fillAll(juce::Colours::black);

       const int trackCount = (int) triggers.size();
       if (trackCount == 0)
           return;

       const int stepCount = (int) triggers[0].size();
       const float trackHeight = 40.f;
       const float stepWidth = 20.f;
       const float dotSize = trackHeight * 0.5f;

       for (int track = 0; track < trackCount; ++track)
       {
           float y = track * trackHeight + (trackHeight - dotSize) / 2.0f;

           for (int step = 0; step < stepCount; ++step)
           {
               if (triggers[track][step])
                   g.setColour(juce::Colours::orange);
               else
                   g.setColour(juce::Colours::grey);
                   

               float x = step * stepWidth + stepWidth / 2.0f - dotSize / 2.0f;

               g.fillEllipse(x, y, dotSize, dotSize);
           }
       }

//       // Optionally: draw grid
//       g.setColour(juce::Colours::darkgrey);
//       for (int step = 0; step < stepCount; ++step)
//       {
//           float x = step * stepWidth + stepWidth / 2.0f;
//           g.drawLine(x, 0.0f, x, (float)getHeight(), 0.5f);
//       }
   }

    
private:
    std::vector<std::vector<bool>> triggers;
    
};
