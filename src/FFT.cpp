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

#include "FFT.hpp"

#include <mutex>


namespace
{
    std::mutex s_fftwMutex;
}


FFT::FFT(unsigned int FFTLength) :
    m_realPart(FFTLength / 2 + 1, 0.f),// FFTW includes the Nyquist, thus returning N/2+1
    m_imagPart(FFTLength / 2 + 1, 0.f),
    m_complexResult(FFTLength / 2 + 1, 0.0)
{
    std::vector<float> tempInput(FFTLength);
    std::vector<float> tempReal(FFTLength);
    std::vector<float> tempImag(FFTLength);

    //fftwf_iodim dim;
    //dim.n  = FFTLength;
    //dim.is = 1;
    //dim.os = 1;

    std::lock_guard<std::mutex> lock(s_fftwMutex);
    //m_plan = fftwf_plan_guru_split_dft_r2c(1, &dim, 0, nullptr, tempInput.data(), tempReal.data(), tempImag.data(), FFTW_ESTIMATE | FFTW_PRESERVE_INPUT);
    //fftwf_print_plan(m_plan);

    m_input = nullptr;
    m_plan2 = fftwf_plan_dft_r2c_1d(FFTLength, m_input, reinterpret_cast<fftwf_complex*>(m_complexResult.data()), FFTW_ESTIMATE);
}


FFT::~FFT()
{
    std::lock_guard<std::mutex> lock(s_fftwMutex);
    //fftwf_destroy_plan(m_plan);
    fftwf_destroy_plan(m_plan2);
}


void FFT::process(const float *input)
{
    float* nonConstInput = const_cast<float*>(input);   // fftw does not take const input even though the data not be manipulated!
    //fftwf_execute_split_dft_r2c(m_plan, nonConstInput, m_realPart.data(), m_imagPart.data());
}


std::vector<std::complex<float>> FFT::process2(const float* input)
{
    m_input = const_cast<float*>(input);
    fftwf_execute(m_plan2);
    return m_complexResult;
}


const std::vector<float>& FFT::realPart() const
{
    return m_realPart;
}


const std::vector<float>& FFT::imagPart() const
{
    return m_imagPart;
}


float FFT::bias() const
{
    return m_realPart.front();
}


float FFT::nyquist() const
{
    return m_realPart.back();
}

