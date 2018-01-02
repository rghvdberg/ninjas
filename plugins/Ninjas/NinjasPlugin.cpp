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
#include "NinjasPlugin.hpp"
#include <sndfile.hh>
#include <vector>
#include <iostream>
#include <string>
#include <limits>
#include <algorithm>

#include "Sample.h"
#include "Slice.h"
#include "Voice.h"
#include "ADSR.h"
#include "Mixer.h"
#include "aubio.h"

#include "DistrhoPluginInfo.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------


// constructor
NinjasPlugin::NinjasPlugin()
  : Plugin ( paramCount, 0, 1 ) //1 parameter, 0 programs (presets) , 1 states
{
  samplerate = getSampleRate();
}

/* --------------------------------------------------------------------------------------------------------
  * Init
*/

/*
    Initialize the parameters.
    This function will be called once, shortly after the plugin is created.
  */
void NinjasPlugin::initParameter ( uint32_t index, Parameter& parameter )
{
  switch ( index )
    {
    case paramNumberOfSlices:
    {
      parameter.hints      = kParameterIsAutomable|kParameterIsInteger;
      parameter.ranges.def = 1.0f;
      parameter.ranges.min = 1.0f;
      parameter.ranges.max = 16.0f;
      parameter.name   = "Slices";
      parameter.symbol  = "number_of_slices";
      parameter.midiCC = 102;
      break;
    }
    case paramAttack:
    {
      parameter.hints      = kParameterIsAutomable ;
      parameter.ranges.def = 0.05f;
      parameter.ranges.min = 0.05f;
      parameter.ranges.max = 1.0f;
      parameter.name   = "Attack";
      parameter.symbol = "attack";
      parameter.midiCC = 103;
      break;
    }
    case paramDecay:
    {
      parameter.hints      = kParameterIsAutomable ;
      parameter.ranges.def = 0.05f;
      parameter.ranges.min = 0.05f;
      parameter.ranges.max = 1.0f;
      parameter.name   = "Decay";
      parameter.symbol =  "decay";
      parameter.midiCC = 104;
      break;
    }
    case paramSustain:
    {
      parameter.hints      = kParameterIsAutomable ;
      parameter.ranges.def = 1.0f;
      parameter.ranges.min = 0.0f;
      parameter.ranges.max = 1.0f;
      parameter.name = "Sustain";
      parameter.symbol = "sustain";
      // parameter.midiCC = 105;
      break;
    }
    case paramRelease:
    {
      parameter.hints      = kParameterIsAutomable ;
      parameter.ranges.def = 0.05f;
      parameter.ranges.min = 0.05f;
      parameter.ranges.max = 1.0f;
      parameter.name   = "Release";
      parameter.symbol = "release";
      parameter.midiCC = 106;
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
      parameter.midiCC = 107;
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
      parameter.midiCC = 108;
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
      parameter.midiCC = 109;
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
      parameter.midiCC = 110;
      break;
    }
    case paramFloppy:
    {
      parameter.hints = kParameterIsAutomable|kParameterIsBoolean;
      parameter.ranges.def = 0.0f;
      parameter.ranges.min = 0.0f;
      parameter.ranges.max = 1.0f;
      parameter.name   = "Floppy";
      parameter.symbol  = "floppy";
      break;
    }

    case paramSliceMode:
    {
      parameter.hints = kParameterIsAutomable|kParameterIsInteger;
      parameter.ranges.def = 0.0f;
      parameter.ranges.min = 0.0f;
      parameter.ranges.max = 1.0f;
      parameter.name   = "Slice Mode";
      parameter.symbol  = "slicemode";
      parameter.midiCC = 111;
      break;
    }

    }
  if ( index >= paramSwitch01 && index <= paramSwitch16 )
    {
      parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
      parameter.ranges.def = 0.0f;
      parameter.ranges.min = 0.0f;
      parameter.ranges.max = 1.0f;
      parameter.name   = "Switch "+String ( index - paramSwitch01 );
      parameter.symbol  = "switch"+String ( index - paramSwitch01 );
//       parameter.midiCC = index - paramSwitch01 + 33;

    }

}

