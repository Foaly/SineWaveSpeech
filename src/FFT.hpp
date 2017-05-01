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

#ifndef FFT_H
#define FFT_H

#include <complex>
#include <fftw3.h>

#include <vector> // replace with boost aligned vector for SIMD

class FFT
{
public:
    FFT(unsigned int FFTLength);

    ~FFT();

    void                        process(const float* input);
    std::vector<std::complex<float>> process2(const float* input);
    const std::vector<float>&   realPart() const;
    const std::vector<float>&   imagPart() const;
    float                       bias()     const;
    float                       nyquist()  const;
    

private:
    //fftwf_plan         m_plan;
    fftwf_plan          m_plan2;
    std::vector<float> m_realPart;
    std::vector<float> m_imagPart;
    float*             m_input;
    std::vector<std::complex<float>> m_complexResult;
};

#endif // FFT_H
