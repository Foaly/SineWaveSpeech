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

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <jackaudioio.hpp>

class PassThrough: public JackCpp::AudioIO {

private:



public:
    /// Audio Callback Function:
    /// - the output buffers are filled here
    /// args
    virtual int audioCallback(jack_nframes_t nframes,
                              // A vector of pointers to each input port.
                              audioBufVector inBufs,
                              // A vector of pointers to each output port.
                              audioBufVector outBufs){


        /// LOOP over all samples
        for(int frameCNT = 0; frameCNT  < nframes; frameCNT++)
        {

            /// just copy sample-wise
            outBufs[0][frameCNT] =  inBufs[0][frameCNT];

        }

        ///return 0 on success
        return 0;
    }

    /// Constructor
    PassThrough() :
        JackCpp::AudioIO("pass_through", 1,1){


        reserveInPorts(1);
        reserveOutPorts(2);

    }

};

///
///
///
int main(int argc, char *argv[]){

    /// initial ports from constructor created here.
    PassThrough t;

    /// activate the client
    t.start();

    /// connect input
    t.connectFromPhysical(0,0);

    /// connect ports to stereo ports
    t.connectToPhysical(0,0);		// connects this client out port 0 to physical destination port 0
    t.connectToPhysical(0,1);		// connects this client out port 1 to physical destination port 1

    ///print names
    std::cout << "outport names:" << std::endl;
    for(unsigned int i = 0; i < t.outPorts(); i++) {
        std::cout << "\t" << t.getOutputPortName(i) << std::endl;
    }


    ///print names
    std::cout << "inport names:" << std::endl;
    for(unsigned int i = 0; i < t.inPorts(); i++) {
        std::cout << "\t" << t.getInputPortName(i) << std::endl;
    }

    /// run for EVER
    sleep(-1);

    /// never reached:!=
    t.disconnectInPort(0);	// Disconnecting ports.
    t.disconnectOutPort(1);
    t.close();	// stop client.
    return 0;
}
