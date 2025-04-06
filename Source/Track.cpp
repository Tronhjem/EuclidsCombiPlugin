#include "Track.hpp"

constexpr uChar uCharTrue = static_cast<uChar>(1);

Track::Track(LogicSequence sequence, int output, int note) :
        mMidiOut(output),
        mInternalCount(0),
        mNote(note),
        mCombinedSequences(sequence)
{
}
 
void Track::Tick(MidiScheduler& midiScheduler, int nextTickTime, int globalCount)
{
    if(mCombinedSequences[globalCount] == uCharTrue)
    {
        midiScheduler.PostMidiNote(mMidiOut, mNote, 127, 11025, nextTickTime);
        mInternalCount++;
    }
}
