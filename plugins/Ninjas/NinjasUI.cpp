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


#include "DistrhoUI.hpp"
#include "NinjasUI.hpp"
#include "NinjasArtwork.hpp"
#include "Ninjas.hpp"
#include <string>
#include "DistrhoPluginInfo.h"


START_NAMESPACE_DISTRHO

namespace Art = NinjasArtwork;

// -----------------------------------------------------------------------------------------------------------
NinjasUI::NinjasUI()
    : UI ( Art::backgroundWidth, Art::backgroundHeight ),
      fImgBackground ( Art::backgroundData, Art::backgroundWidth, Art::backgroundHeight, GL_BGR )
{
    // init lcd
    waveform.fill(NinjasArtwork::lcd_center);
    // knobs

    fKnobSlices = new ImageKnob ( this,
                                  Image ( Art::rotary_slicesData, Art::rotary_slicesWidth, Art::rotary_slicesHeight, GL_BGR ) );
    fKnobSlices->setId ( paramNumberOfSlices );
    fKnobSlices->setAbsolutePos ( 279, 80 );
    fKnobSlices->setRange ( 1.0f, 16.0f );
    fKnobSlices->setDefault ( 1.0f );
    fKnobSlices->setValue ( 1.0f );
    fKnobSlices->setImageLayerCount ( 16 );
    fKnobSlices->setStep ( 1.0f );
    fKnobSlices->setCallback ( this );

    // ADSR
    fKnobAttack = new ImageKnob ( this,
                                  Image ( Art::rotary_adsrData, Art::rotary_adsrWidth, Art::rotary_adsrHeight, GL_BGRA ) );
    fKnobAttack->setId ( paramAttack );
    fKnobAttack->setAbsolutePos ( 560, 256 );
    fKnobAttack->setRange ( 0.05f, 1.0f );
    fKnobAttack->setDefault ( 0.05f );
    fKnobAttack->setValue ( 0.05f );
    fKnobAttack->setRotationAngle ( 300 );
    fKnobAttack->setCallback ( this );

    fKnobDecay = new ImageKnob ( this,
                                 Image ( Art::rotary_adsrData, Art::rotary_adsrWidth, Art::rotary_adsrHeight, GL_BGRA ) );
    fKnobDecay->setId ( paramDecay );
    fKnobDecay->setAbsolutePos ( 637, 256 );
    fKnobDecay->setRange ( 0.05f, 1.0f );
    fKnobDecay->setDefault ( 0.0f );
    fKnobDecay->setValue ( 0.05f );
    fKnobDecay->setRotationAngle ( 300 );
    fKnobDecay->setCallback ( this );

    fKnobSustain = new ImageKnob ( this,
                                   Image ( Art::rotary_adsrData, Art::rotary_adsrWidth, Art::rotary_adsrHeight, GL_BGRA ) );
    fKnobSustain->setId ( paramSustain );
    fKnobSustain->setAbsolutePos ( 715, 256 );
    fKnobSustain->setRange ( 0.0f, 1.0f );
    fKnobSustain->setDefault ( 1.0f );
    fKnobSustain->setValue ( 1.0f );
    fKnobSustain->setRotationAngle ( 300 );
    fKnobSustain->setCallback ( this );

    fKnobRelease = new ImageKnob ( this,
                                   Image ( Art::rotary_adsrData, Art::rotary_adsrWidth, Art::rotary_adsrHeight, GL_BGRA ) );
    fKnobRelease->setId ( paramRelease );
    fKnobRelease->setAbsolutePos ( 792, 256 );
    fKnobRelease->setRange ( 0.05f, 1.0f );
    fKnobRelease->setDefault ( 0.05f );
    fKnobRelease->setValue ( 0.05f );
    fKnobRelease->setRotationAngle ( 300 );
    fKnobRelease->setCallback ( this );
    //


    // switches

    // play modes
    fSwitchFwd = new ImageSwitch ( this,
                                   Image ( Art::switch_offData, Art::switch_offWidth, Art::switch_offHeight, GL_BGR ),
                                   Image ( Art::switch_onData, Art::switch_onWidth, Art::switch_offHeight, GL_BGR ) );
    fSwitchFwd->setId ( paramOneShotFwd );
    fSwitchFwd->setAbsolutePos ( 441,242 );
    fSwitchFwd->setCallback ( this );

    fSwitchRev = new ImageSwitch ( this,
                                   Image ( Art::switch_offData, Art::switch_offWidth, Art::switch_offHeight, GL_BGR ),
                                   Image ( Art::switch_onData, Art::switch_onWidth, Art::switch_offHeight, GL_BGR ) );
    fSwitchRev->setId ( paramOneShotRev );
    fSwitchRev->setAbsolutePos ( 479,242 );
    fSwitchRev->setCallback ( this );

    fSwitchLoopFwd = new ImageSwitch ( this,
                                       Image ( Art::switch_offData, Art::switch_offWidth, Art::switch_offHeight, GL_BGR ),
                                       Image ( Art::switch_onData, Art::switch_onWidth, Art::switch_offHeight, GL_BGR ) );
    fSwitchLoopFwd->setId ( paramLoopFwd );
    fSwitchLoopFwd->setAbsolutePos ( 441,291 );
    fSwitchLoopFwd->setCallback ( this );

    fSwitchLoopRev = new ImageSwitch ( this,
                                       Image ( Art::switch_offData, Art::switch_offWidth, Art::switch_offHeight, GL_BGR ),
                                       Image ( Art::switch_onData, Art::switch_onWidth, Art::switch_offHeight, GL_BGR ) );
    fSwitchLoopRev->setId ( paramLoopRev );
    fSwitchLoopRev->setAbsolutePos ( 479,291 );
    fSwitchLoopRev->setCallback ( this );

    // floppy

    fSwitchFloppy = new ImageSwitch ( this,
                                      Image ( Art::floppy_offData, Art::floppy_offWidth, Art::floppy_offHeight, GL_BGR ),
                                      Image ( Art::floppy_onData, Art::floppy_onWidth, Art::floppy_onHeight, GL_BGR ) );
    fSwitchFloppy->setId ( paramFloppy );
    fSwitchFloppy->setAbsolutePos ( 92,220 );
    fSwitchFloppy->setCallback ( this );

    // grid .. debug


    for ( int i = paramSwitch01, j =0 ; i <= paramSwitch16; ++i , ++j )
    {

        fGrid[j] = new ImageSwitch ( this,
                                     Image ( Art::switch_offData, Art::switch_offWidth, Art::switch_offHeight, GL_BGR ),
                                     Image ( Art::switch_onData, Art::switch_onWidth, Art::switch_onHeight, GL_BGR ) );
        fGrid[j]->setId ( i );
        // fGrid[j]->setAbsolutePos(981+j*41,89);
        fGrid[j]->setCallback ( this );
    }

    // set coordinates for grid

    // x = 980, y = 90


    for ( int y = 0 ; y < 4 ; ++y )
    {
        for ( int x = 0 ; x < 4 ; ++x )
        {
            int index = y * 4 + x;
            fGrid[index]->setAbsolutePos ( 981+x*41,89+y*46 );
        }
    }


    /*
    fGrid01->setAbsolutePos(980,90);
    fGrid02->setAbsolutePos(1021,90);
    fGrid03->setAbsolutePos(1062,90);
    fGrid04->setAbsolutePos(1103,90);
    fGrid05->setAbsolutePos(980,136);
    fGrid06->setAbsolutePos(1021,136);
    fGrid07->setAbsolutePos(1062,136);
    fGrid08->setAbsolutePos(1103,136);
    fGrid09->setAbsolutePos(980,182);
    fGrid10->setAbsolutePos(1021,182);
    fGrid11->setAbsolutePos(1062,182);
    fGrid12->setAbsolutePos(1103,182);
    fGrid13->setAbsolutePos(980,228);
    fGrid14->setAbsolutePos(1021,228);
    fGrid15->setAbsolutePos(1062,228);
    fGrid16->setAbsolutePos(1103,228);
    */
}

