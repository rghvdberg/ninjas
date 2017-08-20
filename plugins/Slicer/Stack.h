/*
 * Stack.h
 *
 *  Created on: 15 aug. 2017
 *      Author: rob
 *      it's not a real stack but can't think of a better name
 */

#ifndef STACK_H_
#define STACK_H_

#include "Voice.h"
#include <vector>
#include "Slice.h"

class Stack {
public:
	Stack();
	virtual ~Stack();
	void add_Voice( Voice* voice);
	void remove_Voice(int ch, int nn);
	Voice* get_Voice(int ch, int nn);
	bool check_Voice_Playing (int ch, int nn);
	int get_Stack_Size();
	float* get_Sample(int i, std::vector<float> * samplevector);
	float get_Gain(int index);
	void inc_Position(int index, int increment);
	float runADSR(int index);
	int get_Position(int index);
	int get_Slice_Start(int index);
	int get_Slice_End(int index);

private:
	std::vector<Voice*> voice_stack;

};

#endif /* STACK_H_ */