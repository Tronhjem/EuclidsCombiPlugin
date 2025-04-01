#pragma once

#include "LogicSequence.h"
#include "MidiScheduler.h"

class Track
{
public:
    Track(LogicSequence sequence, int output, int note);
    void Tick(MidiScheduler& midiScheduler, int nextTickTime, int globalCount);
    
private:
    int mMidiOut;
    int mInternalCount;
    int mNote;
    LogicSequence mSequence;
};
