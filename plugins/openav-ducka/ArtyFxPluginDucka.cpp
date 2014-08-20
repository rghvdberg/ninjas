/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2014 Filipe Coelho <falktx@falktx.com>
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

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

class ArtyFxPluginDucka : public Plugin
{
public:
    ArtyFxPluginDucka()
        : Plugin(4, 0, 0), // 4 parameters, 0 programs, 0 states
          // parameters init
          controlThreshold(0.25f),
          controlReduction(1.0f),
          controlReleaseTime(0.5f),
          controlSidechainAmp(0.0f),
          // filter state init
          w(10.0f / (d_getSampleRate() * 0.02)),
          a(0.07f),
          b(1.0f / (1.0f - a)),
          g1(0.0f),
          g2(0.0f),
          samplerate( d_getSampleRate() ),
          peakFrameCounter(0),
          peakCountDuration( samplerate / 4 ),
          currentTarget(0)
    {
    }

protected:
   /* --------------------------------------------------------------------------------------------------------
    * Information */

   /**
      Get the plugin label.
      A plugin label follows the same rules as Parameter::symbol, with the exception that it can start with numbers.
    */
    const char* d_getLabel() const override
    {
        return "ducka";
    }

   /**
      Get the plugin author/maker.
    */
    const char* d_getMaker() const override
    {
        return "OpenAV Productions";
    }

   /**
      Get the plugin license name (a single line of text).
    */
    const char* d_getLicense() const override
    {
        return "ISC";
    }

   /**
      Get the plugin version, in hexadecimal.
      TODO format to be defined
    */
    uint32_t d_getVersion() const override
    {
        return 0x1000;
    }

   /**
      Get the plugin unique Id.
      This value is used by LADSPA, DSSI and VST plugin formats.
    */
    int64_t d_getUniqueId() const override
    {
        return d_cconst('O', 'V', 'd', 'k');
    }

   /* --------------------------------------------------------------------------------------------------------
    * Init */

   /**
      Initialize the parameter @a index.
      This function will be called once, shortly after the plugin is created.
    */
    void d_initParameter(uint32_t index, Parameter& parameter) override
    {
        parameter.hints      = kParameterIsAutomable;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;

        switch (index)
        {
        case 0:
            parameter.name   = "Threshold";
            parameter.symbol = "threshold";
            parameter.ranges.def = 0.25f;
            break;
        case 1:
            parameter.name   = "Reduction";
            parameter.symbol = "reduction";
            parameter.ranges.def = 1.0f;
            break;
        case 2:
            parameter.name   = "Release Time";
            parameter.symbol = "release_time";
            parameter.ranges.def = 0.5f;
            break;
        case 3:
            parameter.hints |= kParameterIsOutput;
            parameter.name   = "Sidechain Amplitude";
            parameter.symbol = "sidechain_amp";
            parameter.ranges.def = 0.0f;
            break;
        }
    }

   /* --------------------------------------------------------------------------------------------------------
    * Internal data */

   /**
      Get the current value of a parameter.
    */
    float d_getParameterValue(uint32_t index) const override
    {
        switch (index)
        {
        case 0: return controlThreshold;
        case 1: return controlReduction;
        case 2: return controlReleaseTime;
        case 3: return controlSidechainAmp;
        }

        return 0.0f;
    }

   /**
      Change a parameter value.
    */
    void d_setParameterValue(uint32_t index, float value) override
    {
        switch (index)
        {
        case 0:
            controlThreshold = value;
            break;
        case 1:
            controlReduction = value;
            break;
        case 2:
            controlReleaseTime = value;
            break;
        }

        // outputs can't be changed by the host
    }

   /* --------------------------------------------------------------------------------------------------------
    * Process */

   /**
      Run/process function for plugins without MIDI input.
    */
    void d_run(const float** inputs, float** outputs, uint32_t frames) override
    {
        const float* inL  = inputs[0];
        const float* inR  = inputs[1];
        const float* side = inputs[2];
        float* outL = outputs[0];
        float* outR = outputs[1];

        const TimePosition& timePos(d_getTimePosition());

        if (timePos.bbt.valid)
            peakCountDuration = samplerate / ( timePos.bbt.beatsPerMinute / 60.0);
        else
            peakCountDuration = samplerate / 2.0f;

        /// analyse sidechain input for peak
        float sum = 0.f;
        for( uint32_t i = 0; i < frames; i++ )
        {
          if ( *side > 0.000001 )
            sum += *side++;
          else
            sum += -*side++;
        }

        currentTarget = 0.f;

        /// check for peak level (offset to avoid "always on" peak)
        if ( sum / frames > controlThreshold + 0.05 )
        {
          peakFrameCounter = peakCountDuration * controlReleaseTime;
          currentTarget = 1.f - controlReduction;
        }
        else if ( peakFrameCounter < 0 )
        {
          currentTarget = 1.f;
        }
        else
        {
          currentTarget = 1.f - controlReduction;
        }

        if ( currentTarget < 0.f )
            currentTarget = 0.f;

        peakFrameCounter -= frames;

        for( uint32_t i = 0; i < frames; i++ )
        {
          /// smoothing algo is a lowpass, to de-zip the fades
          /// x^^4 approximates linear volume increase for human ears
          g1 += w * ( std::pow ( currentTarget, 4.f ) - g1 - a * g2 - 1e-20f);
          g2 += w * (b * g1 - g2 + 1e-20f);
          float gain = g2;

          *outL++ = *inL++ * gain;
          *outR++ = *inR++ * gain;
        }

        /// update output value
        controlSidechainAmp = 1-currentTarget;
    }

   /* --------------------------------------------------------------------------------------------------------
    * Callbacks */

   /**
      Optional callback to inform the plugin about a sample rate change.
      This function will only be called when the plugin is deactivated.
    */
    void d_sampleRateChanged(double newSampleRate) override
    {
        // reinit filter
        w = 10.0f / (newSampleRate * 0.02);
        a = 0.07f;
        b = 1.0f / (1.0f - a);
        g1 = 0.0f;
        g2 = 0.0f;
        samplerate        = newSampleRate;
        peakFrameCounter  = 0;
        peakCountDuration = samplerate / 4;
        currentTarget     = 0;
    }

    // -------------------------------------------------------------------------------------------------------

private:
    /// control signals
    float controlThreshold;
    float controlReduction;
    float controlReleaseTime;
    float controlSidechainAmp;

    /// filter state
    float w, a, b, g1, g2;

    /// last peak history
    long samplerate;
    bool nowIsAPeak;
    long peakFrameCounter;

    /// nframes available for countdown
    long peakCountDuration;

    /// control output
    float currentTarget;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArtyFxPluginDucka)
};

/* ------------------------------------------------------------------------------------------------------------
 * Plugin entry point, called by DPF to create a new plugin instance. */

Plugin* createPlugin()
{
    return new ArtyFxPluginDucka();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
