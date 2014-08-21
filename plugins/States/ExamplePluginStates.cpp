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
  Simple plugin to demonstrate state usage (including UI).
  The plugin will be treated as an effect, but it will not change the host audio.
 */
class ExamplePluginStates : public Plugin
{
public:
    ExamplePluginStates()
        : Plugin(0, 0, 9) // 0 parameters, 0 programs, 9 states
    {
       // nothing here
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
        return "states";
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
        return d_cconst('d', 'S', 't', 's');
    }

   /* --------------------------------------------------------------------------------------------------------
    * Parameters */

   /**
      This plugin has no parameters, so we can safely ignore some functions.
    */
    void  d_initParameter(uint32_t, Parameter&) override {}
    void  d_setParameterValue(uint32_t, float)  override {}
    float d_getParameterValue(uint32_t) const   override { return 0.0f; }

   /* --------------------------------------------------------------------------------------------------------
    * State */

   /**
      Set the state key and default value of @a index.
      This function will be called once, shortly after the plugin is created.
    */
    void d_initState(uint32_t index, d_string& stateKey, d_string& defaultStateValue) override
    {
        switch (index)
        {
        case 0:
            stateKey = "top-left";
            break;
        case 1:
            stateKey = "top-center";
            break;
        case 2:
            stateKey = "top-right";
            break;
        case 3:
            stateKey = "middle-left";
            break;
        case 4:
            stateKey = "middle-center";
            break;
        case 5:
            stateKey = "middle-right";
            break;
        case 6:
            stateKey = "bottom-left";
            break;
        case 7:
            stateKey = "bottom-center";
            break;
        case 8:
            stateKey = "bottom-right";
            break;
        }

        defaultStateValue = "false";
    }

   /**
      Change an internal state.
    */
    void d_setState(const char*, const char*) override
    {
        // there is no plugin side state here.
        // states on this plugin will only change the UI grid, so we do nothing here
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
    // nothing here

   /**
      Set our plugin class as non-copyable and add a leak detector just in case.
    */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExamplePluginStates)
};

/* ------------------------------------------------------------------------------------------------------------
 * Plugin entry point, called by DPF to create a new plugin instance. */

Plugin* createPlugin()
{
    return new ExamplePluginStates();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
