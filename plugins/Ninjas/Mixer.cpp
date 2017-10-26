/*
 * Mixer.cpp
 *
 *  Created on: 13 aug. 2017
 *      Author: rob
 */

#include "Mixer.h"



Mixer::Mixer()
    : mix {0}, channels {0}, average {0}

{

}

Mixer::~Mixer() {
    // TODO Auto-generated destructor stub
}

void Mixer::add_Sample(float s)
{
    mix +=s;
    ++channels;
}

float Mixer::get_Mix()
{
    average = mix/channels;
    mix = 0;
    channels = 0;
    return average;
}