void NinjasPlugin::initState ( uint32_t index, String& stateKey, String& defaultStateValue )
{
  if ( index == 0 )
    {
      stateKey ="filepath";
      defaultStateValue = "empty";
    }

}

String NinjasPlugin::getState ( const char* ) const
{
  return String ( "filepath" );
}

void NinjasPlugin::setState ( const char* key, const char* value )
{
  if ( strcmp ( key, "filepath" ) == 0 )
    {
      std::string fp = value;
      // load file in sample memory
      if ( !SampleObject.loadSample ( fp, sampleVector, samplerate ) )
        {
          // sample loaded ok, slice it up and set bool
          int64_t size = SampleObject.getSampleSize();
          int channels = SampleObject.getSampleChannels();
          getOnsets ( size, channels, sampleVector, onsets );

          if ( slicemode == 0 )
            createSlicesRaw ( a_slices, slices, size, channels );
          else
            createSlicesOnsets ( onsets, a_slices, slices, size, channels );

          bypass = false;
          setParameterValue ( paramFloppy,1.0f );
        }
      else
        {
          bypass = true;
          setParameterValue ( paramFloppy,0.0f );
        }
    }
}

/* --------------------------------------------------------------------------------------------------------
* Internal data
*/

/**
Get the current value of a parameter.
The host may call this function from any context, including realtime processing.
*/
float NinjasPlugin::getParameterValue ( uint32_t index ) const

{
  float return_Value = 0;

  int ch = currentSlice;

  switch ( index )
    {
    case paramNumberOfSlices:
      return_Value = ( float ) slices;
      break;
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
      if ( a_slices[ch].getSlicePlayMode() == Slice::ONE_SHOT_FWD )
        return_Value = 1;
      else
        return_Value = 0;
      break;
    case paramOneShotRev: // one shot Reverse
      if ( a_slices[ch].getSlicePlayMode() == Slice::ONE_SHOT_REV )
        return_Value = 1;
      else
        return_Value = 0;
      break;
    case paramLoopFwd: // Loop Fwd
      if ( a_slices[ch].getSlicePlayMode() == Slice::LOOP_FWD )
        return_Value = 1;
      else
        return_Value = 0;
      break;
    case paramLoopRev: // Loop Rev
      if ( a_slices[ch].getSlicePlayMode() == Slice::LOOP_REV )
        return_Value = 1;
      else
        return_Value = 0;
      break;
    case paramSliceMode:
      return_Value = slicemode;
      break;
    }
  if ( index >= paramSwitch01 )
    {
      return_Value = p_Grid[index - paramSwitch01];
    }
  return return_Value;

}

/**
Change a parameter value.
The host may call this function from any context, including realtime processing.
When a parameter is marked as automable, you must ensure no non-realtime operations are performed.
@note This function will only be called for parameter inputs.
*/
void NinjasPlugin::setParameterValue ( uint32_t index, float value )
{

  switch ( index )
    {
    case paramNumberOfSlices:
      slices = ( int ) value;
      if ( slicemode == 0 )
        createSlicesRaw ( a_slices,slices,SampleObject.getSampleSize(), SampleObject.getSampleChannels() );
      else
        createSlicesOnsets ( onsets, a_slices,slices,SampleObject.getSampleSize(), SampleObject.getSampleChannels() );
      break;
    case paramAttack:
      p_Attack[currentSlice] = value;
      break;
    case paramDecay:
      p_Decay[currentSlice] = value;
      break;
    case paramSustain:
      p_Sustain[currentSlice] = value;
      break;
    case paramRelease:
      p_Release[currentSlice] = value;
      break;
    case paramOneShotFwd: // one shot forward
      if ( value == 1 )
        a_slices[currentSlice].setSlicePlayMode ( Slice::ONE_SHOT_FWD );
      break;
    case paramOneShotRev: // one shot Reverse
      if ( value == 1 )
        a_slices[currentSlice].setSlicePlayMode ( Slice::ONE_SHOT_REV );
      break;
    case paramLoopFwd: // Loop Fwd
      if ( value == 1 )
        a_slices[currentSlice].setSlicePlayMode ( Slice::LOOP_FWD );
      break;
    case paramLoopRev: // Loop Rev
      if ( value == 1 )
        a_slices[currentSlice].setSlicePlayMode ( Slice::LOOP_REV );
      break;
    case paramSliceMode:
      slicemode = value;
      if ( slicemode == 0 )
        createSlicesRaw ( a_slices,slices,SampleObject.getSampleSize(), SampleObject.getSampleChannels() );
      else
        createSlicesOnsets ( onsets, a_slices,slices,SampleObject.getSampleSize(), SampleObject.getSampleChannels() );
      break;
    } // switch

  if ( index >= paramSwitch01 )
    {
      p_Grid[index - paramSwitch01]=value;
      if ( value == 1 )
        {
          currentSlice = index - paramSwitch01;
        }
    }
} // setParameterValue

