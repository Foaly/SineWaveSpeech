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

#ifndef TRIANGLE_INCLUDE
#define TRIANGLE_INCLUDE

#include <cmath>

class Triangle
{
public:
    Triangle(double _frequency, double _amplitude, double _sampleRate) :
        sampleRate(_sampleRate),
        m_amplitude(_amplitude),
        m_nextSample(0.0),
        m_sign(1)
    {
        frequency(_frequency);
    }

    double getNextSample()
    {
        double sample = m_nextSample;

        m_nextSample += m_step * m_sign;

        if (m_nextSample > 1.0)
        {
            m_nextSample = 1.0 - (m_nextSample - 1.0); // wrap around
            m_sign = -1;
        }
        else if (m_nextSample < -1.0)
        {
            m_nextSample = -1.0 - (m_nextSample + 1.0); // wrap around
            m_sign = 1;
        }

        return m_amplitude * sample;
    }

    void frequency(double frequency)
    {
        m_frequency = frequency;
        m_step = 4.0 * m_frequency / sampleRate;
    }

    double frequency()
    {
        return m_frequency;
    }

    void amplitude(double amplitude)
    {
        m_amplitude = amplitude;
    }

    double amplitude()
    {
        return m_amplitude;
    }


    double sampleRate;

private:
    double m_frequency;
    double m_amplitude;
    double m_nextSample;
    signed char m_sign;
    double m_step;
};

#endif // TRIANGLE_INCLUDE
