/*
 * Mixer.cpp
 *
 *  Created on: 13 aug. 2017
 *      Author: rob
 */

#include "Mixer.h"
#include <iostream>



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
//    std::cout << "add_Sample - channels : " << channels << std::endl;
  
}

float Mixer::get_Mix()
{
    //std::cout << "get_Mix() channels" << channels << std::endl;
    average = mix/channels;
    mix = 0;
    channels = 0;
    return average;
}
