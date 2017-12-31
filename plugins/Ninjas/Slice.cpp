#include "Slice.h"
#include "Sample.h"
#include <iostream>
//#include <aubio/aubio.h>

Slice::Slice()
    :
    sliceStart { 0 },
    sliceEnd { 0 },
    slicePlay(ONE_SHOT_FWD),
    sliceActive { false }

{
 
}



