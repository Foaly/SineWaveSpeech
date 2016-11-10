//
//  SineWaveSpeech.cpp
//  SineWaveSpeech
//
//  Created by Maximilian Wagenbach on 07.11.16.
//
//

#include <iostream>
#include <iomanip>

#include "SineWaveSpeech.hpp"
#include "Sinusoid.hpp"

SineWaveSpeech::SineWaveSpeech(std::size_t FFTSize) :
    m_FFTSize(FFTSize),
    m_magnitudeSpectrum(FFTSize),
    m_sampleRate(0)
{
    
}

/**
 * \brief Generates the sine wave speech synthesis of a given input signal.
 *
 * \param samples    A vector of float samples normalized in the range [-1, 1]
 * \param sampleRate The sample rate of the samples
 *
 * \return A vector containing the generated sine wave speech sounds in the range [-1, 1]
 */
std::vector<float> SineWaveSpeech::generateSineWaveSpeech(std::vector<float> samples, std::size_t sampleRate)
{
    m_sampleRate = sampleRate;
    
    // make sure it can always be devided through FFTSize without remainder, if necessary add 0's
    auto remainder = m_FFTSize - (samples.size() % m_FFTSize);
    samples.insert(samples.end(), remainder, 0.f);
    
    // TODO: check this
    m_outputSamples.clear();
    m_outputSamples.resize(samples.size());
    
    generateMagnitudeSpecta(samples, sampleRate);
    
    generateSineWaveSound();
    
    return m_outputSamples;
}


void SineWaveSpeech::generateMagnitudeSpecta(std::vector<float>& samples, std::size_t sampleRate)
{
    // calculate how many times the FFT will be called
    // -1 to avoid out of bounds reading on last iteration because of our 50% sliding window
    const std::size_t numberOfRepeats = samples.size() / (m_FFTSize / 2) - 1;
    
    float maxMagnitude = 0.f;
    float minMagnitude = 0.f;
    
    m_magnitudes.clear();
    m_magnitudes.reserve(numberOfRepeats);
    
    auto chunckBegin = samples.cbegin();
    for (int i = 0; i < numberOfRepeats; i++)
    {
        std::vector<float> sampleChunck(m_FFTSize);
        std::copy(chunckBegin, chunckBegin + m_FFTSize, sampleChunck.begin());
        
        m_magnitudeSpectrum.process(sampleChunck);
        
        m_magnitudes.push_back(m_magnitudeSpectrum.getMagnitudeSpectrum());
        
        // normalize FFT bins
        std::transform(m_magnitudes.back().begin(), m_magnitudes.back().end(), m_magnitudes.back().begin(),
                       [this](float bin)
                       {
                           return bin / (m_FFTSize / 2.f - 1);
                       });
        
        chunckBegin += m_FFTSize / 2;
        
        // find the max FFT bin
        auto minmax = std::minmax_element(m_magnitudes.back().begin(), m_magnitudes.back().end());
        // check if it's bigger than any previous one
        if (*minmax.second > maxMagnitude)
            maxMagnitude = *minmax.second;
        if (*minmax.first < minMagnitude)
            minMagnitude = *minmax.first;
    }
    
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "min: " << minMagnitude << " max: " << maxMagnitude << std::endl;
}


void SineWaveSpeech::generateSineWaveSound()
{
    const float numberOfBins = m_FFTSize / 2.f;
    const float bandwidth = m_sampleRate / 2.f / numberOfBins;
    const float middleFrequency = bandwidth / 2.f;
    
    Sinusoid sinus(440, 1.0, m_sampleRate);
    int x = 0;
    
    for (auto& mag: m_magnitudes)
    {
        // element 0 is the DC offset and we are not interested in that
        mag.erase(mag.begin());
        
        auto highestAmp = std::max_element(mag.begin(), mag.end());
        
        // calculate the frequency
        unsigned int index = std::distance(mag.begin(), highestAmp);
        float frequency = middleFrequency + bandwidth * index;
        float amplitude = *highestAmp;
        
        // threshold
        //if (amplitude > 0.01)
        //{
            sinus.frequency(frequency);
            sinus.amplitude = amplitude;
            
            for (int i = 0; i < 256; i++)
            {
                m_outputSamples[x + i] = sinus.getNextSample();
            }
        //}
        //else
        //{
        //    std::fill_n(m_outputSamples.begin() + x, 256, 0.f);
        //}
        
        x += 256;
    }
}
