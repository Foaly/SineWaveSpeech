#include "MagnitudeSpectrum.hpp"

#include <cmath>
#include <numeric>
#include <algorithm>
#include <cassert>

namespace
{
    const float epsilon = std::numeric_limits<float>::epsilon();
}



MagnitudeSpectrum::MagnitudeSpectrum(std::size_t FFTSize) :
    m_fft(FFTSize),
    m_magnitudeVector(FFTSize / 2 + 1, 0.f)
{
    // Bithack to check if FFTSize is a power of 2
    // http://www.graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
    assert((FFTSize && !(FFTSize & (FFTSize - 1))) && "Argument \"FFTSize\" has to be a power of 2!");
}


void MagnitudeSpectrum::process(std::vector<float> &sampleChunck)
{
    // TODO: apply window function
    
    // do the FFT
    m_fft.process(sampleChunck.data());

    // calculate the magnitude spectrum
    std::transform(m_fft.realPart().begin(), m_fft.realPart().end(), m_fft.imagPart().begin(), m_magnitudeVector.begin(),
                   [] (float realPart, float imagPart)
                   {
                       return std::sqrt(realPart * realPart + imagPart * imagPart);
                   });
    
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