/**
   A parameter has changed on the plugin side.
   This is called by the host to inform the UI about parameter changes.
 */
void NinjasUI::parameterChanged ( uint32_t index, float value )
{
    // std::cout << "parameterChanged(index, value) " << index << " , " << value << std::endl;
    switch ( index )
    {
    case paramNumberOfSlices:
        fKnobSlices->setValue ( value );
        break;
        // Play Modes
    case paramOneShotFwd:
        fSwitchFwd->setDown ( value > 0.5f );
        break;
    case paramOneShotRev:
        fSwitchRev->setDown ( value > 0.5f );
        break;
    case paramLoopFwd:
        fSwitchLoopFwd->setDown ( value > 0.5f );
        break;
    case paramLoopRev:
        fSwitchLoopRev->setDown ( value > 0.5f );
        break;
        // ADSR
    case paramAttack:
        fKnobAttack->setValue ( value );
        break;
    case paramDecay:
        fKnobDecay->setValue ( value );
        break;
    case paramSustain:
        fKnobSustain->setValue ( value );
        break;
    case paramRelease:
        fKnobRelease->setValue ( value );
        break;

        // floppy
    case paramFloppy:
        fSwitchFloppy->setDown ( value > 0.5f );
        break;

        /* selector grid

        case paramSwitch01:
            fGrid[0]->setDown(value > 0.5f);
            break;
        case paramSwitch02:
            fGrid[1]->setDown(value > 0.5f);
            break;
        case paramSwitch03:
            fGrid[2]->setDown(value > 0.5f);
            break;
        case paramSwitch04:
            fGrid[3]->setDown(value > 0.5f);
            break;
        case paramSwitch05:
            fGrid[4]->setDown(value > 0.5f);
            break;
        case paramSwitch06:
            fGrid[5]->setDown(value > 0.5f);
            break;
        case paramSwitch07:
            fGrid[6]->setDown(value > 0.5f);
            break;
        case paramSwitch08:
            fGrid[7]->setDown(value > 0.5f);
            break;
        case paramSwitch09:
            fGrid[8]->setDown(value > 0.5f);
            break;
        case paramSwitch10:
            fGrid[9]->setDown(value > 0.5f);
            break;
        case paramSwitch11:
            fGrid[10]->setDown(value > 0.5f);
            break;
        case paramSwitch12:
            fGrid[11]->setDown(value > 0.5f);
            break;
        case paramSwitch13:
            fGrid[12]->setDown(value > 0.5f);
            break;
        case paramSwitch14:
            fGrid[13]->setDown(value > 0.5f);
            break;
        case paramSwitch15:
            fGrid[14]->setDown(value > 0.5f);
            break;
        case paramSwitch16:
            fGrid[15]->setDown(value > 0.5f);
            break;
        */
    }

}