/* --------------------------------------------------------------------------------------------------------
* Audio/MIDI Processing */

//------------------- inputs unused , outputs       , size of block we process, pointer to midi data       , number of midi events in current block 
void NinjasPlugin::run ( const float**, float** outputs, uint32_t frames,          const MidiEvent* midiEvents, uint32_t midiEventCount )
{
  float* const outL = outputs[0]; // output ports , stereo
  float* const outR = outputs[1];
  uint32_t framesDone = 0;
  uint32_t curEventIndex = 0; // index for midi event to process
  while ( framesDone < frames )   // we have frames to process !!
    {
      if ( !bypass )
        {
          /* process any ready midi events */
          //   we have midi data to proces, at precisly the current audio frame in the loop
          while ( curEventIndex < midiEventCount && framesDone == midiEvents[curEventIndex].frame )   // the .frame is the offset of the midi event in current block
            {
              if ( midiEvents[curEventIndex].size > MidiEvent::kDataSize ) // not excatly shure what's happening here. this is in both Nekobi and Kars sourcecode
                continue;

              int status = midiEvents[curEventIndex].data[0]; // midi status
              // int channel = status & 0x0F ; // get midi channel
              int message = status & 0xF0 ; // get midi message
              int data1 = midiEvents[curEventIndex].data[1];// note number
              int data2 = midiEvents[curEventIndex].data[2]; //
              // discard notes outside the 16 notes range
              // nn 60 - 74
              // get controller to select active slice
              if ( ! ( ( message == 0x80 || message == 0x90 || message == 0xe0 ) && ( data1 >= 60 && data1 <= 74 ) ) )
                {
                  curEventIndex++;
                  continue;
                }

              switch ( message )
                {
                case 0x80 :   // note off
                {
                  int index = data1-60;
                  bool voice_playing = voices[index].active;
                  if ( voice_playing == false )

                    break; // note wasn't playing anyway .. ignore
                  if ( voice_playing )
                    voices[index].adsr.ADSRstage=ADSR::RELEASE;
                  break;
                }

                case 0x90 :
                {
                  int index = data1 - 60;
                  // new note .. let's activate
                  voices[index].active = true;
                  voices[index].velocity = data2;
                  voices[index].gain = ( float ) data2 / 127.0f;
                  voices[index].adsr.initADSR();
                  voices[index].adsr.setADSR ( p_Attack[index], p_Decay[index] ,p_Sustain[index],p_Release[index], samplerate );
                  // check playmode
                  // if LOOP_REV or ONE_SHOT_REV set playback indici to end of slice
                  if ( a_slices[index].getSlicePlayMode() == Slice::LOOP_REV || a_slices[index].getSlicePlayMode() == Slice::ONE_SHOT_REV )
                    {
                      voices[index].playbackIndex = a_slices[index].getSliceEnd()- a_slices[index].getSliceStart();
                      voices[index].multiplierIndex = ( a_slices[index].getSliceEnd() - a_slices[index].getSliceStart() ) /SampleObject.getSampleChannels();
                    }
                  else     // playmode is forward .. playback indici to start
                    {
                      voices[index].playbackIndex = 0;
                      voices[index].multiplierIndex = 0;
                    }

                  float transpose = ( pitchbend/pitchbend_step ) -12;
                  voices[index].multiplier=pow ( 2.0, transpose / 12.0 );
                  // all set . add to stack
                  //stack.add_Voice(&voices[index]);
                  //
                  break;

                } // case 0x90

                case 0xe0:   // pitchbend
                {
                  pitchbend = ( data2 * 128 ) + data1;
                  break;
                }

                } // switch

              curEventIndex++; // we've processed a midi event,increase index so we know which midi event to process next
            }
          // loop through active voices
          int voice_count {0};
          for ( int i {0} ; i < slices ; i++ )
            {


              if ( voices[i].active )
                {
                  voice_count++;
                  /*get the raw samples from the voice
                  * float* pointer will allow any amount of samples to be pulled in
                  */
                  int sliceStart = a_slices[i].getSliceStart();
                  int sliceEnd = a_slices[i].getSliceEnd();
                  int pos = voices[i].playbackIndex;
                  int channels = SampleObject.getSampleChannels();
                  float* sample = &sampleVector.at ( sliceStart+pos );
                  float sampleL { *sample };
                  float sampleR { * ( sample + ( channels -1 ) ) };
                  // process adsr to get the gain back
                  float adsr_gain = voices[i].adsr.ADSRrun ( &voices[i].active );
                  gain = voices[i].gain * adsr_gain;

                  sampleL = sampleL * gain;
                  sampleR = sampleR * gain;

                  // put samples in mixer

                  mixL.add_Sample ( sampleL );
                  mixR.add_Sample ( sampleR );

                  // increase sample reading position
                  float transpose = ( pitchbend/pitchbend_step ) -12;
                  voices[i].multiplier=pow ( 2.0, transpose / 12.0 );
                  float multiplier = voices[i].multiplier;

                  Slice::slicePlayMode playmode = a_slices[i].getSlicePlayMode();

                  // set multiplier to negative if direction is reverse

                  if ( playmode == Slice::LOOP_REV || playmode == Slice::ONE_SHOT_REV )
                    multiplier=-multiplier;

                  // add the multiplier, when it's negative this should substract

                  voices[i].multiplierIndex += multiplier;
                  int tmp = ( int ) voices[i].multiplierIndex;
                  tmp=tmp * channels;

                  // check bounderies according to playmode: loop or oneshot.

                  switch ( playmode )
                    {
                    case Slice::LOOP_FWD:
                    {
                      if ( sliceStart + tmp >= ( sliceEnd-channels ) )
                        {
                          voices[i].playbackIndex = 0;
                          voices[i].multiplierIndex = 0;
                        }
                      else
                        {
                          voices[i].playbackIndex = tmp;
                        }
                      break;
                    }

                    case Slice::LOOP_REV:
                    {
                      if ( sliceStart + tmp <= sliceStart )
                        {
                          voices[i].playbackIndex = sliceEnd - sliceStart;
                          voices[i].multiplierIndex = ( sliceEnd -sliceStart ) /channels;
                        }
                      else
                        voices[i].playbackIndex = tmp;
                      break;
                    }
                    case Slice::ONE_SHOT_FWD:
                    {
                      if ( sliceStart + tmp >= ( sliceEnd-channels ) )
                        {
                          voices[i].active=false;
                        }
                      else voices[i].playbackIndex = tmp;
                      break;
                    }
                    case Slice::ONE_SHOT_REV:
                    {
                      if ( sliceStart + tmp <= sliceStart )
                        {
                          voices[i].active=false;
                        }
                      else
                        voices[i].playbackIndex = tmp;
                      break;
                    }

                    } //switch
                }// if voices[i].active
            } // end for loop through active voices
          if ( voice_count == 0 )
            {
              mixL.add_Sample ( 0 );
              mixR.add_Sample ( 0 );
            }

          float left = mixL.get_Mix();
          float right = mixR.get_Mix();
          outL[framesDone] = left;
          outR[framesDone] = right;
        }
      else
        {
          // no voices playing
          outL[framesDone] = 0; // output 0 == silence
          outR[framesDone] = 0;
        }
      ++framesDone;

    }
}// run()

