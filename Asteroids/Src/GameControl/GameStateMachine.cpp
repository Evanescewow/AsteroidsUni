#include "GameStateMachine.h"

GameStateMachine::GameStateMachine(sf::RenderWindow* window)
	:
	_game(window)
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
	}
}