void NinjasUI::stateChanged ( const char* key, const char* value )
{
    if ( std::strcmp ( key, "filepath" ) == 0 )
    {
        calcWaveform ( String ( value ) );

    }
}


void NinjasUI::uiFileBrowserSelected ( const char* filename )
{
    // if a file was selected, tell DSP
    if ( filename != nullptr )
    {
        setState ( "filepath", filename );
        calcWaveform ( String ( filename ) );

    }
}
/* ----------------------------------------------------------------------------------------------------------
 * Widget Callbacks
 *----------------------------------------------------------------------------------------------------------*/
void NinjasUI::imageSwitchClicked ( ImageSwitch* imageSwitch, bool down )
{
    const uint buttonId ( imageSwitch->getId() );
    if ( buttonId ==  paramFloppy )
    {
        DGL::Window::FileBrowserOptions opts;
        opts.title = "Load audio file";
        getParentWindow().openFileBrowser ( opts );
    }

    switch ( buttonId )
    {
    case paramOneShotFwd:
    {
        editParameter ( paramOneShotFwd, true );
        editParameter ( paramOneShotRev, true );
        editParameter ( paramLoopFwd, true );
        editParameter ( paramLoopRev, true );

        setParameterValue ( paramOneShotFwd, 1.0f );
        setParameterValue ( paramOneShotRev, 0.0f );
        setParameterValue ( paramLoopFwd, 0.0f );
        setParameterValue ( paramLoopRev, 0.0f );

        fSwitchFwd->setDown ( true );
        fSwitchRev->setDown ( false );
        fSwitchLoopFwd->setDown ( false );
        fSwitchLoopRev->setDown ( false );

        editParameter ( paramOneShotFwd, false );
        editParameter ( paramOneShotRev, false );
        editParameter ( paramLoopFwd, false );
        editParameter ( paramLoopRev, false );
        break;
    }
    case paramOneShotRev:
    {
        editParameter ( paramOneShotFwd, true );
        editParameter ( paramOneShotRev, true );
        editParameter ( paramLoopFwd, true );
        editParameter ( paramLoopRev, true );

        setParameterValue ( paramOneShotFwd, 0.0f );
        setParameterValue ( paramOneShotRev, 1.0f );
        setParameterValue ( paramLoopFwd, 0.0f );
        setParameterValue ( paramLoopRev, 0.0f );

        fSwitchFwd->setDown ( false );
        fSwitchRev->setDown ( true );
        fSwitchLoopFwd->setDown ( false );
        fSwitchLoopRev->setDown ( false );

        editParameter ( paramOneShotFwd, false );
        editParameter ( paramOneShotRev, false );
        editParameter ( paramLoopFwd, false );
        editParameter ( paramLoopRev, false );
        break;
    }
    case paramLoopFwd:
    {
        editParameter ( paramOneShotFwd, true );
        editParameter ( paramOneShotRev, true );
        editParameter ( paramLoopFwd, true );
        editParameter ( paramLoopRev, true );

        setParameterValue ( paramOneShotFwd, 0.0f );
        setParameterValue ( paramOneShotRev, 0.0f );
        setParameterValue ( paramLoopFwd, 1.0f );
        setParameterValue ( paramLoopRev, 0.0f );

        fSwitchFwd->setDown ( false );
        fSwitchRev->setDown ( false );
        fSwitchLoopFwd->setDown ( true );
        fSwitchLoopRev->setDown ( false );

        editParameter ( paramOneShotFwd, false );
        editParameter ( paramOneShotRev, false );
        editParameter ( paramLoopFwd, false );
        editParameter ( paramLoopRev, false );
        break;
    }
    case paramLoopRev:
    {
        editParameter ( paramOneShotFwd, true );
        editParameter ( paramOneShotRev, true );
        editParameter ( paramLoopFwd, true );
        editParameter ( paramLoopRev, true );

        setParameterValue ( paramOneShotFwd, 0.0f );
        setParameterValue ( paramOneShotRev, 0.0f );
        setParameterValue ( paramLoopFwd, 0.0f );
        setParameterValue ( paramLoopRev, 1.0f );

        fSwitchFwd->setDown ( false );
        fSwitchRev->setDown ( false );
        fSwitchLoopFwd->setDown ( false );
        fSwitchLoopRev->setDown ( true );

        editParameter ( paramOneShotFwd, false );
        editParameter ( paramOneShotRev, false );
        editParameter ( paramLoopFwd, false );
        editParameter ( paramLoopRev, false );
        break;
    }


    } // switch (buttonId)

    // process the grid

    if ( buttonId >= paramSwitch01 && buttonId <= paramSwitch16 )
    {

        for ( int i = paramSwitch01, j=0; i <= paramSwitch16; ++i,++j )
        {
            // std::cout << ( i==buttonId ) << std::endl;
            editParameter ( i, true );
            setParameterValue ( i, i == buttonId ? 1.0f : 0.0f );
            fGrid[j]->setDown ( i == buttonId );
            editParameter ( i, false );
        }
    }
}

