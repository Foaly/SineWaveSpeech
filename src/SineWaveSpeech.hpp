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

#ifndef SINEWAVESPEECH_HPP
#define SINEWAVESPEECH_HPP

#include <vector>
#include <atomic>
#include <memory>

#include "MagnitudeSpectrum.hpp"
#include "ToneGenerator.hpp"

class SineWaveSpeech
{
public:
    
    SineWaveSpeech(std::size_t FFTSize, bool zeroPadAtEnd);
    
    std::vector<float> generateSineWaveSpeech(std::vector<float> samples, std::size_t sampleRate);
    void nextToneGenerator();
    
private:
    
    void generateMagnitudeSpecta(std::vector<float>& samples, std::size_t sampleRate);
    void generateSineWaveSound();
    
    std::size_t                                    m_FFTSize;
    MagnitudeSpectrum                              m_magnitudeSpectrum;
    std::size_t                                    m_sampleRate;
    std::vector<std::vector<float>>                m_magnitudes;
    std::vector<float>                             m_outputSamples;
    std::vector<float>                             m_rms;
    std::atomic<unsigned int>                      m_currentToneGenerator;
    std::vector<std::unique_ptr<ToneGenerator>>    m_toneGenertors;
    bool                                           m_zeroPadAtEnd;
};


#endif // SINEWAVESPEECH_HPP
