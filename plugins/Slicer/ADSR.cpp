/*
 * ADSR.cpp
 *
 *  Created on: 11 aug. 2017
 *      Author: rob
 */

#include "ADSR.h"
#include "globals.h"


#include <iostream>

ADSR::ADSR()
{
attack = 0.0; // instant on
attack_gain = 0.0;
decay = 0.0;
decay_gain = 0.0;
sustain = 1.0;
release = 0.0;
release_gain = 0.0;
ADSRstage = ATTACK;
adsr_gain = 0.0;
 }



ADSR::ADSR(float a, float d, float s, float r)
:
	attack{a},
	decay{d},
	sustain{s},
	release{r}

	{
		calcADRGain();
	}

ADSR::~ADSR() {
	// TODO Auto-generated destructor stub
}

/// calc stuff

void ADSR::calcADRGain()
	{
		attack_gain = (1.0 / attack) /(float) SAMPLERATE;
		decay_gain = -(1.0 / decay) / (float)SAMPLERATE;
		release_gain = (1.0 / release) / (float)SAMPLERATE;
	}

float ADSR::getADSR_gain(){
		// where are we in the envelope
	float m_returnValue {0};
		switch (ADSRstage)
		{
		    	case ATTACK:
		    		m_returnValue= attack_gain;

		    	break;
		    	case DECAY:
		    		m_returnValue=decay_gain;
		    	break;
		    	case SUSTAIN: m_returnValue= 0.0f;
		    	break;
		    	case RELEASE: m_returnValue=release_gain;
		    	break;
		}
		return m_returnValue;

	}

float ADSR::ADSRrun(bool * active){
	// attack phase ; should be triggered by note on

		if (ADSRstage == ATTACK && (adsr_gain < 1.0))
		  	adsr_gain+=attack_gain;

		if (ADSRstage == ATTACK && (adsr_gain >= 1.0))
		{
			adsr_gain = 1.0f;
			ADSRstage = DECAY;
			return adsr_gain;
		}
		// now go into decay
		if (ADSRstage == DECAY && adsr_gain > sustain)
			adsr_gain += decay_gain;
		if (ADSRstage == DECAY && adsr_gain <= sustain)
		{
			adsr_gain = sustain;
			ADSRstage = SUSTAIN;
			return adsr_gain;
		}
		// sustain
		if (ADSRstage == SUSTAIN)
			return adsr_gain;
		// release phase ; should be triggered by note off
		if (ADSRstage == RELEASE && adsr_gain > 0.0)
			{
			adsr_gain += release_gain;
			}
		// gain might have dipped below 0.0
		if (ADSRstage == RELEASE && adsr_gain <= 0.0f)
		{
			adsr_gain = 0.0f;
			*active = false;

		}
		return adsr_gain;
	}

void ADSR::setADSR(float a, float d, float s, float r)
{
  attack = a;
  decay = d;
  sustain = s;
  release = r;
  ADSR::calcADRGain();
}

void ADSR::initADSR()
{
  ADSRstage = ATTACK;
  adsr_gain = 0.0f;
}
