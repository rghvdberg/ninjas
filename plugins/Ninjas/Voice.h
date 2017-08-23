#ifndef VOICE_H
#define VOICE_H
#include "Slice.h"
#include <tuple>
#include "ADSR.h"

// int channel, notenumber, velocity, pitchbend, position; float gain; bool active; Slice slice; Voice voice(notenumber, velocity, channel, pitchbend, gain, active, slice, position);
// struct Voice { Voice(ChannelNo ch, NoteNumber nn, Velocity vel, ...) { ... } }; -> ChannelNo ch(5); NoteNumber nn(62); Velocity vel(100); ... Voice voice(ch, nn, vel, ...)
// struct ChannelNo { explicit ChannelNo(int chno): chno(chno) {}; int toInt() const { return chno; } private: int chno; };
class Voice
{
public:
    Voice ();
    //Voice(int id, bool active, int ch,int nn, int vel, int pb, float gn, Slice* slc, int pos);

    /*getter function
     * will fix when really need this :-)
        std::tuple<int, int, int, int, int, float, Slice*, int , ADSR> getVoice()
        {
            return std::make_tuple(index, channel, notenumber, velocity, pitchbend, gain, slice, position, adsr);
        }*/

    int index;
    bool active;
    int channel;  //midi channel, channel is also linked to slice
    int notenumber;
    int velocity;
    int pitchbend;
    float gain; // linked to adsr
    Slice* slice; // the part of the sample this voice plays
    float multiplierIndex; // frame of slice (sample) playing
    float multiplier;
    int playbackIndex;
    ADSR adsr; // ADSR of voice

};

#endif // VOICE_H
