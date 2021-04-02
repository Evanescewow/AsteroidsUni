#include <SFML/Graphics.hpp>
#include "Global/ApplicationDefines.h"
#include "GameControl/Game.h"


// Entry point
int main(void)
{
	//setup game window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 24), WINDOW_TITLE, sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(WINDOW_MAX_FRAMERATE);

	// Create clock for generating fps counter
	sf::Clock clock;
	sf::Time time;


	Game theGame(&window);

	while (window.isOpen())
	{
		// Get elapsed time for fps
		time = clock.getElapsedTime();

		// Update window title
		std::string title = (std::string)WINDOW_TITLE + " | FPS: " + std::to_string(1.0f / time.asSeconds());
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

		// Run the game
		theGame.Go();
	}

	return EXIT_SUCCESS;
}