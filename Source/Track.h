#pragma once

#include "MidiScheduler.h"
#include "DataSequence.h"

class Track
{
public:
//    Track(LogicSequence sequence, int output, int note);
    void Tick(MidiScheduler& midiScheduler, int nextTickTime, int globalCount);
    
private:
    Track& operator=(const Track&)= delete;
    
    int mMidiOut;
    int mInternalCount;
    int mNote;
    std::vector<DataSequence*> mDataSequences;
//
//    NAME, GET_CONTANT_AT_INDEX - stack: 'a',
//    NAME, GET_CONTANT_AT_INDEX
//    OPERATION
    
};
