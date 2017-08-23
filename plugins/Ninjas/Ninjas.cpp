/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2015 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "DistrhoPlugin.hpp"
#include <sndfile.hh>
#include <vector>
#include <iostream>
#include "Sample.h"
#include <string>
#include "Slice.h"
#include "Voice.h"
#include "ADSR.h"
#include "Mixer.h"
#include "Stack.h"
START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

/**
  this will hopefuly become a sample Ninjas plugin
 */

// constructor
class NinjasPlugin : public Plugin
{
public:
    NinjasPlugin()
        : Plugin(1, 0, 0) //1 parameter, 0 programs (presets) , 0 states
    {
        play_sample = 1.0f;
        sampleVector = SampleObject.getSampleVector();
        SampleObject.createSlices(&v_slices,slices);
        v_slices[0].setSlicePlayMode(Slice::LOOP_REV);

    }

protected:
   /* --------------------------------------------------------------------------------------------------------
    * Information */

   /**
      Get the plugin label.
      This label is a short restricted name consisting of only _, a-z, A-Z and 0-9 characters.
    */
    const char* getLabel() const override
    {
        return "Ninjas";
    }

   /**
      Get an extensive comment/description about the plugin.
    */
    const char* getDescription() const override
    {
        return "Ninjas is not just another slicer";
    }

   /**
      Get the plugin author/maker.
    */
    const char* getMaker() const override
    {
        return "Clearly Broken Software";
    }

   /**
      Get the plugin homepage.
    */
    const char* getHomePage() const override
    {
        return "https://github.com/DISTRHO/plugin-examples";
    }

   /**
      Get the plugin license name (a single line of text).
      For commercial plugins this should return some short copyright information.
    */
    const char* getLicense() const override
    {
        return "ISC";
    }

   /**
      Get the plugin version, in hexadecimal.
    */
    uint32_t getVersion() const override
    {
        return d_version(1, 0, 0);
    }

   /**
      Get the plugin unique Id.
      This value is used by LADSPA, DSSI and VST plugin formats.
    */
   
   // don't bother too much about this :-) 
    int64_t getUniqueId() const override
    {
        return d_cconst('N', 'i', 'N', 'j');
    }

   /* --------------------------------------------------------------------------------------------------------
    * Init */

   /**
      Initialize the parameters.
      This function will be called once, shortly after the plugin is created.
    */
    void initParameter(uint32_t index, Parameter& parameter) override
    {
        /* I've added 1 paramater, actually not used anymore in current state of the code
         * I chose a boolean so that it shows up as a toggle in generic ui
         * all paramater should be float, even if it acts like a boolean
         */
        parameter.hints      = kParameterIsAutomable|kParameterIsBoolean;
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.name   = "Play Sample";
        parameter.symbol = "play_sample";

    }

   /* --------------------------------------------------------------------------------------------------------
    * Internal data */

   /**
      Get the current value of a parameter.
      The host may call this function from any context, including realtime processing.
    */
    float getParameterValue(uint32_t index) const override
    {
        return play_sample;

    }

   /**
      Change a parameter value.
      The host may call this function from any context, including realtime processing.
      When a parameter is marked as automable, you must ensure no non-realtime operations are performed.
      @note This function will only be called for parameter inputs.
    */
    void setParameterValue(uint32_t index, float value) override
    {
        play_sample = value;
    }

   /* --------------------------------------------------------------------------------------------------------
    * Audio/MIDI Processing */
   
