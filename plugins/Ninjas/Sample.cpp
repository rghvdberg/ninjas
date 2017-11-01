#include "Sample.h"
#include "sndfile.hh"
#include <vector>
#include <iostream>
#include <string>
#include "Slice.h"

// Constructor

Sample::Sample()
{

}



Sample::Sample(std::string fp, std::vector <float> & samplevector)
{
    // 
}

int Sample::loadSample(std::string fp, std::vector <float> & samplevector)
{
    // create a "Sndfile" handle, it's part of the sndfile library we use to load samples
    
    SndfileHandle fileHandle( fp , SFM_READ,  SF_FORMAT_WAV | SF_FORMAT_FLOAT , 2 , 44100);
   
    // get the number of frames in the sample
    size  = fileHandle.frames();
    if ( size == 0 )
    {
        //file doesn't exist or is of incompatible type, main handles the -1
        std::cout << "Something went wrong" << std::endl;
        return 1;
    }
    // get some more info of the sample
    channels   = fileHandle.channels();
    samplerate = fileHandle.samplerate();
    
    // resize vector 
    
    samplevector.resize(size * channels);
    
    
    fileHandle.read( &samplevector.at(0) , size * channels );
   
    std::cout << "Loaded a file with " << channels << " channels, and a samplerate of " <<
              samplerate << " with " << size << " samples, so its duration is " <<
              size / samplerate << " seconds long." << std::endl;
    return 0;
}

void Sample::createSlices(Slice* slices, int n_slices)
{
    int sliceSize = (size*channels) / n_slices;
    //cout << "sliceSize :" << sliceSize << endl;
    for (int i=0, j=0 ; i < size; i+=sliceSize )
    {
        // set start and end
        slices[j].setSliceStart(i);
        slices[j].setSliceStart(i);
        slices[j].setSliceEnd(i+sliceSize-1);
        slices[j].setSliceActive(true);
        //std::cout << "Slice :" << j << " : " << slices[j].getSliceStart() << "->" << slices[j].getSliceEnd() << std::endl;
        ++j;

    }
}
