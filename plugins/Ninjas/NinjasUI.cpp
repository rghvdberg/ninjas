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

#include "DistrhoPluginInfo.h"
#include "DistrhoUI.hpp"
#include "NinjasUI.hpp"
#include "NinjasArtwork.hpp"
#include "Ninjas.hpp"


START_NAMESPACE_DISTRHO

namespace Art = NinjasArtwork;

// -----------------------------------------------------------------------------------------------------------
NinjasUI::NinjasUI()
    : UI(Art::backgroundWidth, Art::backgroundHeight),
      fImgBackground(Art::backgroundData, Art::backgroundWidth, Art::backgroundHeight, GL_BGR)
{
  // knobs
  
  fKnobSlices = new ImageKnob(this,
		    Image(Art::rotary_slicesData, Art::rotary_slicesWidth, Art::rotary_slicesHeight, GL_BGR));
    fKnobSlices->setId(2); // TODO: implement proper parameters
    fKnobSlices->setAbsolutePos(279, 80);
    fKnobSlices->setRange(1.0f, 16.0f);
    fKnobSlices->setDefault(1.0f);
    fKnobSlices->setValue(1.0f);
    fKnobSlices->setImageLayerCount(16);
    fKnobSlices->setStep(1.0f);
    fKnobSlices->setCallback(this);
    
  // ADSR   
    fKnobAttack = new ImageKnob(this,
		    Image(Art::rotary_adsrData, Art::rotary_adsrWidth, Art::rotary_adsrHeight, GL_BGRA));
    fKnobAttack->setId(3); // TODO: implement proper parameters
    fKnobAttack->setAbsolutePos(560, 256);
    fKnobAttack->setRange(0.0f, 1.0f);
    fKnobAttack->setDefault(0.0f);
    fKnobAttack->setValue(0.0f);
    fKnobAttack->setRotationAngle(300);
    //fKnobAttack->setImageLayerCount(16);
    //fKnobAttack->setStep(1.0f);
    fKnobAttack->setCallback(this);
    
    fKnobDecay = new ImageKnob(this,
		    Image(Art::rotary_adsrData, Art::rotary_adsrWidth, Art::rotary_adsrHeight, GL_BGRA));
    fKnobDecay->setId(4); // TODO: implement proper parameters
    fKnobDecay->setAbsolutePos(637, 256);
    fKnobDecay->setRange(0.0f, 1.0f);
    fKnobDecay->setDefault(0.0f);
    fKnobDecay->setValue(0.0f);
    fKnobDecay->setRotationAngle(300);
    //fKnobDecay->setImageLayerCount(16);
    //fKnobDecay->setStep(1.0f);
    fKnobDecay->setCallback(this);
    
    fKnobSustain = new ImageKnob(this,
		    Image(Art::rotary_adsrData, Art::rotary_adsrWidth, Art::rotary_adsrHeight, GL_BGRA));
    fKnobSustain->setId(5); // TODO: implement proper parameters
    fKnobSustain->setAbsolutePos(715, 256);
    fKnobSustain->setRange(0.0f, 1.0f);
    fKnobSustain->setDefault(1.0f);
    fKnobSustain->setValue(1.0f);
    fKnobSustain->setRotationAngle(300);
    //fKnobSustain->setImageLayerCount(16);
    //fKnobSustain->setStep(1.0f);
    fKnobSustain->setCallback(this);
    
    fKnobRelease = new ImageKnob(this,
		    Image(Art::rotary_adsrData, Art::rotary_adsrWidth, Art::rotary_adsrHeight, GL_BGRA));
    fKnobRelease->setId(6); // TODO: implement proper parameters
    fKnobRelease->setAbsolutePos(792, 256);
    fKnobRelease->setRange(0.0f, 1.0f);
    fKnobRelease->setDefault(0.0f);
    fKnobRelease->setValue(0.0f);
    fKnobRelease->setRotationAngle(300);
    //fKnobRelease->setImageLayerCount(16);
    //fKnobRelease->setStep(1.0f);
    fKnobRelease->setCallback(this);
  // 
  
  
  // switches
  fSwitchFwd = new ImageSwitch(this,
				  Image(Art::switch_offData, Art::switch_offWidth, Art::switch_offHeight, GL_BGR),
				  Image(Art::switch_onData, Art::switch_onWidth, Art::switch_offHeight, GL_BGR));
  fSwitchFwd->setId(1);
  fSwitchFwd->setAbsolutePos(441,242);
  fSwitchFwd->setCallback(this);
  
  fSwitchFloppy = new ImageSwitch(this,
				  Image(Art::floppy_offData, Art::floppy_offWidth, Art::floppy_offHeight, GL_BGR),
				  Image(Art::floppy_onData, Art::floppy_onWidth, Art::floppy_onHeight, GL_BGR));
  fSwitchFloppy->setId(7);
  fSwitchFloppy->setAbsolutePos(92,220);
  fSwitchFloppy->setCallback(this);
  
}

   /**
      A parameter has changed on the plugin side.
      This is called by the host to inform the UI about parameter changes.
    */
void NinjasUI::parameterChanged(uint32_t index, float value)
    {
  switch (index)
    {
    case 1:
        fSwitchFwd->setDown(value > 0.5f);
        break;
    case 2:
       fKnobSlices->setValue(value);
       break;
    case 3:
       fKnobAttack->setValue(value);
       break;
    case 4:
       fKnobDecay->setValue(value);
       break;
    case 5:
       fKnobSustain->setValue(value);
       break;
    case 6:
       fKnobRelease->setValue(value);
       break;
    case 7:
       fSwitchFwd->setDown(value > 0.5f);
       break;
       
    }
 
}

void NinjasUI::uiFileBrowserSelected(const char* filename)
{
	// if a file was selected, tell DSP
	if (filename != nullptr)
	{
	  
	  std::cout << filename << std::endl;
	}
	//	setState("filepath", filename);
}

/* ----------------------------------------------------------------------------------------------------------
 * Widget Callbacks
 *----------------------------------------------------------------------------------------------------------*/
 void NinjasUI::imageSwitchClicked(ImageSwitch* imageSwitch, bool down)
{
    const uint buttonId(imageSwitch->getId());
if (buttonId ==  7)
{
  DGL::Window::FileBrowserOptions opts;
  opts.title = "Load audio file";
  getParentWindow().openFileBrowser(opts);
}
    editParameter(buttonId, true);
    setParameterValue(buttonId, down ? 1.0f : 0.0f);
    editParameter(buttonId, false);
}

void NinjasUI::imageKnobDragStarted(ImageKnob* knob)
{
    editParameter(knob->getId(), true);
}

void NinjasUI::imageKnobDragFinished(ImageKnob* knob)
{
    editParameter(knob->getId(), false);
}

void NinjasUI::imageKnobValueChanged(ImageKnob* knob, float value)
{
    setParameterValue(knob->getId(), value);
}


void NinjasUI::onDisplay()
{
    fImgBackground.draw();
}



/* ------------------------------------------------------------------------------------------------------------
 * UI entry point, called by DPF to create a new UI instance. */

UI* createUI()
{
    return new NinjasUI();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
