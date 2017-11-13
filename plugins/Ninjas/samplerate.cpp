#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <samplerate.h>

using namespace std;

SRC_DATA src_data;
vector<float> sample_in, sample_out;


int main ( int argc, char **argv )
{
    SndfileHandle fileHandle ( "/home/rob/git/ninjas/plugins/Ninjas/sample.ogg" , SFM_READ,  SF_FORMAT_WAV | SF_FORMAT_FLOAT , 1 , 44100 );

    // get the number of frames in the sample
    int size  = fileHandle.frames();

    if ( size == 0 )
    {
        // file doesn't exist or is of incompatible type, main handles the -1
        cout << "something went wrong" << endl;
        return -1;
    }

    // get some more info of the sample
    int channels   = fileHandle.channels();
    int samplerate = fileHandle.samplerate();

    // we declared sampleVector earlier, now we resize it
    sample_in.resize ( size * channels );


    // this tells sndfile to
    fileHandle.read ( &sample_in.at ( 0 ) , size );

    std::cout << "Loaded a file with " << channels << " channels, and a samplerate of " <<
              samplerate << " with " << size << " samples, so its duration is " <<
              size / samplerate << " seconds long." << std::endl;
    
    src_data.data_in = & sample_in.at( 0 );
    src_data.src_ratio = ( double ) 44100 / ( double ) samplerate;
    src_data.output_frames = size * src_data.src_ratio;

    sample_out.resize(src_data.output_frames * channels);
    
    src_data.data_out = & sample_out.at( 0 );
    src_data.input_frames = size;
   
    int err = src_simple(& src_data, SRC_SINC_BEST_QUALITY,channels);
    cout << err << endl;
    return 0;
}
