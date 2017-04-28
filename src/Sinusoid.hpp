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

#ifndef SINUSOID_INCLUDE
#define SINUSOID_INCLUDE

#include <cmath>

/*
 * \brief A sine wave generator. It uses matrix rotation as an optimization.
 *        See https://en.wikipedia.org/wiki/Rotation_matrix#In_two_dimensions
 *        Note this implementation is optimized for sinusoids whos frequency stays the
 *        same between calls to getNextSample(). If the frequency is changed between 
 *        every call to getNextSample() a naive implementation using std::sin() will be
 *        more efficent. Also note that the phase stays continous when the frequency changes.
 */

class Sinusoid
{
public:
    Sinusoid(double _frequency, double _amplitude, double _sampleRate) :
        amplitude(_amplitude),
        sampleRate(_sampleRate),
        m_x(0.0),
        m_y(1.0)
    {
        frequency(_frequency);
    }

    double getNextSample()
    {
        double oldX = m_x;
        m_x = m_x * m_cosinus + m_y * m_sinus;
        m_y = oldX * -m_sinus + m_y * m_cosinus;
        
        return amplitude * oldX;
    }
    
    void frequency(double frequency)
    {
        m_frequency = frequency;
        double step = twoPI * m_frequency / sampleRate;
        m_sinus = std::sin(step);
        m_cosinus = std::cos(step);
    }
    
    double frequency()
    {
        return m_frequency;
    }

    double amplitude;
    double sampleRate;
    
private:
    double m_frequency;
    double m_sinus;
    double m_cosinus;
    double m_x;
    double m_y;
    
    const double PI = std::atan(1.0) * 4.0;
    const double twoPI = 2.0 * PI;
};

#endif // SINUSOID_INCLUDE
