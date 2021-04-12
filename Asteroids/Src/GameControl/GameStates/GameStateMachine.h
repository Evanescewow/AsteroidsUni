#pragma once
#include "Game.h"
#include "Menu.h"
#include "ControlsScreen.h"
#include "../Common/Console.h"


class GameStateMachine
{
public:
	GameStateMachine(sf::RenderWindow* window);
	bool Go();

private:
	sf::Clock _resetClock;

	Game _game;
	Menu _menu;
	ControlsScreen _controlsScreen;

	GameState _currentState = GameState::MENU;
	static constexpr float BUTTON_RESET_TIME_IN_SECONDS = 0.2f;
};