   /*       inputs unused , outputs        , size of block we process, pointer to midi data       , number of midie events in current block */  
    void run(const float**, float** outputs, uint32_t frames,          const MidiEvent* midiEvents, uint32_t midiEventCount)
    {
        float* const outL = outputs[0]; // output ports , stereo
        float* const outR = outputs[1];
        uint32_t framesDone = 0;
        uint32_t curEventIndex = 0; // index for midi event to process

        while (framesDone < frames) // we have frames to process !!
        {
            /* process any ready midi events */
            //   we have midi data to proces, at precisly the current audio frame in the loop
            while (curEventIndex < midiEventCount && framesDone == midiEvents[curEventIndex].frame) // the .frame is the offset of the midi event in current block
            {
                if (midiEvents[curEventIndex].size > MidiEvent::kDataSize) // not excatly shure what's happening here. this is in both Nekobi and Kars sourcecode
                    continue;
                
                int status = midiEvents[curEventIndex].data[0]; // midi status
                int channel = status & 0x0F ; // get midi channel
                int message = status & 0xF0 ; // get midi message
                int note = midiEvents[curEventIndex].data[1];// note number
                int velocity = midiEvents[curEventIndex].data[2]; //
                /* TODO pitchbend .. */
                switch(message)
                {
                case 0x80 : // note off
                {
                    // check if note is playing
                    bool voice_playing = stack.check_Voice_Playing(channel, note);

                    std::cout << "voice_playing = " << voice_playing << std::endl;
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
                        std::cout << "Line 204 : ADSRstage =" << vp->adsr.ADSRstage << std::endl;
                        /* TODO .. think about how this works for pitchbend*/
                    }
                    // sample_is_playing = 0; // turn the sample playing off
                    break;
                }

                case 0x90 :
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
                        std::cout << "index out of range" << std::endl;
                        // return -1;
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
                        voices[i].adsr.setADSR(0.1f, 0.1f ,1.0f ,0.1f);
                        voices[i].playbackIndex = 0;
                        voices[i].multiplierIndex = 0;
                        transpose= note-60;
                        voices[i].multiplier=pow(2.0, (float)transpose / 12.0);
                        // all set . add to stack
                        stack.add_Voice(&voices[i]);
                        //
                        voice_index++;
                        voice_index=voice_index%128;
                    } // else

                } // case 0x90

                    break;
                } // switch

                
                curEventIndex++; // we've processed a midi event,increase index so we know which midi event to process next
            }
            // get all voices playing
            int max = stack.get_Stack_Size();
            if (max > 0)
            {
                // loop through active voices
                for (int i = 0; i < max ; ++i)
                {
                    /*get the raw samples from the voice
                TODOD mono/stereo handling ..
                float* pointer will allow any amount of samples to be pulled in
                */

                    /*debug stuff
                int pos_debug = stack.get_Position(i);
                int start_debug = stack.get_Slice_Start(i);
                int end_debug = stack.get_Slice_End(i);
                cout << pos_debug << " | " << start_debug + pos_debug << " - " << end_debug << endl;*/
                    if (stack.get_Voice_Active(i))
                    {
                        float* sample = stack.get_Sample(i, &sampleVector);
                        // cout << *sample << " - " << *(sample+1) << endl;

                        float sampleL { *sample };
                        float sampleR { *(sample + (SampleObject.getSampleChannels() -1) ) };
                        // TODO should make variable for channels

                        // get gain factor
                        // process adsr to get the gain back
                        float adsr_gain = stack.runADSR(i);
                        // std::cout<<"adsr_gain =" << adsr_gain << std::endl;

                        gain = stack.get_Gain(i) * adsr_gain;
                        // cout << "Pos :"<< pos_debug << " Gain :" << gain << endl;

                        sampleL = sampleL * gain;
                        sampleR = sampleR * gain;
                        // put samples in mixer
                        mixL.add_Sample(sampleL);
                        mixR.add_Sample(sampleR);
                        // increase the sample read index
                        int channels = SampleObject.getSampleChannels();
                        stack.inc_Position(i, channels);
                    }
                    else
                    {
                        stack.remove_Voice(i);
                        mixL.add_Sample(0.0f);
                        mixR.add_Sample(0.0f);
                    }
                } // end for loop through active voices
                // get mixer
                float left = mixL.get_Mix();
                float right = mixR.get_Mix();
                outL[framesDone] = left;
                outR[framesDone] = right;
            } // if max > 0
            else
            {
                // no voices playing
                outL[framesDone] = 0; // output 0 == silence
                outR[framesDone] = 0;
            }


            /*
                switch(sample_is_playing) // are we playing a sample ??
                {
                    case 1 : // we are !!
                    {
                      if ( playbackIndex >= (sampleVector.size()-1) ) // reset index if we are at the end of the sample
                      {
                      playbackIndex = 0;
                      multiplierIndex = 0;
                      /*
                      std::cout << "index reset" << playbackIndex << "-" << multiplierIndex << std::endl; // debug output
                      std::cout << sampleVector.size()<< std::endl; // debug output

                      }
                      outL[framesDone] = sampleVector[playbackIndex]; // copy sampledata to buffer
                      outR[framesDone] = sampleVector[playbackIndex+1];

                      /* there's two index running. this one is needed to pitch the sample
                       1 = normal speed .. we use every frame in the sample
                      * 0.5 = half speed .. we use every frame twice
                      * 2 = double speed .. we skip a frame
                      * this results in different pitch

                      multiplierIndex = multiplierIndex + multiplier;
                      int tmp = (int) multiplierIndex;
                      tmp*=channels;
                      playbackIndex = tmp ;
                      int tmp = multiplierIndex; // cast the float to int
                      tmp*=2; // double it to to sample-align it to the stereo .playbackIndex should was be a power of 2 (0,2,4,6,8 ..)
                     playbackIndex=tmp; // could possibly skip some steps here, but to me at least it makes sense

                      /* debug stuff .. this brings playback to a screaching halt!!!
                       * std::cout << std::fixed << sampleVector[playbackIndex] << ":" << sampleVector[playbackIndex+1] << "-" << playbackIndex << " | ";
                       * std::cout << multiplierIndex << "|"  << playbackIndex << ":";


                    break;
                    }
                    case 0 :  // sample is not playing
                    {
                       outL[framesDone] = 0; // output 0 == silence
                       outR[framesDone] = 0;
                     //copy zeros
                    break;
                    }
                }*/

            ++framesDone;
        } // the frames loop
    } // run()


    // -------------------------------------------------------------------------------------------------------

