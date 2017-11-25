#include "Sample.h"
#include "sndfile.hh"
#include "samplerate.h"
#include <vector>
#include <iostream>
#include <string>
#include "Slice.h"
// #include "Ninjas.hpp"

// Constructor

Sample::Sample()
{

}

Sample::Sample ( std::string fp, std::vector <float> & samplevector )
{
    // not sure Im gonna use this constructor
}

int Sample::loadSample ( std::string fp, std::vector <float> & samplevector, double host_samplerate )
{
    // create a "Sndfile" handle, it's part of the sndfile library we use to load samples
    SndfileHandle fileHandle ( fp , SFM_READ,  SF_FORMAT_WAV | SF_FORMAT_FLOAT , 2 , 44100 );

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
    samplevector.resize ( size * channels );

    // load sample memory in samplevector
    fileHandle.read ( &samplevector.at ( 0 ) , size * channels );

    std::cout << "Loaded a file with " << channels << " channels, and a samplerate of " <<
              samplerate << " with " << size << " samples, so its duration is " <<
              size / samplerate << " seconds long." << std::endl;
    // check if samplerate != host_samplerate
    std::cout << samplerate << " , " << host_samplerate << " , " << ( samplerate != host_samplerate ) << std::endl;
    if ( samplerate != host_samplerate )
    {
        int err = resample ( samplevector, & samplevector, host_samplerate );

        return err;
    }

    return 0;
}

void Sample::createSlices ( Slice* slices, int n_slices )
{
    long double sliceSize = ( long double ) ( size*channels ) / ( long double ) n_slices;
    
    std::cout << "sliceSize :" << sliceSize
    << " x " << n_slices << " n_slices = "
    << n_slices * sliceSize << std::endl;
    

    for ( int i = 0 ; i < n_slices; i++ )
    {
        slices[i].setSliceStart ( ( int ) i * sliceSize );
        slices[i].setSliceEnd ( ( ( int ) ( i+1 ) * sliceSize ) - 1 );

	std::cout << "Slice :" << i << " : "
        << slices[i].getSliceStart() << "->"
        << slices[i].getSliceEnd() << std::endl;
    }
}

int Sample::resample ( std::vector<float> sample_in, std::vector<float> * sample_out, double host_samplerate )
{
    // copy samplevector in data_in
    // data_out points to samplevector

    SRC_DATA src_data;
    src_data.data_in = & sample_in.at ( 0 );
    src_data.src_ratio = host_samplerate / samplerate;
    src_data.output_frames = size * src_data.src_ratio;

    sample_out->resize ( src_data.output_frames * channels );

    src_data.data_out = & sample_out->at ( 0 );
    src_data.input_frames = size;

    int err = src_simple ( & src_data, SRC_SINC_BEST_QUALITY,channels );
    size = src_data.output_frames_gen;
    std::cout <<  size << std::endl;
    return err;
}

