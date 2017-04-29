#!/bin/sh

g++ -std=c++11 -O3 FFT.cpp MagnitudeSpectrum.cpp SineWaveSpeech.cpp CaptainJack.cpp -ljackcpp -ljack -lfftw3f -o sineWaveSpeech
