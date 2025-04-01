//
//  Track.cpp
//  EuclidsCombinator - VST3
//
//  Created by Christian Tronhjem on 29.03.25.
//

#include "Track.hpp"

constexpr uChar uCharTrue = static_cast<uChar>(1);

Track::Track(LogicSequence sequence, int output, int note) :
        mMidiOut(output),
        mInternalCount(0),
        mNote(note),
        mSequence(sequence)
{
    
}
 
void Track::Tick(MidiScheduler& midiScheduler, int nextTickTime, int globalCount)
{
    if(mSequence[globalCount] == uCharTrue)
    {
        midiScheduler.PostMidiNote(mMidiOut, mNote, 127, 11025, nextTickTime);
        mInternalCount++;
    }
}
