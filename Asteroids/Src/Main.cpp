#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "Global/ApplicationDefines.h"
#include "GameControl/GameStates/Game.h"
#include "GameControl/GameStates/GameStateMachine.h"
#include "GameControl/Common/ResourceManager.h"

// Check if a string is an integer
bool IsNumber(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

bool HandleCmdArgs(int argc, char* argv[])
{
	// parse command line args
	if (argc > 1)
	{
		bool validArgsGive = true;

		// Check if correct amount of parameters given
		if (argc != 3)
			validArgsGive = false;

		// Check all arguments are numbers
		for (int i = 1; i < argc; i++)
		{
			if (!IsNumber(argv[i]))
			{
				validArgsGive = false;
			}
		}

		if (!validArgsGive)
		{
			return false;
		}

		// push back commands line args to resource manager
		for (int i = 1; i < argc; i++)
		{
			ResourceManager::getInstance()._commandLineArgs.push_back(argv[i]);
		}
	}

	return true;
}

// Entry point
int main(int argc, char* argv[])
{
	// Handle any arguments passed to command line
	bool isTestMode = HandleCmdArgs(argc - 1, argv);

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

		// check if in testing mode
		if (isTestMode)
		{
			// increment fps counting variables
			fpsCounter += fps;
			frameCounter++;
		}

		// Update window title
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
	}

	// output average fps
 	if (isTestMode)
 	{
		std::ofstream dataFile;
		dataFile.open("output.txt");
		unsigned long averageFps = fpsCounter / frameCounter;
		dataFile << argv[1] << std::endl << "Average Fps: " << averageFps;
		dataFile.close();

		std::cout << averageFps;
	}

	return EXIT_SUCCESS;
}
