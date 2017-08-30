/*
 * Stack.cpp
 *
 *  Created on: 15 aug. 2017
 *      Author: rob
 */

#include "Stack.h"
#include <vector>
#include <iostream>
#include <algorithm>

Stack::Stack() {

}

Stack::~Stack() {
	// TODO Auto-generated destructor stub
}
void Stack::add_Voice(Voice* voice)
{

	voice_stack.push_back(voice);

}

void Stack::remove_Voice(int ch, int nn)
{
	for(int i = 0; i <= (int) voice_stack.size(); i++)
	{
			if ( (voice_stack[i]->channel == ch) && (voice_stack[i]->notenumber == nn))
				voice_stack.erase(voice_stack.begin()+i);
	}
}

void Stack::remove_Voice(int i)
{
 voice_stack.erase(voice_stack.begin()+i);
}

void Stack::remove_inactive_voices()
{
    voice_stack.erase(std::remove_if(voice_stack.begin(), voice_stack.end(), []( Voice*& v) { return !v->active; }), voice_stack.end());
}

Voice* Stack::get_Voice(int ch, int nn)
{
	for(int i = 0; i <= (int) voice_stack.size(); i++)
	{
		if ( (voice_stack[i]->channel == ch) && (voice_stack[i]->notenumber == nn))
			return voice_stack[i];
}
	// error 404 voice not found
	return nullptr;
}

bool Stack::check_Voice_Playing(int ch, int nn)
{
	for(int i = 0; i <= (int) voice_stack.size(); i++){
		if ( (voice_stack[i]->channel == ch) && (voice_stack[i]->notenumber == nn))
			return true;
	}
		return false;
}

int Stack::get_Stack_Size(){
	return voice_stack.size();
}

float* Stack::get_Sample(int i , std::vector<float> * samplevector, Slice* slices){
	int midichannel = get_midiChannel(i);
	int start = slices[midichannel].getSliceStart();
	int pos { voice_stack[i]->playbackIndex };
	float* sample = &samplevector->at(start+pos);
			return sample;
}
float Stack::get_Gain(int i){
	return voice_stack[i]->gain;
}

void Stack::inc_Position(int i, int channels, Slice* slices)
{
    int midichannel = get_midiChannel(i);
    int sliceStart    = slices[midichannel].getSliceStart();
    int sliceEnd      = slices[midichannel].getSliceEnd();
    float multiplier = voice_stack[i]->multiplier;
    //float multiplierIndex = voice_stack[i]->multiplierIndex;
    Slice::slicePlayMode playmode = slices[midichannel].getSlicePlayMode();

    //    std::cout << "function Stack::inc_Position(int i, int channels)" << std::endl;
    //   std::cout << "slicePlayMode = " << playmode << std::endl;
    //   std::cout << "sliceStart    = " << sliceStart << std::endl;
    //    std::cout << "sliceEnd      = " << sliceEnd << std::endl;
    //    std::cout << "playbackIndex = " << playbackIndex << std::endl;
    //    std::cout << "multiplier    = " << multiplier << std::endl;



    // set multiplier to negative if direction is reverse
    if (playmode == Slice::LOOP_REV || playmode == Slice::ONE_SHOT_REV)
        multiplier=-multiplier;

    // add the multiplier, when it's negative this should substract
    voice_stack[i]->multiplierIndex += multiplier;
    int tmp = (int) voice_stack[i]->multiplierIndex;
    tmp=tmp * channels;
    // std::cout << "tmp           = " << tmp << std::endl;
    //std::cout << "start + tmp   = " << start+tmp << std::endl;
    // check bounderies according to playmode: loop or oneshot.
    switch (playmode)
    {
    case Slice::LOOP_FWD:
    {
        if (tmp >= (sliceEnd-channels))
        {
            voice_stack[i]->playbackIndex = 0;
            voice_stack[i]->multiplierIndex = 0;
        }
        else
        {
            voice_stack[i]->playbackIndex = tmp;
        }
        break;
    }

    case Slice::LOOP_REV:
    {
        if (sliceStart + tmp <= sliceStart)
        {
            voice_stack[i]->playbackIndex = sliceEnd ;
            voice_stack[i]->multiplierIndex = (sliceEnd -sliceStart)/channels;
        }
        else
            voice_stack[i]->playbackIndex = tmp;
        break;
    }
    case Slice::ONE_SHOT_FWD:
    {
        if (sliceStart + tmp >= (sliceEnd-channels))
        {
         //   std::cout << "tmp = " << tmp << std::endl;

            remove_Voice(i);
        }
        else voice_stack[i]->playbackIndex = tmp;
      //  std::cout << voice_stack[i]->playbackIndex << std::endl;
        break;
    }
    case Slice::ONE_SHOT_REV:
    {
        //std::cout << sliceStart << " , " << tmp << " , " << sliceEnd << std::endl;
        if (sliceStart + tmp <= sliceStart)
        {
            remove_Voice(i);
        }
        else
            voice_stack[i]->playbackIndex = tmp;
        break;
    }

    }

}

float Stack::runADSR(int i)
{
	bool * active = &voice_stack[i]->active;
	float adsr_gain = voice_stack[i]->adsr.ADSRrun(active);
    //std::cout << "ADSRstage = "<< voice_stack[i]->adsr.ADSRstage << std::endl;
	return adsr_gain;
}

int Stack::get_Position(int i)
{
    return voice_stack[i]->playbackIndex;
}

// int Stack::get_Slice_Start(int i)
// {
// 	return voice_stack[i]->slice->getSliceStart();
// }

// int Stack::get_Slice_End(int i)
// {
// 	return voice_stack[i]->slice->getSliceEnd();
// }

bool Stack::get_Voice_Active(int i)
{
    return voice_stack[i]->active;
}

int Stack::get_midiChannel(int i)
{
    return voice_stack[i]->channel;
}