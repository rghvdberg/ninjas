// system includes
#include <iostream>
#include <vector>
#include "sndfile.hh"
#include <tuple>

// local includes
#include "Sample.h"
#include <string>
#include "Slice.h"
#include "Voice.h"
#include "ADSR.h"
#include "Mixer.h"
#include "globals.h"
#include "Stack.h"

using namespace std;

// maybe move this to it's own class

int main()
{
    //init

    //create sample object, pass filepath to it right away
    Sample SampleObject("/home/rob/build/ninjas/sample.ogg");

    // get the vector holding the sample
    vector<float> mySample = SampleObject.getSampleVector();

    /*vector<float> mySample;
     fill vector with test data
    for (int i =0; i < 256; i++)
    {
    	int j = i /16;
    	float h = (float) j *100;
    	float k = (float) (i % 16);
    	float var = (h + k) / 10000.0;
    	mySample.push_back(var);
    	mySample.push_back(-var);
    }*/

    /*cout << mySample.size() << endl;

     get sample size (length)
    */
    int mySize = SampleObject.getSampleSize();

    /*int mySize = mySample.size();
    for (int i = 0; i <512;i++)
    {
    	cout << mySample[i] << " , ";
    }
    cout << endl;
    */
    cout << "Sample size :" << mySize << endl;

    // make slices
    vector<Slice> v_slices ;
    int slices {1};
    int sliceSize { mySize / slices };
    cout << "sliceSize :" << sliceSize << endl;
    for (int i=0, j=0 ; i < mySize; i+=sliceSize )
    {
        v_slices.push_back( Slice() ); // create slice
        // set start and end
        v_slices[j].setSliceStart( i );
        v_slices[j].setSliceEnd(i+sliceSize-1);
        //	cout << j << " : "  << i << " -> " << i+sliceSize-1 << endl;
        //	cout << "Slice :" << j << " : " << 	v_slices[j].getSliceStart() << "->" << v_slices[j].getSliceEnd() << endl;
        ++j;

    }
    // output array
    // prototyping run() process

    /*float outL[BUFFERSIZE] { };
    float outR[BUFFERSIZE] { };
    */
    uint32_t framesDone = 0;
    /*
    uint32_t curEventIndex = 0;
    */

    /* create Mixer objects
    One for each audio channel
    after each iteration mix is put in audiobuffer
     */
    Mixer mixL { };
    Mixer mixR { };

    /*Voice stack
    here we keep track of voices playing
    TODO timing stuff .. kinda important
     */
    Stack stack { };

    /*by example of the cars plugin create array of voices
    tried to create them 'on the fly' but that won't work.
     */
    Voice voices[128] { };
    /*Status   lsb      msb
     * 1110nnnn 0lllllll 0mmmmmmm
     * Pitch Bend Change. This message is sent to indicate a change in the pitch bender (wheel or lever, typically).
     * The pitch bender is measured by a fourteen bit value. Center (no pitch change) is 2000H.
     * Sensitivity is a function of the receiver, but may be set using RPN 0. (lllllll)
     * are the least significant 7 bits. (mmmmmmm) are the most significant 7 bits.
     */

    int pitchbend { 8192 };
    float gain { 1.0f };
    int voice_index { 0 };
    /*  becomese the main loop
     *

     */

    // first check for new midi events
    // get midi events .. suppose we receive note 60 on channel 1, velocity 64
    int status { 0x90 };
    //cout << "Input midi status (0x90) :";
    //cin >> hex >> status;
    //int status { 0x92 };
    int note { 60};
    int velocity { 127 };
    int channel { status & 0x0F }; // get midi channel
    int message { status & 0xF0 }; // get midi message

    // check if note message

    while (framesDone < (uint32_t) mySize)
    {

        if (message == 0x80 || (message == 0x90))
        {

            switch (message)
            {
            case (0x80):
            {
                // check if note is playing
                bool voice_playing = stack.check_Voice_Playing(channel, note);
                if (voice_playing == false)
                    break; // note wasn't playing anyway .. ignore
                if (voice_playing)
                {
                    // get the (pointer to) voice
                    Voice* vp = stack.get_Voice(channel, note);
                    if (!vp->active)
                        stack.remove_Voice(channel,note);
                    else
                        vp->adsr.ADSRstage=ADSR::RELEASE;
                    /* TODO .. think about how this works for pitchbend*/
                }
                break;
            }

            case (0x90):
            {
                // new note,
                // find empty "slot"
                int i { 0 };
                while ( (voices[i].active) && i <= 127 )
                {
                    //cout << i << " , " << voices[i].active << endl;
                    i++;
                }
                /* voices[i] should be free but lets check it
                 * also if no unactive voices are found it means that 128 voices are playing
                 * unlikely .. but
                 * TODO needs fixing, create proper cue (fifo)
                 */
                if (i>127)
                {
                    cout << "index out of range" << endl;
                    return -1;

                }
                else {
                    //set properties of voice and add to stack
                    voices[i].active = true;
                    voices[i].index= voice_index;
                    voices[i].channel = channel;
                    voices[i].notenumber = note;
                    voices[i].velocity = velocity;
                    voices[i].pitchbend = pitchbend;
                    voices[i].gain = (float) velocity / 127.0f;
                    voices[i].slice = &v_slices[channel];
                    voices[i].adsr.initADSR();
                    voices[i].adsr.setADSR(0.1f, 0.1f ,0.75f ,0.1f);
                    voices[i].position = 0;
                    // all set . add to stack
                    stack.add_Voice(&voices[i]);
                    //
                    voice_index++;
                    voice_index=voice_index%128;
                } // else

            } // case 0x90

            break;
            } // switch

        } // if note message

        // get all voices playing
        int max = stack.get_Stack_Size();

        // loop through active voices
        for (int i = 0; i < max ; ++i)
        {
            // get the raw samples from the voice
            // TODOD mono/stereo handling ..
            // float* pointer will allow any amount of samples to be pulled in
            int pos_debug = stack.get_Position(i);
            /*int start_debug = stack.get_Slice_Start(i);
            int end_debug = stack.get_Slice_End(i);
            */// cout << pos_debug << " | " << start_debug + pos_debug << " - " << end_debug << endl;
            float* sample = stack.get_Sample(i, &mySample);
            // cout << *sample << " - " << *(sample+1) << endl;

            float sampleL { *sample };
            float sampleR { *(sample + 1) };
            // get gain factor
            // process adsr to get the gain back
            float adsr_gain = stack.runADSR(i);
            gain = stack.get_Gain(i) * adsr_gain;

            cout << "Pos :"<< pos_debug << " Gain :" << gain << endl;

            sampleL = sampleL * gain;
            sampleR = sampleR * gain;
            // put samples in mixer
            mixL.add_Sample(sampleL);
            mixR.add_Sample(sampleR);
            // increase the sample position
            int channels = SampleObject.getSampleChannels();
            // cout << "sample channels :#" << channels << endl;
            stack.inc_Position(i, channels);


        } // end for loop through active voices
        message = 0x00;
        ++framesDone;
    }

    return 0;
}  // main
