//
//  Track.cpp
//  EuclidsCombinator - VST3
//
//  Created by Christian Tronhjem on 29.03.25.
//

#include "Track.hpp"

Track::Track(LogicSequence sequence, int output, int note) :
        mMidiOut(output),
        mInternalCount(0),
        mNote(note),
        mSequence(sequence)
{
    
}
 
void Track::Tick(MidiScheduler& midiScheduler, int nextTickTime, int globalCount)
{
    if(mSequence[globalCount] == (uint_8)1)
    {
        midiScheduler.PostMidiNote(mMidiOut, mNote, 127, 11025, nextTickTime);
        mInternalCount++;
    }
}
