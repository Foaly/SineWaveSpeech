#ifndef MAGNITUDESPECTRUM_H
#define MAGNITUDESPECTRUM_H

#include <vector>

#include "FFT.hpp"


class MagnitudeSpectrum
{
public:
    MagnitudeSpectrum(std::size_t FFTSize);
    
    void                      process(std::vector<float>& sampleChunck);
    const std::vector<float>& getMagnitudeSpectrum() const;
    const std::vector<float>  getLogarithmicMagnitudeSpectrum();
    
    
private:
    FFT m_fft;
    std::vector<float> m_magnitudeVector;
    std::vector<float> m_logarithmicMagnitudeVector;
};


#endif // MAGNITUDESPECTRUM_H
