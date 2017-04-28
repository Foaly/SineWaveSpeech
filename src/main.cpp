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

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <iomanip>
#include <algorithm>

#include "SineWaveSpeech.hpp"
#include "ResourcePath.hpp"

int main(int, char const**)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    window.setFramerateLimit(60);

    // Load a sound
    sf::SoundBuffer originalSoundBuffer;
    if (!originalSoundBuffer.loadFromFile(resourcePath() + "Hallo.wav"))
    {
        std::cerr << "Could not load \"Hello.wav\" sound." << std::endl;
        return EXIT_FAILURE;
    }
    if (originalSoundBuffer.getChannelCount() > 1)
    {
        // TODO: Implement stereo support
        std::cerr << "Loaded sound has to be mono (1 channel)." << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sound originalSound(originalSoundBuffer);
    
    const size_t FFTSize = 512;
    const unsigned int sampleRate = originalSoundBuffer.getSampleRate();
    SineWaveSpeech sineWaveSpeech(FFTSize);
    
    // get the samples as ints
    std::vector<sf::Int16> rawSamples(originalSoundBuffer.getSamples(), originalSoundBuffer.getSamples() + originalSoundBuffer.getSampleCount());
    
    // transform int samples into normalized float samples
    std::vector<float> samples(rawSamples.size());
    std::transform(rawSamples.begin(), rawSamples.end(), samples.begin(),
                   [](float sample)
                   {
                       return sample / 32767.f;
                   });
    
    std::vector<float> outputSamples = sineWaveSpeech.generateSineWaveSpeech(samples, sampleRate);
    
    std::transform(outputSamples.begin(), outputSamples.end(), rawSamples.begin(),
                   [](float sample)
                   {
                       return sample * 32767.f;
                   });
    
    // load the generated sinus sound
    sf::SoundBuffer sinusSoundBuffer;
    if (!sinusSoundBuffer.loadFromSamples(rawSamples.data(), rawSamples.size(), 1, sampleRate)) {
        std::cerr << "Could not load sinus sound." << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sound sinusSound(sinusSoundBuffer);
    
    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Key strokes
            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    // Escape pressed: exit
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                    case sf::Keyboard::O:
                        originalSound.play();
                        break;
                    case sf::Keyboard::Space:
                        sinusSound.play();
                        break;
                    case sf::Keyboard::S:
                        sinusSoundBuffer.saveToFile("generatedSinus.wav");
                        std::cout << "Saved generated sinus." << std::endl;
                        break;
                    default:
                        break;
                        
                }
            }
        }

        // Clear screen
        window.clear();

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
