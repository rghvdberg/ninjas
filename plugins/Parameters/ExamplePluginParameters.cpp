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

/**
  Simple plugin to demonstrate parameter usage (including UI).
  The plugin will be treated as an effect, but it will not change the host audio.
 */
class ExamplePluginParameters : public Plugin
{
public:
    ExamplePluginParameters()
        : Plugin(9, 0, 0) // 9 parameters, 0 programs, 0 states
    {
       /**
          Initialize all our parameters to their defaults.
          In this example all parameters have 0 as default, so we can simply zero them.
        */
        std::memset(fParamGrid, 0, sizeof(float)*9);
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
        return "parameters";
    }

   /**
      Get the plugin author/maker.
    */
    const char* d_getMaker() const override
    {
        return "DISTRHO";
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
        return d_cconst('d', 'P', 'r', 'm');
    }

   /* --------------------------------------------------------------------------------------------------------
    * Init */

   /**
      Initialize the parameter @a index.
      This function will be called once, shortly after the plugin is created.
    */
    void d_initParameter(uint32_t index, Parameter& parameter) override
    {
       /**
          All parameters in this plugin are similar except for name.
          As such, we initialize the common details first, then set the unique name later.
        */

       /**
          Changing parameters does not cause any realtime-unsafe operations, so we can mark them as automable.
          Also set as boolean because they work are on/off switches.
        */
        parameter.hints = kParameterIsAutomable|kParameterIsBoolean;

       /**
          Minimum 0 (off), maximum 1 (on).
          Default is off.
        */
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;

       /**
          Set the (unique) parameter name.
          @see fParamGrid
        */
        switch (index)
        {
        case 0:
            parameter.name = "top-left";
            break;
        case 1:
            parameter.name = "top-center";
            break;
        case 2:
            parameter.name = "top-right";
            break;
        case 3:
            parameter.name = "middle-left";
            break;
        case 4:
            parameter.name = "middle-center";
            break;
        case 5:
            parameter.name = "middle-right";
            break;
        case 6:
            parameter.name = "bottom-left";
            break;
        case 7:
            parameter.name = "bottom-center";
            break;
        case 8:
            parameter.name = "bottom-right";
            break;
        }

       /**
          Our parameter names are unique valid symbols, so we can just re-use them.
        */
        parameter.symbol = parameter.name;
    }

   /* --------------------------------------------------------------------------------------------------------
    * Internal data */

   /**
      Get the current value of a parameter.
    */
    float d_getParameterValue(uint32_t index) const override
    {
        return fParamGrid[index];
    }

   /**
      Change a parameter value.
    */
    void d_setParameterValue(uint32_t index, float value) override
    {
        fParamGrid[index] = value;
    }

   /* --------------------------------------------------------------------------------------------------------
    * Process */

   /**
      Run/process function for plugins without MIDI input.
    */
    void d_run(const float** inputs, float** outputs, uint32_t frames) override
    {
       /**
          This plugin does nothing, it just demonstrates parameter usage.
          So here we directly copy inputs over outputs, leaving the audio untouched.
          We need to be careful in case the host re-uses the same buffer for both ins and outs.
        */
        if (outputs[0] != inputs[0])
            std::memcpy(outputs[0], inputs[0], sizeof(float)*frames);

        if (outputs[1] != inputs[1])
            std::memcpy(outputs[1], inputs[1], sizeof(float)*frames);
    }

    // -------------------------------------------------------------------------------------------------------

private:
   /**
      Our parameters are used to display a 3x3 grid like this:
       0 1 2
       3 4 5
       6 7 8

      The index matches its grid position.
    */
    float fParamGrid[9];

   /**
      Set our plugin class as non-copyable and add a leak detector just in case.
    */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExamplePluginParameters)
};

/* ------------------------------------------------------------------------------------------------------------
 * Plugin entry point, called by DPF to create a new plugin instance. */

Plugin* createPlugin()
{
    return new ExamplePluginParameters();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
