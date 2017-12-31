/*
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

#ifndef NINJAS_HPP_INCLUDED
#define NINJAS_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include <sndfile.hh>
#include <vector>
#include <iostream>

#include <algorithm>
#include "Sample.h"
#include <string>
#include "Slice.h"
#include "Voice.h"
#include "ADSR.h"
#include "Mixer.h"
#include "../../deps/include/aubio/aubio.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class NinjasPlugin : public Plugin
{
public:

    NinjasPlugin();
    Sample SampleObject;

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "Ninjas";
    }

    const char* getDescription() const override
    {
        return "Ninjas Is Not Just Another Slicer";
    }

    const char* getMaker() const noexcept override
    {
        return "Clearly Broken Software";
    }

    const char* getHomePage() const override
    {
        return "https://rghvdberg.github.io/";
    }

    const char* getLicense() const noexcept override
    {
        return "ISC";
    }

    uint32_t getVersion() const noexcept override
    {
        return d_version ( 1, 0, 0 );
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst ( 'N', 'i', 'N', 'j' );
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter ( uint32_t index, Parameter& parameter );
    void initState ( uint32_t index, String& stateKey, String& defaultStateValue );

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue ( uint32_t index ) const override;
    void  setParameterValue ( uint32_t index, float value ) override;
    String getState ( const char* key ) const;
    void setState ( const char* key, const char* value ) override;


    // -------------------------------------------------------------------
    // Process

    //void activate() override;
    void run ( const float**, float** outputs, uint32_t frames, const MidiEvent* midiEvents, uint32_t midiEventCount ) override;

    // -------------------------------------------------------------------

private:
  
    void createSlicesRaw ( Slice* slices, int n_slices, int64_t size, int channels  );
    void getOnsets (int64_t size, int channels, std::vector<float> & sampleVector, std::vector<uint_t> & onsets);
    void createSlicesOnsets(std::vector<uint_t> & onsets,Slice* slices, int n_slices, int64_t size, int channels );
    int64_t find_nearest(std::vector<uint_t> & haystack, uint_t needle);
  
    // Paramaters * 16 for 16 slices ... maybe like this
    float p_Attack[16] { 0.01,0.01,0.01,0.01, 0.01,0.01,0.01,0.01, 0.01,0.01,0.01,0.01, 0.01,0.01,0.01,0.01 };
    float p_Decay[16] { 0.01,0.01,0.01,0.01, 0.01,0.01,0.01,0.01, 0.01,0.01,0.01,0.01, 0.01,0.01,0.01,0.01 };
    float p_Sustain[16] { 1,1,1,1, 1,1,1,1, 1,1,1,1 ,1,1,1,1 };
    float p_Release[16] { 0.01,0.01,0.01,0.01, 0.01,0.01,0.01,0.01, 0.01,0.01,0.01,0.01, 0.01,0.01,0.01,0.01 };
    float p_OneShotFwd[16] { 1,1,1,1, 1,1,1,1, 1,1,1,1 ,1,1,1,1 };
    float p_OneShotRev[16];
    float p_LoopFwd[16];
    float p_LoopRev[16];
    float p_SliceStart[16];
    float p_SliceEnd[16];
    float p_Grid[16] {1 ,0 ,0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float slicemode {0};
    // empty sample object

    std::vector<float> sampleVector; // this holds the sample data

    /* create Mixer objects
    One for each audio channel
    after each iteration mix is put in audiouffer
     */
    Mixer mixL;
    Mixer mixR;

    int pitchbend { 8192 };
    int pitchbend_range { 24 };
    float pitchbend_step = ( float ) 16384 / ( float ) pitchbend_range;
    float gain { 1.0f };
    int voice_index { 0 };
    Slice a_slices[16];
    int slices {1};
    // int sliceSize;
    int currentSlice {0};
    std::string filepath = "";
    bool bypass {true};
        
    std::vector<uint_t>onsets;
    double samplerate = getSampleRate();

    /*by example of the cars plugin create array of voices
    tried to create them 'on the fly' but that won't work.
    */

    Voice voices[16]
    {
        Voice ( samplerate ),Voice ( samplerate ),Voice ( samplerate ),Voice ( samplerate ),
        Voice ( samplerate ),Voice ( samplerate ),Voice ( samplerate ),Voice ( samplerate ),
        Voice ( samplerate ),Voice ( samplerate ),Voice ( samplerate ),Voice ( samplerate ),
        Voice ( samplerate ),Voice ( samplerate ),Voice ( samplerate ),Voice ( samplerate )
    };

    /**
       Set our plugin class as non-copyable and add a leak detector just in case.
     */


    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR ( NinjasPlugin )
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // NINJAS_HPP_INCLUDED
