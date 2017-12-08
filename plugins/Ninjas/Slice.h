#ifndef SLICE_H
#define SLICE_H
// #include "Sample.h"


class Slice
{

public:
    enum slicePlayMode
    {
        ONE_SHOT_FWD ,
        ONE_SHOT_REV,
        LOOP_FWD,
        LOOP_REV,
    };

    // constructor

    Slice();

    // setter functions

    void setSliceStart (int sl_start)
    {
        sliceStart = sl_start;
    }

    void setSliceEnd (int sl_end)
    {
        sliceEnd = sl_end;
    }

    void setSlicePlayMode (slicePlayMode playMode)
    {
        slicePlay = playMode;
    }

    void setSliceActive ( bool active)
    {
        sliceActive = active;
    }


    // getter functions

    int getSliceStart () const
    {
        return sliceStart;
    }

    int getSliceEnd () const
    {
        return sliceEnd;
    }

    slicePlayMode getSlicePlayMode() const
    {
        return slicePlay;
    }

    bool getSliceActive() const
    {
        return sliceActive;
    }
    //

private:
    unsigned long int sliceStart;
    unsigned long int sliceEnd;
    slicePlayMode slicePlay;
    bool sliceActive;
};

#endif // SLICE_H
