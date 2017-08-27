#include "Voice.h"
#include "Slice.h"
#include "ADSR.h"

Voice::Voice()
{
    channel = 0;
    active = false;
    notenumber = 0;
    velocity =127;
    gain = 1.0;
    slice = nullptr ;
    multiplierIndex = 0.0;
    multiplier=1.0;
    playbackIndex=0;
}
//Voice::Voice(int id, bool act, int ch ,int nn , int vel , int pb, float gn, Slice* slc, float pos )
//{
//	active = act;
//	channel = ch;
//	notenumber = nn;
//	velocity =vel;
//	pitchbend = pb;
//	gain = gn;
//	slice = slc;
//    multiplierIndex = pos;
//    ADSR adsr { };
//}
