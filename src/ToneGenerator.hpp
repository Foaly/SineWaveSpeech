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

#ifndef TONEGENRATOR_INCLUDE
#define TONEGENRATOR_INCLUDE

#include <cmath>

/*
 * \brief ToneGenerator base class
 */

class ToneGenerator
{
public:
    ToneGenerator(double _frequency, double _amplitude, double _sampleRate) :
        sampleRate(_sampleRate),
        m_amplitude(_amplitude),
        m_frequency(_frequency)
    {

    }
    
    virtual double getNextSample() = 0;
    
    virtual void frequency(double frequency)
    {
        m_frequency = frequency;
    }
    
    double frequency()
    {
        return m_frequency;
    }
    
    virtual void amplitude(double amplitude)
    {
        m_amplitude = amplitude;
    }
    
    double amplitude()
    {
        return m_amplitude;
    }
    
    
    double sampleRate;
    
protected:
    double m_amplitude;
    double m_frequency;
    
    const double PI = std::atan(1.0) * 4.0;
    const double twoPI = 2.0 * ToneGenerator::PI;
};

#endif // TONEGENRATOR_INCLUDE
