#pragma once
#include <SFML/Graphics.hpp>

enum class GameState
{
	MENU,
	GAME,
	CONTROL,
	INVALID
};

class State
{
public:
	State(sf::RenderWindow* window, GameState defaultState):
		_window(window), 
		_defaultState(defaultState),
		_nextState(defaultState){}
	~State() {}

	virtual GameState Go()
	{
		// handle rendering
		this->_window->clear(sf::Color::Black);
		this->UpdateModel();
		this->ComposeFrame();
		this->_window->display();

		// Check if a scene change is queued
		if (this->_nextState != this->_defaultState)
		{
			// store state to change and reset state for next usage
			GameState temp = this->_nextState;
			this->_nextState = this->_defaultState;

			// return new state
			return temp;
		}

		// no change needed
		return _defaultState;
	}

	virtual void ComposeFrame() = 0;
	virtual void UpdateModel() = 0;

private:
	GameState _defaultState = GameState::INVALID;

protected:
	sf::RenderWindow* _window = nullptr;
	GameState _nextState = GameState::INVALID;
};

