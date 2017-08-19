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
  this will hopefuly become a sample slicer plugin
 */

// constructor
class SlicerPlugin : public Plugin
{
public:
    SlicerPlugin()
        : Plugin(1, 0, 0) //1 parameter, 0 programs (presets) , 0 states
    {
        play_sample = 1.0f;
        Sample SampleObject{"sample.ogg"};
        sampleVector = SampleObject.getSampleVector();

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
        return "Slicer";
    }

   /**
      Get an extensive comment/description about the plugin.
    */
    const char* getDescription() const override
    {
        return "Slicer plugin";
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
        return d_cconst('d', 'N', 'f', 'o');
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
                
                int status = midiEvents[curEventIndex].data[0]; // midi status .. I asume we only use note info for the moment 
                int note = midiEvents[curEventIndex].data[1]; // note number .. can't be bothered too much about velocity, but that would be in .data[2]
                std::cout << status << std::endl; // just some debug output ;-)
                switch(status) 
                {
                    case 0x80 : // note off
                        sample_is_playing = 0; // turn the sample playing off
                        break;
                        
                    case 0x90 :
                        sample_is_playing = 1; // turn on playing
                        playbackIndex = 0; //reset playbackIndex
                        multiplierIndex = 0; //reset the fractional index needed for pitching
                        transpose = note - 60; // note 60 is c4, get the offset from that note
                        multiplier = pow(2.0, (float)transpose / 12.0); 
                        /* WIZARDRY AND MAGIC .. this sets the multiplier for the fractional index
                         * when transpose is 0 .. mutliplier = 1 => normal speed
                         * when transpose is 12 .. multiplier = 2 => double speed
                         * when transpose is -12 .. multiplier = 0.5 => half speed
                         * and this works for any transpose thrown at it
                         */
                                      
                       break;
                        
                }
                
              curEventIndex++; // we've processed a midi event,increase index so we know which midi event to process next
              }
				switch(sample_is_playing) // are we playing a sample ??
				{
					case 1 : // we are !!
                    {
					  if ( playbackIndex >= (sampleVector.size()-1) ) // reset index if we are at the end of the sample
                      {
                      playbackIndex = 0;
                      multiplierIndex = 0;
                      std::cout << "index reset" << playbackIndex << "-" << multiplierIndex << std::endl; // debug output
                      std::cout << sampleVector.size()<< std::endl; // debug outpu
                      }
                      outL[framesDone] = sampleVector[playbackIndex]; // copy sampledata to buffer
                      outR[framesDone] = sampleVector[playbackIndex+1];
                      multiplierIndex = multiplierIndex + multiplier; 
                      /* there's two index running. this one is needed to pitch the sample 
                      * 1 = normal speed .. we use every frame in the sample
                      * 0.5 = half speed .. we use every frame twice
                      * 2 = double speed .. we skip a frame
                      * this results in different pitch */
                      int tmp = multiplierIndex; // 'cat the float to int
                      tmp*=2; // double it to to sample align it to the stereo .playbackIndex should was be a power of 2 (0,2,4,6,8 ..)
                      playbackIndex = tmp; // could possibly skip some steps here, but to me at least it makes sense
                      
                      /* debug stuff .. this brings playback to a screaching halt!!!
                       * std::cout << std::fixed << sampleVector[playbackIndex] << ":" << sampleVector[playbackIndex+1] << "-" << playbackIndex << " | ";
                       * std::cout << multiplierIndex << "|"  << playbackIndex << ":";
                      */
                    break;
                    }
					case 0 :  // sample is not playing
                    {
					   outL[framesDone] = 0; // output 0 == silence
					   outR[framesDone] = 0;
					/* copy zeros */
					break;
                    }
				} 
			++framesDone;	
    } // the frames loop 
    } // run()

void loadSample()
    {
    /*  load sample stuff
     *  code from https://github.com/harryhaaren/openAudioProgrammingTutorials/blob/master/loopedSample/loopedSample.cpp
    */
    SndfileHandle fileHandle( "/home/rob/build/slicer/plugin-examples/plugins/Slicer/sample.ogg" , SFM_READ, SF_FORMAT_WAV | SF_FORMAT_FLOAT , 2 , 44100);
    int size = fileHandle.frames();
    
    /* stuff not needed for now
     * int channels = fileHandle.channels();
     * int samplerate = fileHandle.samplerate();
    */
    
    sampleVector.resize(size);
    fileHandle.read( &sampleVector.at(0), size);
   }

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
    //Sample SampleObject{"sample.ogg"};
    int sample_is_playing = 0; // flag if the sample is playing


   /**
      Set our plugin class as non-copyable and add a leak detector just in case.
    */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SlicerPlugin)
};

/* ------------------------------------------------------------------------------------------------------------
 * Plugin entry point, called by DPF to create a new plugin instance. */

Plugin* createPlugin()
{
    return new SlicerPlugin();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
