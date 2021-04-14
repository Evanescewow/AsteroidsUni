#include <SFML/Graphics.hpp>
#include <iostream>
#include "Global/ApplicationDefines.h"
#include "GameControl/GameStates/Game.h"
#include "GameControl/GameStates/GameStateMachine.h"


// Entry point
int main(void)
{
	//setup game window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 24), WINDOW_TITLE, sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(WINDOW_MAX_FRAMERATE);

	// Create clock for generating fps counter
	sf::Clock clock;
	sf::Time time;

	// counter for rolling fps average
	unsigned long fpsCounter = 0;
	unsigned long frameCounter = 0;

	// State machine to handle different application screens
	GameStateMachine application(&window);

	while (window.isOpen())
	{
		// Get elapsed time for fps
		time = clock.getElapsedTime();

		// Update window title
		unsigned int fps = 1.0f / time.asSeconds();
		fpsCounter += fps;
		std::string title = (std::string)WINDOW_TITLE + " | FPS: " + std::to_string(fps);
		window.setTitle(title);

		// reset fps clock
		clock.restart().asSeconds();

		//handle closed event
		sf::Event evt;
		if (window.pollEvent(evt))
		{
			if (evt.type == sf::Event::Closed)
			{
				window.close();
				return EXIT_SUCCESS;
			}
		}
		
		// Run the machine
		if (!application.Go())
		{
			// If the state machine returns false an exit call has been processed
			window.close();
		}

		// Increment frame counter for average fps
		frameCounter++;
	}

	// output average fps
	unsigned long averageFps = fpsCounter / frameCounter;
	std::cout << "Average FPS: " << averageFps;


	return EXIT_SUCCESS;
}