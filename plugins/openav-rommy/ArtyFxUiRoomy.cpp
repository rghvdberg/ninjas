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

#include "DistrhoUI.hpp"

// include full file here for convenience
#include "gui/roomy_widget.cxx"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

class ArtyFxUiRoomy : public UI
{
public:
    ArtyFxUiRoomy()
        : UI(),
          fUI(this)
    {
        // ntk method to add sub-window
        add(fUI.window);
        fUI.window->show();

        // start widgets on their default values
        fUI.graph->size(0.5f);
        fUI.graph->damping(0.5f);
        fUI.graph->wet(0.5f);
        fUI.time->value(0.5f);
        fUI.damping->value(0.5f);
        fUI.dryWet->value(0.5f);

        setSize(fUI.getWidth(), fUI.getHeight());
    }

protected:
   /* --------------------------------------------------------------------------------------------------------
    * DSP/Plugin Callbacks */

   /**
      A parameter has changed on the plugin side.
      This is called by the host to inform the UI about parameter changes.
    */
    void d_parameterChanged(uint32_t index, float value) override
    {
        switch (index)
        {
        case 0:
            fUI.graph->size(value);
            fUI.time->value(value);
            break;
        case 1:
            fUI.graph->damping(value);
            fUI.damping->value(value);
            break;
        case 2:
            fUI.graph->wet(value);
            fUI.dryWet->value(value);
            break;
        }
    }

    // -------------------------------------------------------------------------------------------------------

private:
    RoomyUI fUI;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArtyFxUiRoomy)
};

/* ------------------------------------------------------------------------------------------------------------
 * UI entry point, called by DPF to create a new UI instance. */

UI* createUI()
{
    return new ArtyFxUiRoomy();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
