#ifndef SAWTOOTH_INCLUDE
#define SAWTOOTH_INCLUDE

#include <cmath>

/**
 * \brief A sawtooth wave generator. A sawtooth wave ramps upward and then
 *        sharply drops. See https://en.wikipedia.org/wiki/Sawtooth_wave
 */

class Sawtooth
{
public:
    Sawtooth(double _frequency, double _amplitude, double _sampleRate) :
    m_amplitude(_amplitude),
    sampleRate(_sampleRate),
    m_nextSample(-1.0)
    {
        frequency(_frequency);
    }

    double getNextSample()
    {
        double sample = m_nextSample;

        m_nextSample += m_step;

        if (m_nextSample > 1.0)
        {
            m_nextSample -= 2.0; // wrap around
        }

        return m_amplitude * sample;
    }

    void frequency(double frequency)
    {
        m_frequency = frequency;
        m_step = 2.0 * m_frequency / sampleRate;
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
    double m_amplitude;
    double m_frequency;
    double m_nextSample;
    double m_step;
};

#endif // SAWTOOTH_INCLUDE