void NinjasUI::imageKnobDragStarted ( ImageKnob* knob )
{
    // std::cout << "knobID = " << knob->getId() << std::endl;
    editParameter ( knob->getId(), true );
}

void NinjasUI::imageKnobDragFinished ( ImageKnob* knob )
{
    // std::cout << "knobID = " << knob->getId() << std::endl;

    editParameter ( knob->getId(), false );
}

void NinjasUI::imageKnobValueChanged ( ImageKnob* knob, float value )
{
    // std::cout << "knobID = " << knob->getId() << " set to " << value << std::endl;

    setParameterValue ( knob->getId(), value );
}


void NinjasUI::onDisplay()
{
    fImgBackground.draw();

    glEnable ( GL_BLEND );
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable ( GL_LINE_SMOOTH );
    glHint ( GL_LINE_SMOOTH_HINT, GL_NICEST );

    glLineWidth ( 2 );
    float r, g, b;
    r = 0x1b/255.f;
    g = 0x43/255.f;
    b = 0x0d/255.f;
    glColor4f ( r, g, b, 1.0f );

    for ( int i =0 ; i < 556 ; i++ )
    {
        glBegin ( GL_LINES );
        glVertex2i ( i+Art::lcd_left,Art::lcd_center );
        glVertex2i ( i+Art::lcd_left,waveform[i] );
	// std::cout << waveform[i] << ",";
        glEnd();

        /*Line<int> lijn ( i+Art::lcd_left,Art::lcd_center,
               i+Art::lcd_left,waveform[i] );
          lijn.draw();
        */
    }
    glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f );
    // std::cout << std::endl;

}