// slice funtions

void NinjasPlugin::createSlicesRaw ( Slice* slices, int n_slices, int64_t size, int channels )
{
  long double sliceSize = ( long double ) ( size*channels ) / ( long double ) n_slices;
  for ( int i = 0 ; i < n_slices; i++ )
    {
      slices[i].setSliceStart ( ( int ) i * sliceSize );
      slices[i].setSliceEnd ( ( ( int ) ( i+1 ) * sliceSize ) - 1 );
    }
}

void NinjasPlugin::getOnsets ( int64_t size, int channels, std::vector<float> & sampleVector, std::vector<uint_t> & onsets )
{
  // temp sample vector
  std::vector<float> tmp_sample_vector;
  tmp_sample_vector.resize ( size );

  int hop_size = 256;
  int win_s = 512;
  fvec_t ftable;               // 1. create fvec without allocating it
  intptr_t readptr = 0;
  ftable.length = hop_size;    // 2. set ftable length
  fvec_t * out = new_fvec ( 2 ); // output position
  //double samplerate = getSampleRate();
  if ( channels == 2 ) // create mono sample
    {
      for ( int i=0, j=0 ; i <= size; i++ )
        {
          // sum to mono
          float sum_mono = ( sampleVector[j] + sampleVector[j+1] ) * 0.5f;
          tmp_sample_vector[i]=sum_mono;
          j+=2;
        }
    }
  else
    {
      tmp_sample_vector = sampleVector;
    }

  // create onset object
  aubio_onset_t  * onset = new_aubio_onset ( "complex", win_s, hop_size, samplerate );
  while ( readptr < tmp_sample_vector.size() )
    {
      ftable.data = &tmp_sample_vector[readptr];
      aubio_onset_do ( onset , &ftable, out );
      if ( out->data[0] != 0 )
        {
          onsets.push_back ( aubio_onset_get_last ( onset ) );
        }
      readptr += hop_size;
    }
  del_aubio_onset ( onset );
  // TODO .. del_fvec stuff ?
  // del_fvec ( &ftable );
  // del_fvec ( out );
  aubio_cleanup();
}

