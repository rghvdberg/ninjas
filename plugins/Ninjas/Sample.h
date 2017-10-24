#ifndef SAMPLE_H
#define SAMPLE_H

#include "sndfile.hh"
#include <vector>
#include <string>
#include "Slice.h"



class Sample
{
    private:
    std::string filepath;
    std::vector <float> sampleVector;
    int samplerate;
    int length;
    int channels;
    int size; // in frames !!!

    public:
        Sample();

        Sample(std::string filepath);

        std::vector<float> getSampleVector()
         {
             return sampleVector;
	 }

        int getSampleSize() const
        {
            return size;
        }

        int getSampleChannels() const
        {
            return channels;
        }
        
        void createSlices( Slice* slices, int number);
	
	int loadSample(std::string fp);
};

#endif // SAMPLE_H
