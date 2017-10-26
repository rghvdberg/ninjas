/*
 * Mixer.h
 *
 *  Created on: 13 aug. 2017
 *      Author: rob
 */

#ifndef MIXER_H_
#define MIXER_H_

class Mixer {
public:
    Mixer();
    virtual ~Mixer();

    void add_Sample(float s);
    float get_Mix();

private:
    float mix;
    int channels;
    float average;
};


#endif /* MIXER_H_ */
