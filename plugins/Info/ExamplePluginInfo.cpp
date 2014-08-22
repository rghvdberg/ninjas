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
  Plugin to demonstrate parameter outputs using meters.
 */
class ExamplePluginInfo : public Plugin
{
public:
    ExamplePluginInfo()
        : Plugin(kParameterCount, 0, 0)
    {
        std::memset(fParameters, 0, sizeof(float)*kParameterCount);
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
        return "info";
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
        return d_cconst('d', 'N', 'f', 'o');
    }

   /* --------------------------------------------------------------------------------------------------------
    * Init */

   /**
      Initialize the parameter @a index.
      This function will be called once, shortly after the plugin is created.
    */
    void d_initParameter(uint32_t index, Parameter& parameter) override
    {
        parameter.hints      = kParameterIsAutomable|kParameterIsOutput;
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 16777216.0f;

        switch (index)
        {
        case kParameterBufferSize:
            parameter.name   = "BufferSize";
            parameter.symbol = "buffer_size";
            break;
        case kParameterTimePlaying:
            parameter.hints |= kParameterIsBoolean;
            parameter.name   = "TimePlaying";
            parameter.symbol = "time_playing";
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            break;
        case kParameterTimeFrame:
            parameter.name   = "TimeFrame";
            parameter.symbol = "time_frame";
            break;
        case kParameterTimeValidBBT:
            parameter.hints |= kParameterIsBoolean;
            parameter.name   = "TimeValidBBT";
            parameter.symbol = "time_validbbt";
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            break;
        case kParameterTimeBar:
            parameter.name   = "TimeBar";
            parameter.symbol = "time_bar";
            break;
        case kParameterTimeBeat:
            parameter.name   = "TimeBeat";
            parameter.symbol = "time_beat";
            break;
        case kParameterTimeTick:
            parameter.name   = "TimeTick";
            parameter.symbol = "time_tick";
            break;
        case kParameterTimeBarStartTick:
            parameter.name   = "TimeBarStartTick";
            parameter.symbol = "time_barstarttick";
            break;
        case kParameterTimeBeatsPerBar:
            parameter.name   = "TimeBeatsPerBar";
            parameter.symbol = "time_beatsperbar";
            break;
        case kParameterTimeBeatType:
            parameter.name   = "TimeBeatType";
            parameter.symbol = "time_beattype";
            break;
        case kParameterTimeTicksPerBeat:
            parameter.name   = "TimeTicksPerBeat";
            parameter.symbol = "time_ticksperbeat";
            break;
        case kParameterTimeBeatsPerMinute:
            parameter.name   = "TimeBeatsPerMinute";
            parameter.symbol = "time_beatsperminute";
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
        return fParameters[index];

    }

   /**
      Change a parameter value.
    */
    void d_setParameterValue(uint32_t, float) override
    {
        // this is only called for input paramters, which we have none of.
    }

   /* --------------------------------------------------------------------------------------------------------
    * Process */

   /**
      Run/process function for plugins without MIDI input.
    */
    void d_run(const float** inputs, float** outputs, uint32_t frames) override
    {
       /**
          This plugin does nothing, it just demonstrates information usage.
          So here we directly copy inputs over outputs, leaving the audio untouched.
          We need to be careful in case the host re-uses the same buffer for both ins and outs.
        */
        if (outputs[0] != inputs[0])
            std::memcpy(outputs[0], inputs[0], sizeof(float)*frames);

        if (outputs[1] != inputs[1])
            std::memcpy(outputs[1], inputs[1], sizeof(float)*frames);

        // set info
        fParameters[kParameterBufferSize] = d_getBufferSize();

        const TimePosition& timePos(d_getTimePosition());

        fParameters[kParameterTimePlaying]        = timePos.playing ? 1.0f : 0.0f;
        fParameters[kParameterTimeFrame]          = timePos.frame;
        fParameters[kParameterTimeValidBBT]       = timePos.bbt.valid ? 1.0f : 0.0f;
        fParameters[kParameterTimeBar]            = timePos.bbt.bar;
        fParameters[kParameterTimeBeat]           = timePos.bbt.beat;
        fParameters[kParameterTimeTick]           = timePos.bbt.tick;
        fParameters[kParameterTimeBarStartTick]   = timePos.bbt.barStartTick;
        fParameters[kParameterTimeBeatsPerBar]    = timePos.bbt.beatsPerBar;
        fParameters[kParameterTimeBeatType]       = timePos.bbt.beatType;
        fParameters[kParameterTimeTicksPerBeat]   = timePos.bbt.ticksPerBeat;
        fParameters[kParameterTimeBeatsPerMinute] = timePos.bbt.beatsPerMinute;
    }

    // -------------------------------------------------------------------------------------------------------

private:
    // Parameters
    float fParameters[kParameterCount];

   /**
      Set our plugin class as non-copyable and add a leak detector just in case.
    */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExamplePluginInfo)
};

/* ------------------------------------------------------------------------------------------------------------
 * Plugin entry point, called by DPF to create a new plugin instance. */

Plugin* createPlugin()
{
    return new ExamplePluginInfo();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