void NinjasUI::calcWaveform ( String fp )
{
    const int LCD_HEIGHT = 107 / 2;
    const int LCD_LENGHT = 556;
    float sum {0};
    //float average {0.5};
    int  iIndex {0};
    float fIndex {0};
    int plotValue {53};
    double samplerate = getSampleRate();

    SndfileHandle fileHandle ( fp , SFM_READ,  SF_FORMAT_WAV | SF_FORMAT_FLOAT , 2 , samplerate );
    int size   = fileHandle.frames();
    if ( size == 0 )
    {
        return;
    }
    float samples_per_pixel = ( float ) size / ( float ) LCD_LENGHT;
    int channels   = fileHandle.channels();
    std::vector<float> tmp ;
    tmp.resize ( size * channels );
    fileHandle.read ( &tmp.at ( 0 ) , size * channels );


    for ( int i = 0; i < LCD_LENGHT ; i++ )
    {
        fIndex = i * samples_per_pixel;
        iIndex = fIndex;
        sum = 0;
        for ( int j = 0, k = 0 ; j < ( int ) samples_per_pixel; j++ )
        {
            k = iIndex +j;
            //TODO mono/stereo
            // sum = sum + tmp.at ( k * channels );
            sum = tmp.at ( k * channels );
            //std::cout << iIndex << " = " << sum << std::endl;

        }
        //average = (float) sum / samples_per_pixel;
        // convert 0.0 - 1.0 to 0 - 107
        plotValue = sum * LCD_HEIGHT + Art::lcd_center;
        //    std::cout << sum << ", " << plotValue << std::endl;
        waveform[i] = plotValue;
	repaint();
    }
    
    //std::cout << std::endl;
    return;

}







/* ------------------------------------------------------------------------------------------------------------
 * UI entry point, called by DPF to create a new UI instance. */

UI* createUI()
{
    return new NinjasUI();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
