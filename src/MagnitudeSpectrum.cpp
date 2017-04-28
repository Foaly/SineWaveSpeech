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

#include "MagnitudeSpectrum.hpp"

#include <cmath>
#include <numeric>
#include <algorithm>
#include <cassert>

namespace
{
    const float epsilon = std::numeric_limits<float>::epsilon();
}



MagnitudeSpectrum::MagnitudeSpectrum(std::size_t FFTSize, Range spectrumRangeType) :
    m_fft(FFTSize),
    m_spectrumRangeType(spectrumRangeType),
    m_magnitudeVector(FFTSize / 2, 0.f)
{
    // Bithack to check if FFTSize is a power of 2
    // http://www.graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
    assert((FFTSize && !(FFTSize & (FFTSize - 1))) && "Argument \"FFTSize\" has to be a power of 2!");
    
    switch (m_spectrumRangeType)
    {
        case Range::IncludeDC_IncludeNyquist:
            m_magnitudeVector.resize(FFTSize / 2 + 1);
            break;
        case Range::ExcludeDC_ExcludeNyquist:
            m_magnitudeVector.resize(FFTSize / 2 - 1);
            break;
        default:
            // the other cases are handled by the initializer list
            break;
    }
}


void MagnitudeSpectrum::process(std::vector<float> &sampleChunck)
{
    // TODO: apply window function
    
    // do the FFT
    m_fft.process(sampleChunck.data());
    
    std::size_t startBin = 0;
    if (m_spectrumRangeType == Range::ExcludeDC_IncludeNyquist ||
        m_spectrumRangeType == Range::ExcludeDC_ExcludeNyquist)
    {
        startBin = 1;
    }

    
    std::size_t lastBin = 0;
    if (m_spectrumRangeType == Range::IncludeDC_ExcludeNyquist ||
        m_spectrumRangeType == Range::ExcludeDC_ExcludeNyquist)
    {
        lastBin = 1;
    }

    // calculate the magnitude spectrum
    std::transform(m_fft.realPart().begin() + startBin, m_fft.realPart().end() - lastBin, m_fft.imagPart().begin(), m_magnitudeVector.begin(),
                   [] (float realPart, float imagPart)
                   {
                       return std::sqrt(realPart * realPart + imagPart * imagPart);
                   });
    
}


std::size_t MagnitudeSpectrum::numberOfBins()
{
    return m_magnitudeVector.size();
}


const std::vector<float>& MagnitudeSpectrum::getMagnitudeSpectrum() const
{
    return m_magnitudeVector;
}


const std::vector<float> MagnitudeSpectrum::getLogarithmicMagnitudeSpectrum()
{
    std::vector<float> logarithmicMagnitudeVector(m_magnitudeVector.size(), 0.f);
    std::transform(m_magnitudeVector.begin(), m_magnitudeVector.end(), logarithmicMagnitudeVector.begin(),
                   [] (float magnitude)
                    {
                        return std::log10(magnitude / 100 + epsilon); // log of 0 is undefined
                    });
    
    return logarithmicMagnitudeVector;
}

