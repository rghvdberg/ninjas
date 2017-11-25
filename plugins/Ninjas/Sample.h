#ifndef SAMPLE_H
#define SAMPLE_H

#include "sndfile.hh"
#include "samplerate.h"
#include <vector>
#include <string>
#include "Slice.h"
#include "DistrhoPlugin.hpp"



class Sample
{
private:
    std::string filepath;
    // std::vector <float> sampleVector;
    double samplerate;
    // int length;
    int channels;
    sf_count_t size; // in frames !!!
    
public:
    Sample();

    Sample(std::string filepath, std::vector <float> & samplevector);
    
    uint32_t getSampleSize() const
    {
        return size;
    }

    int getSampleChannels() const
    {
        return channels;
    }
    
  
    void createSlices( Slice* slices, int number);

    int loadSample(std::string fp, std::vector <float> & samplevector, double host_samplerate);
    
    int resample (std::vector <float> sample_in, std::vector <float> * sample_out, double host_samplerate);
    
    };

#endif // SAMPLE_H
