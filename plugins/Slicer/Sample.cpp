#include "Sample.h"
#include "sndfile.hh"
#include <vector>
#include <iostream>

// Constructor
Sample::Sample()
{

}

Sample::Sample(std::string fp)
{
    int ok = loadSample(fp);
    std::cout << "sample loading flag: " << ok << std::endl;
};

    int Sample::loadSample(std::string fp)

    {
        // create a "Sndfile" handle, it's part of the sndfile library we
        // use to load samples
          SndfileHandle fileHandle( fp , SFM_READ,  SF_FORMAT_WAV | SF_FORMAT_FLOAT , 2 , 44100);
        // get the number of frames in the sample
          size  = fileHandle.frames();
          if ( size == 0 )
            {
                //file doesn't exist or is of incompatible type, main handles the -1
                std::cout << "Something went wrong" << std::endl;
                return -1;
            }
          // get some more info of the sample
          channels   = fileHandle.channels();

          samplerate = fileHandle.samplerate();
          // we declared sampleVector earlier, now we resize it
          sampleVector.resize(size);
          // this tells sndfile to
          fileHandle.read( &sampleVector.at(0) , size );
          std::cout << "Loaded a file with " << channels << " channels, and a samplerate of " <<
          samplerate << " with " << size << " samples, so its duration is " <<
          size / samplerate << " seconds long." << std::endl;

  return 0;
}
