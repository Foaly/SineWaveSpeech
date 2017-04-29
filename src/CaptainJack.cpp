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

class CaptianJack: public JackCpp::AudioIO {

private:
    std::vector<float> m_in;
    std::size_t m_fs;
    SineWaveSpeech m_sineGenerator;


public:
    /// Audio Callback Function:
    /// - the output buffers are filled here
    /// args
    virtual int audioCallback(jack_nframes_t nframes,
                              // A vector of pointers to each input port.
                              audioBufVector inBufs,
                              // A vector of pointers to each output port.
                              audioBufVector outBufs) {
        std::copy_n(inBufs[0], nframes, m_in.begin());
        auto out = m_sineGenerator.generateSineWaveSpeech(m_in, m_fs);
        for(int i = 0; i  < nframes; ++i)
        {
            outBufs[0][i] = out[i];
        }

        return 0;
    }

    /// Constructor
    CaptianJack() :
        JackCpp::AudioIO("captian_jack", 1,1),
        m_sineGenerator(512),
        m_in (static_cast<std::size_t>(getBufferSize())),
        m_fs (getSampleRate())
    {
        reserveInPorts(1);
        reserveOutPorts(2);
    }

};

int main(int argc, char *argv[]) {
    CaptianJack cj;

    /// activate the client
    cj.start();

    /// connect input
    cj.connectFromPhysical(0,0);

    /// connect ports to stereo ports
    cj.connectToPhysical(0,0);		// connects this client out port 0 to physical destination port 0
    cj.connectToPhysical(0,1);		// connects this client out port 1 to physical destination port 1

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

    /// run for EVER
    sleep(-1);

    /// never reached:!=
    cj.disconnectInPort(0);	// Disconnecting ports.
    cj.disconnectOutPort(1);
    cj.close();	// stop client.
    return 0;//*/
}
