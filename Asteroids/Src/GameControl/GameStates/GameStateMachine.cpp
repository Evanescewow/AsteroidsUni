#include "GameStateMachine.h"

GameStateMachine::GameStateMachine(sf::RenderWindow* window)
	:
	_game(window),
	_menu(window)
{
}

void GameStateMachine::Go()
{

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
}
