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
#include <sndfile.hh>
#include <vector>
#include <iostream>
#include <string>
#include <limits>
#include "Sample.h"

#include "Slice.h"
#include "Voice.h"
#include "ADSR.h"
#include "Mixer.h"
#include "Stack.h"

#include "DistrhoPluginInfo.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------


// constructor
NinjasPlugin::NinjasPlugin()
    : Plugin(paramCount, 0, 0) //1 parameter, 0 programs (presets) , 0 states
{
    //sampleVector = SampleObject.getSampleVector();
    //std::cout << "sampleVector size =" << sampleVector.size() << std::endl;
    //SampleObject.createSlices(a_slices,slices);
    //a_slices[0].setSlicePlayMode(Slice::ONE_SHOT_FWD);
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
    case paramNumberOfSlices:
    {
        parameter.hints      = kParameterIsAutomable|kParameterIsInteger;
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 16.0f;
        parameter.name   = "Slices";
        parameter.symbol  = "number_of_slices";
    }
     case paramAttack:
    {
        //parameter.hints      = ;
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.name   = "Attack";
        parameter.symbol = "attack";
        break;
    }
    case paramDecay:
    {
        //parameter.hints      = ;
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.name   = "Decay";
        parameter.symbol =  "decay";
        break;
    }
    case paramSustain:
    {
        //parameter.hints      = ;
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.name = "Sustain";
        parameter.symbol = "sustain";
        break;
    }
    case paramRelease:
    {
        //parameter.hints      = ;
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.name   = "Release";
        parameter.symbol = "release";
        break;
    }
    case paramOneShotFwd:
    {
        parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.name   = "One Shot Forward";
        parameter.symbol  = "one_shot_fwd";
        break;
    }
    case paramOneShotRev:
    {
        parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.name   = "One Shot Reverse";
        parameter.symbol  = "one_shot_rev";
        break;
    }
    case paramLoopFwd:
    {
        parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.name   = "Looped Play Forward";
        parameter.symbol  = "loop_fwd";
        break;
    }
    case paramLoopRev:
    {
        parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.name   = "Looped Play Reverse";
        parameter.symbol  = "loop_rev";
        break;
    }
    case paramFloppy:
        parameter.hints = kParameterIsBoolean;
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.name   = "Floppy";
        parameter.symbol  = "floppy";
    }

    if (index > 9 )
    {
        parameter.hints      = kParameterIsBoolean;
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.name   = "Switch "+String(index-9);
        parameter.symbol  = "switch"+String(index-9);
    }

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
    float return_Value = 0;
    /* TODO derive channel from switch matrix | might be fixed ? */
    int ch = currentSlice;
    switch(index)
    {
    case paramNumberOfSlices:
        return_Value = slices;
    case paramAttack:
        return_Value = p_Attack[ch];
        break;
    case paramDecay:
        return_Value = p_Decay[ch];
        break;
    case paramSustain:
        return_Value = p_Sustain[ch];
        break;
    case paramRelease:
        return_Value = p_Release[ch];
        break;
    case paramOneShotFwd: // one shot forward
        if (a_slices[ch].getSlicePlayMode() == Slice::ONE_SHOT_FWD)
            return_Value = 1;
        else
            return_Value = 0;
        break;
    case paramOneShotRev: // one shot Reverse
        if (a_slices[ch].getSlicePlayMode() == Slice::ONE_SHOT_REV)
            return_Value = 1;
        else
            return_Value = 0;
        break;
    case paramLoopFwd: // Loop Fwd
        if (a_slices[ch].getSlicePlayMode() == Slice::LOOP_FWD)
            return_Value = 1;
        else
            return_Value = 0;
        break;
    case paramLoopRev: // Loop Rev
        if (a_slices[ch].getSlicePlayMode() == Slice::LOOP_REV)
            return_Value = 1;
        else
            return_Value = 0;
        break;
    }
    if (index > 9)
    {
        return_Value = (float) currentSlice;
    }
    return return_Value;

}

/**
    Change a parameter value.
    The host may call this function from any context, including realtime processing.
    When a parameter is marked as automable, you must ensure no non-realtime operations are performed.
    @note This function will only be called for parameter inputs.
  */
void NinjasPlugin::setParameterValue(uint32_t index, float value)
{
    int ch = currentSlice; /* TODO calc channel | maybe this is fixed now */

    //TODO rework this !!!
    switch(index)
    {
    case paramNumberOfSlices:
        slices = (int) value;
        SampleObject.createSlices(a_slices,slices);
        break;
    case paramAttack:
        p_Attack[ch] = value;
        break;
    case paramDecay:
        p_Decay[ch] = value;
        break;
    case paramSustain:
        p_Sustain[ch] = value;
        break;
    case paramRelease:
        p_Release[ch] = value;
        break;
    case paramOneShotFwd: // one shot forward
        if (value == 1)
            a_slices[ch].setSlicePlayMode(Slice::ONE_SHOT_FWD);
        break;
    case paramOneShotRev: // one shot Reverse
        if (value == 1)
            a_slices[ch].setSlicePlayMode(Slice::ONE_SHOT_REV);
        break;
    case paramLoopFwd: // Loop Fwd
        if (value == 1)
            a_slices[ch].setSlicePlayMode(Slice::LOOP_FWD);
        break;
    case paramLoopRev: // Loop Rev
        if (value == 1)
            a_slices[ch].setSlicePlayMode(Slice::LOOP_REV);
        break;

    } // switch
    if (index > 9)
      currentSlice = index - 9; 
} // setParameterValue

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
            if (channel > slices -1)
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
                    voices[i].gain = (float) velocity / 127.0f;
                    voices[i].adsr.initADSR();
                    voices[i].adsr.setADSR(p_Attack[channel], p_Decay[channel] ,p_Sustain[channel],p_Release[channel]);
                    if (a_slices[channel].getSlicePlayMode() == Slice::LOOP_REV || a_slices[channel].getSlicePlayMode() == Slice::ONE_SHOT_REV)
                    {
                        voices[i].playbackIndex = a_slices[channel].getSliceEnd();
                        voices[i].multiplierIndex = ( a_slices[channel].getSliceEnd() - a_slices[channel].getSliceStart()) /SampleObject.getSampleChannels();
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

                if (stack.get_Voice_Active(i))
                {
                    float* sample = stack.get_Sample(i, &sampleVector, a_slices);
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
                    stack.inc_Position(i, channels, a_slices);
                }
                else
                {
                    //std::cout << "removing voice " << i << std::endl;
                    //stack.remove_Voice(i);
                    mixL.add_Sample(0.0f);
                    mixR.add_Sample(0.0f);
                }
            } // end for loop through active voices

            stack.remove_inactive_voices();
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
