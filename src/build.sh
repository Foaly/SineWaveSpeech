#!/bin/sh

g++ -std=c++11 -O3 MagnitudeSpectrum.cpp SineWaveSpeech.cpp CaptainJack.cpp -ljackcpp -ljack -lfftw3f -o sineWaveSpeech
