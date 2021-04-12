#include "GameStateMachine.h"

GameStateMachine::GameStateMachine(sf::RenderWindow* window)
	:
	_game(window),
	_menu(window),
	_controlsScreen(window)
{
}

bool GameStateMachine::Go()
{
	bool shouldExit = false;
	

	GameState newState = GameState::INVALID;

		// determine game state and run them
		switch (this->_currentState)
		{
		case GameState::GAME:
			newState = _game.Go();
			break;
		case GameState::MENU:
			newState = _menu.Go();
			break;
		case GameState::CONTROLS:
			newState = _controlsScreen.Go();
			break;
		}

	// Check to see if dead period has passed to prevent auto button spam
	if (_resetClock.getElapsedTime().asSeconds() >= BUTTON_RESET_TIME_IN_SECONDS)
	{
		// Check if a state change has occurred
		if (newState != _currentState)
		{
			// update state and restart spam clock
			_currentState = newState;
			_resetClock.restart();
		}
	}
	// check for exit condition
	if (_currentState == GameState::EXIT)
		return false;
	return true;
}