private:
    // Parameters
    float play_sample;
    
    // variables needed in run()
    int playbackIndex = 0;
    float multiplierIndex = 0.0; // the 'speed' of the sample .. this translates in pitch 
    int transpose = 0; // offset from C4 in semitones
    float multiplier = pow(2.0, (float)transpose / 12.0); 
    // sample variables
    // empty sample object
    std::vector<float> sampleVector; // this holds the sample data
    Sample SampleObject{"/home/rob/git/plugin-examples/plugins/Ninjas/sample.ogg"};
    int sample_is_playing = 0; // flag if the sample is playing

    /* create Mixer objects
    One for each audio channel
    after each iteration mix is put in audiobuffer
     */
    Mixer mixL;
    Mixer mixR;

    /*Voice stack
    here we keep track of voices playing
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
    std::vector<Slice> v_slices ;
    int slices = 1;
    int sliceSize;
    /*	std::cout << "sliceSize :" << sliceSize << std::endl;
    	for (int i=0, j=0 ; i < mySize; i+=sliceSize )
    	{
    		v_slices.push_back( Slice() ); // create slice
    		// set start and end
    		v_slices[j].setSliceStart( i );
    		v_slices[j].setSliceEnd(i+sliceSize-1);
    		debug
    		cout << j << " : "  << i << " -> " << i+sliceSize-1 << endl;
    		cout << "Slice :" << j << " : " << 	v_slices[j].getSliceStart() << "->" << v_slices[j].getSliceEnd() << endl;

    		++j;
    	}
*/


   /**
      Set our plugin class as non-copyable and add a leak detector just in case.
    */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NinjasPlugin)
};

/* ------------------------------------------------------------------------------------------------------------
 * Plugin entry point, called by DPF to create a new plugin instance. */

Plugin* createPlugin()
{
    return new NinjasPlugin();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
