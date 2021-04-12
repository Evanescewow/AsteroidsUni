#pragma once
#include "Game.h"
#include "Menu.h"
#include "../Common/Console.h"


class GameStateMachine
{
public:
	GameStateMachine(sf::RenderWindow* window);
	void Go();

private:

	Game _game;
	Menu _menu;

	GameState _currentState = GameState::MENU;
};

