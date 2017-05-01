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

#ifndef MAGNITUDESPECTRUM_H
#define MAGNITUDESPECTRUM_H

#include <vector>
#include <complex>

//#include "FFT.hpp"


class MagnitudeSpectrum
{
public:
    
    // define the range of the returned spectrum
    enum class Range
    {
        IncludeDC_IncludeNyquist,   // bins[0 ... (FFTSize/2)]        = FFTSize/2 + 1 bins
        IncludeDC_ExcludeNyquist,   // bins[0 ... (FFTSize/2) - 1]    = FFTSize/2 bins
        ExcludeDC_IncludeNyquist,   // bins[1 ... (FFTSize/2)]        = FFTSize/2 bins
        ExcludeDC_ExcludeNyquist    // bins[1 ... (FFTSize/2) - 1]    = FFTSize/2 -1 bins
    };
    
    
    MagnitudeSpectrum(std::size_t FFTSize, Range spectrumRangeType = Range::ExcludeDC_IncludeNyquist);
    
    void                      process(std::vector<float> sampleChunck);
    const std::vector<float>& getMagnitudeSpectrum() const;
    const std::vector<float>  getLogarithmicMagnitudeSpectrum();
    std::size_t               numberOfBins();
    
    
private:
    //FFT                        m_fft;
    std::size_t                m_FFTSize;
    std::vector<std::complex<float>> m_fftResult;
    Range                      m_spectrumRangeType;
    std::vector<float>         m_magnitudeVector;
    std::vector<float>         m_logarithmicMagnitudeVector;
    std::vector<float>   m_window;
};


#endif // MAGNITUDESPECTRUM_H
