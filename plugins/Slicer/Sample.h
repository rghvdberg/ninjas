#ifndef SAMPLE_H
#define SAMPLE_H

#include "sndfile.hh"
#include <vector>
#include <string>



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
        Sample(std::string filepath);

        std::vector<float> getSampleVector()
         {
             return sampleVector;
             };
        int getSampleSize() {return size;};
        int getSampleChannels() {return channels;};
};

#endif // SAMPLE_H
