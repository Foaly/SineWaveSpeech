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


#include "SineWaveSpeech.hpp"

#include <jackaudioio.hpp>

#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <unistd.h>

#include <termios.h>
#include <stdio.h>

static struct termios old, newT;

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  newT = old; /* make new settings same as old settings */
  newT.c_lflag &= ~ICANON; /* disable buffered i/o */
  newT.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &newT); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void)
{
  return getch_(0);
}


class CaptianJack: public JackCpp::AudioIO {

private:
    std::vector<float> m_in;
    std::size_t m_fs;


public:
    /// Jack Audio Callback Function
    virtual int audioCallback(jack_nframes_t nframes,
                              // A vector of pointers to each input port.
                              audioBufVector inBufs,
                              // A vector of pointers to each output port.
                              audioBufVector outBufs)
    {
       /* for(int i = 0; i  < nframes; ++i)
        {
            inBufs[0][i] *= 5.f;
        }*/

        auto halfNFrames = nframes / 2;
        std::copy_n(m_in.begin() + halfNFrames, halfNFrames, m_in.begin());

        std::copy_n(inBufs[0], nframes, m_in.begin() + halfNFrames);

        auto out = m_sineGenerator.generateSineWaveSpeech(m_in, m_fs);

        for(int i = 0; i  < nframes; ++i)
        {
            std::cout << out[i] << std::endl;
            outBufs[0][i] = out[i];
        }

        return 0;
    }

    /// Constructor
    CaptianJack() :
        JackCpp::AudioIO("captian_jack", 1,1),
        m_sineGenerator(512, false),
        m_in (static_cast<std::size_t>(getBufferSize()) * 1.5f, 0.f),
        m_fs (getSampleRate())
    {
        reserveInPorts(1);
        reserveOutPorts(2);

        /// activate the client
        start();

        /// connect input
        connectFromPhysical(0,0);

        /// connect ports to stereo ports
        connectToPhysical(0,0);		// connects this client out port 0 to physical destination port 0
        connectToPhysical(0,1);		// connects this client out port 1 to physical destination port 1
    }

    ~CaptianJack()
    {
        disconnectInPort(0);	// Disconnecting ports.
        disconnectOutPort(0);
        close();	// stop client.
    }


    SineWaveSpeech m_sineGenerator;
};

int main(int argc, char *argv[]) {
    CaptianJack cj;

    ///print names
    std::cout << "outport names:" << std::endl;
    for(unsigned int i = 0; i < cj.outPorts(); i++) {
        std::cout << "\t" << cj.getOutputPortName(i) << std::endl;
    }

    ///print names
    std::cout << "inport names:" << std::endl;
    for(unsigned int i = 0; i < cj.inPorts(); i++) {
        std::cout << "\t" << cj.getInputPortName(i) << std::endl;
    }

    while (true)
    {
        auto c = getch();
        switch (c) {
        case 'q':
            return 0;
        case 'w':
            cj.m_sineGenerator.nextToneGenerator();
            break;
        default:
            std::cout << c << std::endl;
            break;
        }
    }

    return 0;
}
