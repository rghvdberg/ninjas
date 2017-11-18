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





START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class NinjasUI : public UI,
    public ImageSwitch::Callback,
    public ImageKnob::Callback

{
public:
    NinjasUI();


protected:
    // -------------------------------------------------------------------
    // DSP Callbacks

    void parameterChanged(uint32_t index, float value) override;
    void uiFileBrowserSelected(const char* filename) override;
    void stateChanged(const char* key, const char* value) override;

    // -------------------------------------------------------------------
    // Widget Callbacks
    void imageSwitchClicked(ImageSwitch* ImageSwitch, bool down) override;

    void imageKnobDragStarted(ImageKnob* knob) override;
    void imageKnobDragFinished(ImageKnob* knob) override;
    void imageKnobValueChanged(ImageKnob* knob, float value) override;

    void onDisplay() override;

private:
    Image fImgBackground;
    ScopedPointer<ImageSwitch> fSwitchFwd, fSwitchFloppy, fSwitchRev, fSwitchLoopFwd, fSwitchLoopRev;
    ScopedPointer<ImageSwitch> fGrid[16];
    /*
    fGrid01, fGrid02, fGrid03 , fGrid04 , fGrid05, fGrid06, fGrid07, fGrid08, fGrid09, fGrid10 ,fGrid11, fGrid12 ,fGrid13, fGrid14, fGrid15, fGrid16;
   */
    ScopedPointer<ImageKnob> fKnobSlices, fKnobAttack, fKnobDecay, fKnobSustain, fKnobRelease;
    // char* filepath;
    
    void calcWaveform( String fp );
    std::array<int,556> waveform;
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NinjasUI)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // 
