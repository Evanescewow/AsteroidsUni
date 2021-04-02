#pragma once
#include "Game.h"
#include "Common/Console.h"


class GameStateMachine
{
public:
	GameStateMachine(sf::RenderWindow* window);
	void Go();

private:

	Game _game;
	GameState _currentState = GameState::GAME;
};

