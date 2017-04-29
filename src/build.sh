#!/bin/sh

CPPFILES = "FFT.cpp
            MagnitudeSpectrum.cpp
            SineWaveSpeech.cpp
            CaptainJack.cpp"

g++ -std=c++11 $CPPFILES -ljackcpp -ljack -o sineWaveSpeech
