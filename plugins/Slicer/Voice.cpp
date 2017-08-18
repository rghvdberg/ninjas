#include "Voice.h"
#include "Slice.h"
#include "ADSR.h"

Voice::Voice()
{
	channel = 0;
	active = false;
	notenumber = 0;
	velocity =127;
	pitchbend = 0x2000;
	gain = 1.0;
	slice = nullptr ;
	position = 0;
	ADSR adsr { };
}
Voice::Voice(int id, bool act, int ch ,int nn , int vel , int pb, float gn, Slice* slc, int pos )
{
	active = act;
	channel = ch;
	notenumber = nn;
	velocity =vel;
	pitchbend = pb;
	gain = gn;
	slice = slc;
	position = pos;
	ADSR adsr { };
}
