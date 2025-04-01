//
//  Track.hpp
//
//  Created by Christian Tronhjem on 29.03.25.
//

#pragma once

#include "Sequence.h"
#include "MidiScheduler.h"

class Track
{
public:
    Track(Sequence sequence, int output, int note) :
        mMidiOut(output),
        mInternalCount(0),
        mNote(note),
        mSequence(sequence)
    {
    }
    
    void Tick(MidiScheduler& midiScheduler, int nextTickTime, int globalCount)
    {
        if(mSequence[globalCount] == (uint_8)1)
        {
            midiScheduler.PostMidiNote(mMidiOut, mNote, 127, 11025, nextTickTime);
            mInternalCount++;
        }
    }
    
private:
    int mMidiOut;
    int mInternalCount;
    int mNote;
    Sequence mSequence;
};