void NinjasPlugin::createSlicesOnsets ( std::vector<uint_t> & onsets, Slice* slices, int n_slices, int64_t size, int channels )
{
  if ( size == 0 )
    {
//       std::cout << "no sample loaded" << std::endl;
      return;
    }
  long double sliceSize = ( long double ) size / ( long double ) n_slices;

  // raw slicing
  for ( int i = 0 ; i < n_slices; i++ )
    {
      int64_t start = ( ( int ) i * sliceSize );
      int64_t end = ( ( ( int ) ( i+1 ) * sliceSize ) - 1 );
      // map to nearest onset
      int64_t onset_start = find_nearest ( onsets,start );
      int64_t onset_end = find_nearest ( onsets,end )-1;

      slices[i].setSliceStart ( onset_start * channels );
      slices[i].setSliceEnd ( onset_end * channels );
      // set slice end of last slice to end of sample
      if ( i == n_slices -1 )
        {
          slices[i].setSliceEnd ( end * channels );
        }
    }
}

int64_t NinjasPlugin::find_nearest ( std::vector<uint_t> & haystack, uint_t needle )
{
  auto distance_to_needle_comparator = [&] ( int64_t && a,  int64_t && b )
  {
    return abs ( a - needle ) < abs ( b - needle );
  };

  return *std::min_element ( std::begin ( haystack ), std::end ( haystack ), distance_to_needle_comparator );
}

/* ------------------------------------------------------------------------------------------------------------
* Plugin entry point, called by DPF to create a new plugin instance. */

Plugin* createPlugin()
{
  return new NinjasPlugin();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
