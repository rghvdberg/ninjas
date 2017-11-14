#include "Voice.h"
#include "Slice.h"
#include "ADSR.h"
#include <iostream>

Voice::Voice( double sr)
{
    channel = 0;
    active = false;
    notenumber = 0;
    velocity =127;
    gain = 1.0;
    multiplierIndex = 0.0;
    multiplier=1.0;
    playbackIndex=0;
    v_sr = sr;
    
    ADSR adsr { v_sr };
}
