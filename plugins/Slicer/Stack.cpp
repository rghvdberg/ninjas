/*
 * Stack.cpp
 *
 *  Created on: 15 aug. 2017
 *      Author: rob
 */

#include "Stack.h"
#include <vector>
#include <iostream>


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

float* Stack::get_Sample(int i , std::vector<float> * samplevector){
	int start = voice_stack[i]->slice->getSliceStart();
	int pos { voice_stack[i]->position };
	float* sample = &samplevector->at(start+pos);
			return sample;
}
float Stack::get_Gain(int i){
	return voice_stack[i]->gain;
}

void Stack::inc_Position(int i, int increment)
{
	int start = voice_stack[i]->position + voice_stack[i]->slice->getSliceStart();
	int end = voice_stack[i]->slice->getSliceEnd();
	//std::cout << "start : " << start << " end : " << end <<std::endl;

	if (start + increment < end)
	{
		voice_stack[i]->position += increment;
	}
	else
	{
		voice_stack[i]->position = 0;
	}

}

float Stack::runADSR(int i)
{
	bool * active = &voice_stack[i]->active;
	float adsr_gain = voice_stack[i]->adsr.ADSRrun(active);
	std::cout <<voice_stack[i]->adsr.ADSRstage << std::endl;
	return adsr_gain;
}

int Stack::get_Position(int i)
{
	return voice_stack[i]->position;
}

int Stack::get_Slice_Start(int i)
{
	return voice_stack[i]->slice->getSliceStart();
}

int Stack::get_Slice_End(int i)
{
	return voice_stack[i]->slice->getSliceEnd();
}

