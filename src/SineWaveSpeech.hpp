//
//  SineWaveSpeech.hpp
//  SineWaveSpeech
//
//  Created by Maximilian Wagenbach on 07.11.16.
//
//

#ifndef SINEWAVESPEECH_HPP
#define SINEWAVESPEECH_HPP

#include <vector>

#include "MagnitudeSpectrum.hpp"

class SineWaveSpeech
{
public:
    
    SineWaveSpeech(std::size_t FFTSize);
    
    std::vector<float> generateSineWaveSpeech(std::vector<float> samples, std::size_t sampleRate);
    
private:
    
    void generateMagnitudeSpecta(std::vector<float>& samples, std::size_t sampleRate);
    void generateSineWaveSound();
    
    std::size_t                     m_FFTSize;
    MagnitudeSpectrum               m_magnitudeSpectrum;
    std::size_t                     m_sampleRate;
    std::vector<std::vector<float>> m_magnitudes;
    std::vector<float>              m_outputSamples;
    std::vector<float>              m_rms;
};


#endif // SINEWAVESPEECH_HPP
