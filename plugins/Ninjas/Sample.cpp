#include "Sample.h"
#include "sndfile.hh"
#include <vector>
#include <iostream>
#include <string>
#include "Slice.h"

// Constructor

Sample::Sample(std::string fp)
{
    int ok = loadSample(fp);
    std::cout << "sample loading flag: " << ok << std::endl;
}

    int Sample::loadSample(std::string fp)

    {
        // create a "Sndfile" handle, it's part of the sndfile library we
        // use to load samples
        std::cout << "file path =" << fp << std::endl;
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
          // we declared sampleVector earlier, now we resize it
          sampleVector.resize(size);
          // this tells sndfile to
          fileHandle.read( &sampleVector.at(0) , size );
          std::cout << "Loaded a file with " << channels << " channels, and a samplerate of " <<
          samplerate << " with " << size << " samples, so its duration is " <<
          size / samplerate << " seconds long." << std::endl;

  return 0;
}
    void Sample::createSlices(std::vector<Slice>* slices, int n_slices)
    {
       int sliceSize = size / n_slices;
        //cout << "sliceSize :" << sliceSize << endl;
        for (int i=0, j=0 ; i < size; i+=sliceSize )
        {
            slices->push_back(Slice());
            // set start and end
            slices->at(j).setSliceStart(i);
            slices->at(j).setSliceStart( i );
            slices->at(j).setSliceEnd(i+sliceSize-1);
        //	cout << j << " : "  << i << " -> " << i+sliceSize-1 << endl;
        //	cout << "Slice :" << j << " : " << 	v_slices[j].getSliceStart() << "->" << v_slices[j].getSliceEnd() << endl;
            ++j;

        }
    }
