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
    int loadSample(std::string fp);
    int size;

    public:
        Sample();

        Sample(std::string filepath);

        std::vector<float> getSampleVector()
         {
             return sampleVector;
             }

        int getSampleSize()
        {
            return size;
        }

        int getSampleChannels()
        {
            return channels;
        }
        void createSlices( Slice* slices, int number);
};

#endif // SAMPLE_H
