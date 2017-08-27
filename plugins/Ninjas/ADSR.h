/*
 * ADSR.h
 *	 attack; // time value : 1 is one second
	 decay; // time value
	 sustain; // gain value : 1 is max gain
	 release; // time value : after note off , voice is active until release hits zero

 *  Created on: 11 aug. 2017
 *      Author: rob
 */

#ifndef ADSR_H_
#define ADSR_H_

class ADSR {
public:
	ADSR();
	ADSR(float a, float d, float s, float r);
	virtual ~ADSR();

	float getADSR_gain(); // returns gain factor for current frame
	void setADSR(float a, float d, float s, float r); // setter, don't see the value of setting each param separately
    void setAttack(float a)
    {
        attack = a;
    }

    float getAttack() const
    {
        return attack;
    }

	float  ADSRrun(bool * active); //only call this when Voice.active == true
	enum stage_of_ADSR {
				ATTACK,
				DECAY,
				SUSTAIN,
				RELEASE
				};
	stage_of_ADSR ADSRstage;
	void initADSR();

private:
	float attack; // time value how long the gain takes from 0.0 to 1.0f ..
	float attack_gain; // each frame this is added to gain of note (max 1.0f)
	float decay; // time value from decay (1.0f) to sustain
	float decay_gain; // negative value, each frame this is subtracted from gain
	float sustain; // gain value : 1.0f is max gain
	float release; // time value : after note off , voice is active until release hits 0.0f
	float release_gain; // negative value, each frame subtracted from gain until 0.0f
	float adsr_gain; // multiply this by gain and we get the ... well gain
	void calcADRGain();


};

#endif /* ADSR_H_ */
