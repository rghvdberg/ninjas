/*
 * DISTRHO Kars Plugin, based on karplong by Chris Cannam.
 * Copyright (C) 2015 Filipe Coelho <falktx@falktx.com>
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

#ifndef NINJAS_UI_HPP_INCLUDED
#define NINJAS_UI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "ImageWidgets.hpp"
#include "NinjasArtwork.hpp"
#include "sndfile.hh"
#include "DistrhoPlugin.hpp"
#include "iterator"
#include "algorithm"
#include "array"
#include "aubio/aubio.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class NinjasUI : public UI, 
		 public ImageSwitch::Callback,
		 public ImageKnob::Callback,
		 public ImageSlider::Callback
{
public:
    NinjasUI();

protected:
    // -------------------------------------------------------------------
    // DSP Callbacks

    void parameterChanged ( uint32_t index, float value ) override;
    void uiFileBrowserSelected ( const char* filename ) override;
    void stateChanged ( const char* key, const char* value ) override;

    // -------------------------------------------------------------------
    // Widget Callbacks
    void imageSwitchClicked ( ImageSwitch* ImageSwitch, bool down ) override;
    void imageKnobDragStarted ( ImageKnob* knob ) override;
    void imageKnobDragFinished ( ImageKnob* knob ) override;
    void imageKnobValueChanged ( ImageKnob* knob, float value ) override;
    void imageSliderDragStarted(ImageSlider* slider) override;
    void imageSliderDragFinished(ImageSlider* slider) override;
    void imageSliderValueChanged(ImageSlider* slider, float value) override;
    
    void onDisplay() override;

private:
    Image fImgBackground;
    ScopedPointer<ImageSwitch> fSwitchFwd, fSwitchFloppy, fSwitchRev, fSwitchLoopFwd, fSwitchLoopRev;
    ScopedPointer<ImageSwitch> fGrid[16];
    ScopedPointer<ImageKnob> fKnobSlices, fKnobAttack, fKnobDecay, fKnobSustain, fKnobRelease;
    ScopedPointer<ImageSlider> fSliceModeSlider;
        
    void calcWaveform ( String fp );
    void recallSliceSettings ( int slice );
    std::array<int,1112> waveform;
    float p_Attack[16] { 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05 };
    float p_Decay[16] { 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05 };
    float p_Sustain[16] { 1,1,1,1, 1,1,1,1, 1,1,1,1 ,1,1,1,1 };
    float p_Release[16] { 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05 };
    float p_OneShotFwd[16] { 1,1,1,1, 1,1,1,1, 1,1,1,1 ,1,1,1,1 };
    float p_OneShotRev[16];
    float p_LoopFwd[16];
    float p_LoopRev[16];
    float p_SliceStart[16];
    float p_SliceEnd[16];
    int currentSlice {0};

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR ( NinjasUI )
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // 
