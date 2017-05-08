////////////////////////////////////////////////////////////
//
// SineWaveSpeech - A sine wave speech synthesizer
// Copyright (C) 2017  Maximilian Wagenbach
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////
//
//  Created by Maximilian Wagenbach on 07.11.16.

#include <iostream>
#include <iomanip>
#include <numeric>
#include <algorithm>

#include "SineWaveSpeech.hpp"

SineWaveSpeech::SineWaveSpeech(std::size_t FFTSize, bool zeroPadAtEnd) :
    m_FFTSize(FFTSize),
    m_magnitudeSpectrum(FFTSize, MagnitudeSpectrum::Range::ExcludeDC_IncludeNyquist),
    m_sampleRate(0),
    m_sinus(440, 0.0, m_sampleRate),
    m_sawtooth(440, 0.0, m_sampleRate),
    m_triangle(440, 0.0, m_sampleRate),
    m_toneGenerator(0),
    m_zeroPadAtEnd(zeroPadAtEnd)
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
    m_sinus.sampleRate = m_sampleRate;
    m_sawtooth.sampleRate = m_sampleRate;
    m_triangle.sampleRate = m_sampleRate;
    
    if (m_zeroPadAtEnd)
    {
        // make sure it can always be devided through FFTSize without remainder, if necessary add 0's
        auto remainder = m_FFTSize - (samples.size() % m_FFTSize);
        samples.insert(samples.end(), remainder, 0.f);
    }
    
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
    
    m_magnitudes.clear();
    m_magnitudes.reserve(numberOfRepeats);
    
    m_rms.clear();
    m_rms.reserve(numberOfRepeats);
    
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
        
        // calculate the RMS of the sample block
        double meansquare = std::sqrt( ( std::inner_product( sampleChunck.begin(), sampleChunck.end(), sampleChunck.begin(), 0.0 ) ) / static_cast<double>( sampleChunck.size() ) );
        
        m_rms.push_back(meansquare);
    }
}


void SineWaveSpeech::nextToneGenerator()
{
    ++m_toneGenerator;
    m_toneGenerator = m_toneGenerator % 3;
}


void SineWaveSpeech::generateSineWaveSound()
{
    const float numberOfBins = m_magnitudeSpectrum.numberOfBins();
    const float bandwidth = m_sampleRate / 2.f / numberOfBins;
    const float middleFrequency = bandwidth / 2.f;
    
    int x = 0;
    std::size_t currentBlock = 0;
    
    for (auto& mag: m_magnitudes)
    {
        // find the highest amplitude
        auto highestAmp = std::max_element(mag.begin(), mag.end());
        
        // calculate the frequency
        unsigned int index = std::distance(mag.begin(), highestAmp);
        float frequency = middleFrequency + bandwidth * index;
        //float amplitude = *highestAmp;
        
        if (frequency > 3000)
        {
            std::fill_n(m_outputSamples.begin() + x, 256, 0.f);
        }
        else
        {
            float amplitude = std::min(m_rms[currentBlock] * std::sqrt(2.f), 1.f); // clamp to 1, because sometimes
            //std::cout << amplitude << std::endl;
            
            int interpolationSteps = 50;
            double oldFrequency = m_sinus.frequency();
            double oldAmplitude = m_sinus.amplitude();

            switch (m_toneGenerator)
            {
                case 1:
                    oldFrequency = m_sawtooth.frequency();
                    oldAmplitude = m_sawtooth.amplitude();
                    break;
                case 2:
                    oldFrequency = m_triangle.frequency();
                    oldAmplitude = m_triangle.amplitude();
                    break;
                default:
                    break;
            }

            double frequencyStep = (frequency - oldFrequency) / interpolationSteps;
            double amplitudeStep = (amplitude - oldAmplitude) / interpolationSteps;
        
            for (int i = 0; i < 256; i++)
            {
                switch (m_toneGenerator)
                {
                    case 0:
                        if (i < interpolationSteps)
                        {
                            m_sinus.frequency(m_sinus.frequency() + frequencyStep);
                            m_sinus.amplitude(m_sinus.amplitude() + amplitudeStep);
                        }
                        else if (i == interpolationSteps)
                        {
                            m_sinus.frequency(frequency);
                            m_sinus.amplitude(amplitude);
                        }

                        m_outputSamples[x + i] = m_sinus.getNextSample();
                        break;
                    case 1:
                        if (i < interpolationSteps)
                        {
                            m_sawtooth.frequency(m_sawtooth.frequency() + frequencyStep);
                            m_sawtooth.amplitude(m_sawtooth.amplitude() + amplitudeStep);
                        }
                        else if (i == interpolationSteps)
                        {
                            m_sawtooth.frequency(frequency);
                            m_sawtooth.amplitude(amplitude);
                        }

                        m_outputSamples[x + i] = m_sawtooth.getNextSample();
                        break;

                    case 2:
                        if (i < interpolationSteps)
                        {
                            m_triangle.frequency(m_triangle.frequency() + frequencyStep);
                            m_triangle.amplitude(m_triangle.amplitude() + amplitudeStep);
                        }
                        else if (i == interpolationSteps)
                        {
                            m_triangle.frequency(frequency);
                            m_triangle.amplitude(amplitude);
                        }

                        m_outputSamples[x + i] = m_triangle.getNextSample();
                        break;

                    default:
                        break;
                }

            }
        }
        
        x += m_FFTSize / 2;
        currentBlock++;
    }
}
