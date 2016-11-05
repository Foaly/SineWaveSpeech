#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <iomanip>
#include <algorithm>

#include "MagnitudeSpectrum.hpp"
#include "ResourcePath.hpp"


int main(int, char const**)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    window.setFramerateLimit(60);

    // Load a sound
    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile(resourcePath() + "hallo.wav")) {
        std::cerr << "Could not load \"hello.wav\" sound." << std::endl;
        return EXIT_FAILURE;
    }
    
    sf::Sound sound(soundBuffer);

    // Play the music
    sound.play();
    
    const size_t FFTSize = 512;
    MagnitudeSpectrum magnitudeSpectrum(FFTSize);
    
    // get the samples as ints
    std::vector<sf::Int16> rawSamples(soundBuffer.getSamples(), soundBuffer.getSamples() + soundBuffer.getSampleCount());
    // make sure it can always be devided through FFTSize without remainder, if necessary add 0's
    auto remainder = FFTSize - (rawSamples.size() % FFTSize);
    rawSamples.insert(rawSamples.end(), remainder, 0);
    
    // calculate how many times the FFT will be called
    // -1 to avoid out of bounds reading on last iteration because of our 50% sliding window
    std::size_t numberOfRepeats = rawSamples.size() / (FFTSize / 2) - 1;
    
    // transform int samples into normalized float samples
    std::vector<float> samples(rawSamples.size());
    std::transform(rawSamples.begin(), rawSamples.end(), samples.begin(),
                   [](float sample)
                   {
                       return sample / 32767.f;
                   });
    
    float                           maxMagnitude = 0.f;
    float                           minMagnitude = 0.f;
    std::vector<std::vector<float>> magnitudes;
    
    auto chunckBegin = samples.cbegin();
    for (int i = 0; i < numberOfRepeats; i++)
    {
        std::vector<float> sampleChunck(FFTSize);
        std::copy(chunckBegin, chunckBegin + FFTSize, sampleChunck.begin());
        
        magnitudeSpectrum.process(sampleChunck);
        
        magnitudes.push_back(magnitudeSpectrum.getMagnitudeSpectrum());
        
        // normalize FFT bins
        std::transform(magnitudes.back().begin(), magnitudes.back().end(), magnitudes.back().begin(),
                       [](float bin)
                       {
                           return bin / FFTSize;
                       });
        
        chunckBegin += FFTSize / 2;
        
        // find the max FFT bin
        auto minmax = std::minmax_element(magnitudes.back().begin(), magnitudes.back().end());
        // check if it's bigger than any previous one
        if (*minmax.second > maxMagnitude)
            maxMagnitude = *minmax.second;
        if (*minmax.first < minMagnitude)
            minMagnitude = *minmax.first;
    }
    
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "min: " << minMagnitude << " max: " << maxMagnitude << std::endl;

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
                    case sf::Keyboard::S:
                        sound.play();
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
