#pragma once

enum class GameState
{
	MENU,
	GAME,
	CONTROL
};

class State
{
public:
	State() {}
	~State() {}

	virtual GameState Go() = 0;

};

