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
#include "Ninjas.hpp"
//#include <sndfile.hh>
//#include <vector>
//#include <iostream>
//#include "Sample.h"
//#include <string>
//#include "Slice.h"
//#include "Voice.h"
//#include "ADSR.h"
//#include "Mixer.h"
//#include "Stack.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------


// constructor
NinjasPlugin::NinjasPlugin()
      : Plugin(paramCount, 0, 0) //1 parameter, 0 programs (presets) , 0 states
    {
        sampleVector = SampleObject.getSampleVector();
        std::cout << "sampleVector size =" << sampleVector.size() << std::endl;
        SampleObject.createSlices(&v_slices,slices);
        v_slices[0].setSlicePlayMode(Slice::ONE_SHOT_FWD);
    }

   /* --------------------------------------------------------------------------------------------------------
    * Init
   */

   /*
      Initialize the parameters.
      This function will be called once, shortly after the plugin is created.
    */
    void NinjasPlugin::initParameter(uint32_t index, Parameter& parameter)
    {
        switch(index)
        {
        case 0:
        {
            //parameter.hints      = ;
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            parameter.name   = "Attack";
            parameter.symbol = "attack";
            break;
        }
        case 1:
        {
            //parameter.hints      = ;
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            parameter.name   = "Decay";
            parameter.symbol = "decay";
            break;
        }
        case 2:
        {
            //parameter.hints      = ;
            parameter.ranges.def = 1.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            parameter.name   = "Sustain";
            parameter.symbol = "sustain";
            break;
        }
        case 3:
        {
            //parameter.hints      = ;
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            parameter.name   = "Release";
            parameter.symbol = "release";
            break;
        }
        case 4:
        {
            parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
            parameter.ranges.def = 1.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            parameter.name   = "One Shot Forward";
            parameter.symbol = "one_shot_fwd";
            break;
        }
        case 5:
        {
            parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            parameter.name   = "One Shot Reverse";
            parameter.symbol = "one_shot_rev";
            break;
        }
        case 6:
        {
            parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            parameter.name   = "Looped Play Forward";
            parameter.symbol = "loop_fwd";
            break;
        }
        case 7:
        {
            parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            parameter.name   = "Looped Play Reverse";
            parameter.symbol = "loop_rev";
            break;
        }
        } // switch


    }

   /* --------------------------------------------------------------------------------------------------------
    * Internal data
   */

   /**
      Get the current value of a parameter.
      The host may call this function from any context, including realtime processing.
    */
    float NinjasPlugin::getParameterValue(uint32_t index) const
    {
        return 0;

    }

   /**
      Change a parameter value.
      The host may call this function from any context, including realtime processing.
      When a parameter is marked as automable, you must ensure no non-realtime operations are performed.
      @note This function will only be called for parameter inputs.
    */
    void NinjasPlugin::setParameterValue(uint32_t index, float value)
    {
        // TODO handle stuff
    }

   /* --------------------------------------------------------------------------------------------------------
    * Audio/MIDI Processing */
   
   /*       inputs unused , outputs        , size of block we process, pointer to midi data       , number of midie events in current block */  
    void NinjasPlugin::run(const float**, float** outputs, uint32_t frames,          const MidiEvent* midiEvents, uint32_t midiEventCount)
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
                std::cout << std::hex << "Status : " << status << std::endl;
                std::cout << std::hex << "Channel : " << channel << std::endl;
                std::cout << std::hex << "Message : " << message << std::endl;
                std::cout << std::hex << "Note : " << note << std::endl;
                std::cout << std::hex << "Velocity : " << velocity << std::endl;

                // skip if midi received on channel bigger than the # of slices
                // note : each slice has it's own midi channel
                if (channel > (v_slices.size()-1))
                {
                    curEventIndex++;
                    continue;
                }
                /* TODO pitchbend .. */
                switch(message)
                {
                case 0x80 : // note off
                {
                    // check if note is playing
                    bool voice_playing = stack.check_Voice_Playing(channel, note);

                    //std::cout << "voice_playing = " << voice_playing << std::endl;
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
                        if (voices[i].slice->getSlicePlayMode() == Slice::LOOP_REV || voices[i].slice->getSlicePlayMode() == Slice::ONE_SHOT_REV)
                        {
                            voices[i].playbackIndex = voices[i].slice->getSliceEnd();
                            voices[i].multiplierIndex = voices[i].multiplierIndex = (voices[i].slice->getSliceEnd() - voices[i].slice->getSliceStart())/SampleObject.getSampleChannels();
                        }
                        else
                        {
                        voices[i].playbackIndex = 0;
                        voices[i].multiplierIndex = 0;
                        }
                        int transpose= note-60;
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
                        std::cout << "removing voice " << i << std::endl;
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


/* ------------------------------------------------------------------------------------------------------------
 * Plugin entry point, called by DPF to create a new plugin instance. */

Plugin* createPlugin()
{
    return new NinjasPlugin();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
