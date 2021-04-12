#include "GameStateMachine.h"

GameStateMachine::GameStateMachine(sf::RenderWindow* window)
	:
	_game(window),
	_menu(window)
{
}

bool GameStateMachine::Go()
{
	bool shouldExit = false;


	// determine game state and run them
	switch (this->_currentState)
	{
	case GameState::GAME:
		 _currentState = _game.Go();
		break;
	case GameState::MENU:
		_currentState = _menu.Go();
		break;
	}

	// check for exit condition
	if (_currentState == GameState::EXIT)
		return false;
	return true;
}
