#ifndef MAGNITUDESPECTRUM_H
#define MAGNITUDESPECTRUM_H

#include <vector>

#include "FFT.hpp"


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
    
    void                      process(std::vector<float>& sampleChunck);
    const std::vector<float>& getMagnitudeSpectrum() const;
    const std::vector<float>  getLogarithmicMagnitudeSpectrum();
    std::size_t               numberOfBins();
    
    
private:
    FFT                  m_fft;
    Range                m_spectrumRangeType;
    std::vector<float>   m_magnitudeVector;
    std::vector<float>   m_logarithmicMagnitudeVector;
};


#endif // MAGNITUDESPECTRUM_H
