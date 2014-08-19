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

#include "dsp/dsp_reverb.hxx"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

class ArtyFxPluginRoomy : public Plugin
{
public:
    ArtyFxPluginRoomy()
        : Plugin(3, 0, 0), // 3 parameters, 0 programs, 0 states
          dspReverb(d_getSampleRate())
    {
       // Initialize all our parameters to their defaults.
        controlTime    = 0.5f;
        controlDamping = 0.5f;
        controlDryWet  = 0.5f;
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
        return "roomy";
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
        return d_cconst('O', 'V', 'r', 'm');
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
        parameter.ranges.def = 0.5f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;

        switch (index)
        {
        case 0:
            parameter.name   = "Time";
            parameter.symbol = "time";
            break;
        case 1:
            parameter.name   = "Damping";
            parameter.symbol = "damping";
            break;
        case 2:
            parameter.name   = "Dry Wet Mix";
            parameter.symbol = "dry_wet";
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
        case 0: return controlTime;
        case 1: return controlDamping;
        case 2: return controlDryWet;
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
            controlTime = value;
            break;
        case 1:
            controlDamping = value;
            break;
        case 2:
            controlDryWet = value;
            break;
        }
    }

   /* --------------------------------------------------------------------------------------------------------
    * Process */

   /**
      Run/process function for plugins without MIDI input.
    */
    void d_run(const float** inputs, float** outputs, uint32_t frames) override
    {
        dspReverb.rt60    ( controlTime    );
        dspReverb.damping ( controlDamping );
        dspReverb.dryWet  ( controlDryWet  );

        dspReverb.process(frames, inputs, outputs);
    }

   /* --------------------------------------------------------------------------------------------------------
    * Callbacks */

   /**
      Optional callback to inform the plugin about a sample rate change.
      This function will only be called when the plugin is deactivated.
    */
    void d_sampleRateChanged(double /*newSampleRate*/) override
    {
        // TODO - reinit dspReverb
    }

    // -------------------------------------------------------------------------------------------------------

private:
    // our parameters
    float controlTime;
    float controlDamping;
    float controlDryWet;

    // the reverb
    Reverb dspReverb;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArtyFxPluginRoomy)
};

/* ------------------------------------------------------------------------------------------------------------
 * Plugin entry point, called by DPF to create a new plugin instance. */

Plugin* createPlugin()
{
    return new ArtyFxPluginRoomy();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
