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



    // getter functions

    int getSliceStart ()
    {
        return sliceStart;
    }

    int getSliceEnd ()
    {
        return sliceEnd;
    }

    slicePlayMode getSlicePlayMode()
    {
        return slicePlay;
    }


private:
    int sliceStart;
    int sliceEnd;
    slicePlayMode slicePlay;
    void initSlice();


};

#endif // SLICE_H
