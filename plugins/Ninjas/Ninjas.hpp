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
#include "Sample.h"
#include <string>
#include "Slice.h"
#include "Voice.h"
#include "ADSR.h"
#include "Mixer.h"
#include "Stack.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class NinjasPlugin : public Plugin
{
public:

    enum Parameters
    {
      
      // slice # 0
      paramAttack0 = 0,
      paramDecay0,
      paramSustain0,
      paramRelease0,
      paramOneShotFwd0,
      paramOneShotRev0,
      paramLoopFwd0,
      paramLoopRev0,
      paramSliceStart0,
      paramSliceEnd0,
      paramSliceActive0,
      // slice # 1
      paramAttack1, 
      paramDecay1,
      paramSustain1,
      paramRelease1,
      paramOneShotFwd1,
      paramOneShotRev1,
      paramLoopFwd1,
      paramLoopRev1,
      paramSliceStart1,
      paramSliceEnd1,
      paramSliceActive1,
            // slice # 2
      paramAttack2, 
      paramDecay2,
      paramSustain2,
      paramRelease2,
      paramOneShotFwd2,
      paramOneShotRev2,
      paramLoopFwd2,
      paramLoopRev2,
      paramSliceStart2,
      paramSliceEnd2,
      paramSliceActive2,
             // slice # 3
      paramAttack3, 
      paramDecay3,
      paramSustain3,
      paramRelease3,
      paramOneShotFwd3,
      paramOneShotRev3,
      paramLoopFwd3,
      paramLoopRev3,
      paramSliceStart3,
      paramSliceEnd3,
      paramSliceActive3,
                // slice # 4
      paramAttack4, 
      paramDecay4,
      paramSustain4,
      paramRelease4,
      paramOneShotFwd4,
      paramOneShotRev4,
      paramLoopFwd4,
      paramLoopRev4,
      paramSliceStart4,
      paramSliceEnd4,
      paramSliceActive4,
            // slice # 5
      paramAttack5, 
      paramDecay5,
      paramSustain5,
      paramRelease5,
      paramOneShotFwd5,
      paramOneShotRev5,
      paramLoopFwd5,
      paramLoopRev5,
      paramSliceStart5,
      paramSliceEnd5,
      paramSliceActive5,
            // slice # 6
      paramAttack6, 
      paramDecay6,
      paramSustain6,
      paramRelease6,
      paramOneShotFwd6,
      paramOneShotRev6,
      paramLoopFwd6,
      paramLoopRev6,
      paramSliceStart6,
      paramSliceEnd6,
      paramSliceActive6,
             // slice # 7
      paramAttack7, 
      paramDecay7,
      paramSustain7,
      paramRelease7,
      paramOneShotFwd7,
      paramOneShotRev7,
      paramLoopFwd7,
      paramLoopRev7,
      paramSliceStart7,
      paramSliceEnd7,
      paramSliceActive7,
                // slice # 8
      paramAttack8, 
      paramDecay8,
      paramSustain8,
      paramRelease8,
      paramOneShotFwd8,
      paramOneShotRev8,
      paramLoopFwd8,
      paramLoopRev8,
      paramSliceStart8,
      paramSliceEnd8,
      paramSliceActive8,
      // slice # 9
      paramAttack9,
      paramDecay9,
      paramSustain9,
      paramRelease9,
      paramOneShotFwd9,
      paramOneShotRev9,
      paramLoopFwd9,
      paramLoopRev9,
      paramSliceStart9,
      paramSliceEnd9,
      paramSliceActive9,
      // slice # 10
      paramAttack10, 
      paramDecay10,
      paramSustain10,
      paramRelease10,
      paramOneShotFwd10,
      paramOneShotRev10,
      paramLoopFwd10,
      paramLoopRev10,
      paramSliceStart10,
      paramSliceEnd10,
      paramSliceActive10,
            // slice # 11
      paramAttack11, 
      paramDecay11,
      paramSustain11,
      paramRelease11,
      paramOneShotFwd11,
      paramOneShotRev11,
      paramLoopFwd11,
      paramLoopRev11,
      paramSliceStart11,
      paramSliceEnd11,
      paramSliceActive11,
             // slice # 12
      paramAttack12, 
      paramDecay12,
      paramSustain12,
      paramRelease12,
      paramOneShotFwd12,
      paramOneShotRev12,
      paramLoopFwd12,
      paramLoopRev12,
      paramSliceStart12,
      paramSliceEnd12,
      paramSliceActive12,
                // slice # 13
      paramAttack13, 
      paramDecay13,
      paramSustain13,
      paramRelease13,
      paramOneShotFwd13,
      paramOneShotRev13,
      paramLoopFwd13,
      paramLoopRev13,
      paramSliceStart13,
      paramSliceEnd13,
      paramSliceActive13,
            // slice # 14
      paramAttack14, 
      paramDecay14,
      paramSustain14,
      paramRelease14,
      paramOneShotFwd14,
      paramOneShotRev14,
      paramLoopFwd14,
      paramLoopRev14,
      paramSliceStart14,
      paramSliceEnd14,
      paramSliceActive14,
            // slice # 15
      paramAttack15, 
      paramDecay15,
      paramSustain15,
      paramRelease15,
      paramOneShotFwd15,
      paramOneShotRev15,
      paramLoopFwd15,
      paramLoopRev15,
      paramSliceStart15,
      paramSliceEnd15,
      paramSliceActive15,
      paramNumberOfSlices,
      paramCount
    };

    NinjasPlugin();

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
        return d_version(1, 0, 0);
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('N', 'i', 'N', 'j');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter);

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void  setParameterValue(uint32_t index, float value) override;

    // -------------------------------------------------------------------
    // Process

    //void activate() override;
    void run(const float**, float** outputs, uint32_t frames, const MidiEvent* midiEvents, uint32_t midiEventCount) override;

    // -------------------------------------------------------------------

private:
    // Paramaters * 16 for 16 slices ... maybe like this
    float p_Attack[16];
    float p_Decay[16];
    float p_Sustain[16] { 1,1,1,1, 1,1,1,1, 1,1,1,1 ,1,1,1,1 };
    float p_Release[16];
    float p_OneShotFwd[16];
    float p_OneShotRev[16];
    float p_LoopFwd[16];
    float p_LoopRev[16];
    float p_SliceStart[16];
    float p_SliceEnd[16];
    float p_Slice_Active[16];
    float p_NumberOfSlices;
    
    

    // empty sample object
    std::vector<float> sampleVector; // this holds the sample data
    Sample SampleObject{"/home/rob/git/plugin-examples/plugins/Ninjas/sample.ogg"};
    /* create Mixer objects
    One for each audio channel
    after each iteration mix is put in audiouffer
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

    int pitchbend { 8192 };
    float gain { 1.0f };
    int voice_index { 0 };
    Slice a_slices[16];
    int slices = 1;
    int sliceSize;

    /**
       Set our plugin class as non-copyable and add a leak detector just in case.
     */


    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NinjasPlugin)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // NINJAS_HPP_INCLUDED
