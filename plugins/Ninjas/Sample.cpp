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
    // not sure Im gonna use this constructor
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
    
    // load sample memory in samplevector
    fileHandle.read( &samplevector.at(0) , size * channels );
   
    std::cout << "Loaded a file with " << channels << " channels, and a samplerate of " <<
              samplerate << " with " << size << " samples, so its duration is " <<
              size / samplerate << " seconds long." << std::endl;	      
    return 0;
}

void Sample::createSlices(Slice* slices, int n_slices)
{
    float sliceSize = (float)(size*channels) / (float) n_slices;
    std::cout << "sliceSize :" << sliceSize 
    << " x " << n_slices << " n_slices = "
    << n_slices * sliceSize << std::endl;
    
    for (int i = 0 ; i < n_slices; i++)
    {
      slices[i].setSliceStart( (int) i * sliceSize);
      slices[i].setSliceEnd(   ((int) (i+1) * sliceSize) - 1);
      std::cout << "Slice :" << i << " : "
      << slices[i].getSliceStart() << "->"
      << slices[i].getSliceEnd() << std::endl;
    }
}
