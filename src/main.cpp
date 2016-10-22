#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <fftw3.h>

#include <iostream>

// Here is a small helper for you! Have a look.
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
