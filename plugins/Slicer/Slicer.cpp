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
START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

/**
  this will hopefuly become a sample slicer plugin
 */
class SlicerPlugin : public Plugin
{
public:
    SlicerPlugin()
        : Plugin(1, 0, 0)
    {
        play_sample = 1.0f;
        loadSample();

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
    int64_t getUniqueId() const override
    {
        return d_cconst('d', 'N', 'f', 'o');
    }

   /* --------------------------------------------------------------------------------------------------------
    * Init */

   /**
      Initialize the parameter @a index.
      This function will be called once, shortly after the plugin is created.
    */
    void initParameter(uint32_t index, Parameter& parameter) override
    {
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

    void run(const float**, float** outputs, uint32_t frames, const MidiEvent* midiEvents, uint32_t midiEventCount)
    {
       float* const outL = outputs[0];
       float* const outR = outputs[1];
       // get midi events
       if (midiEventCount > 0)
       {
		   for (int curEventIndex = 0; curEventIndex < midiEventCount;++curEventIndex)
		    {
				const uint8_t*  data = midiEvents[curEventIndex].data; // pointer to midi data
				std::cout << "Total events in buffer : "<< midiEventCount << " Current event : "<<curEventIndex << " - ";
				std::cout << "Data : " << (int)data[0] << "," << (int)data[1] << "," << (int)data[2] <<" - ";
				std::cout << "Offset in frames " << midiEvents[curEventIndex].frame << std::endl;
			}
		}
       //

        for ( int i = 0; i < frames; i++)
			{
				switch(midinote)
				{
					case 1 :
					if ( playbackIndex >= (sampleVector.size()-1) ) {
                      playbackIndex = 0;
                      }
                      outL[i]  = sampleVector[playbackIndex];
                      outR[i] = sampleVector[playbackIndex+1];
                      playbackIndex +=2;
                    break;
					case 0 :
					   outL[i] = 0;
					   outR[i] = 0;
					/* copy zeros */
					break;
				}
    }
    }
void loadSample()
    {
    /*  load sample stuff
     *  code from https://github.com/harryhaaren/openAudioProgrammingTutorials/blob/master/loopedSample/loopedSample.cpp
    */
    SndfileHandle fileHandle( "/home/rob/build/slicer/plugin-examples/plugins/Slicer/sample.wav" , SFM_READ, SF_FORMAT_WAV | SF_FORMAT_FLOAT , 2 , 44100);
    int size = fileHandle.frames();
    // int channels = fileHandle.channels();
    // int samplerate = fileHandle.samplerate();
    sampleVector.resize(size);
    fileHandle.read( &sampleVector.at(0), size);
    }

    // -------------------------------------------------------------------------------------------------------

private:
    // Parameters
    float play_sample;
    int playbackIndex = 0;
    // sample variables
    std::vector<float> sampleVector;
    int midinote = 0;
    //

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
