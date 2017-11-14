#ifndef VOICE_H
#define VOICE_H
#include "Slice.h"
#include "ADSR.h"

// int channel, notenumber, velocity, pitchbend, position; float gain; bool active; Slice slice; Voice voice(notenumber, velocity, channel, pitchbend, gain, active, slice, position);
// struct Voice { Voice(ChannelNo ch, NoteNumber nn, Velocity vel, ...) { ... } }; -> ChannelNo ch(5); NoteNumber nn(62); Velocity vel(100); ... Voice voice(ch, nn, vel, ...)
// struct ChannelNo { explicit ChannelNo(int chno): chno(chno) {}; int toInt() const { return chno; } private: int chno; };
class Voice
{
public:
    Voice ( double sr );
   
    int index;
    bool active;
    int channel;  //midi channel, channel is also linked to slice
    int notenumber;
    int velocity;
    float gain; // linked to adsr
    float multiplierIndex; // frame of slice (sample) playing
    float multiplier;
    int playbackIndex;
    double v_sr;
    ADSR adsr { v_sr }  ; // ADSR of voice

};

#endif // VOICE_H
