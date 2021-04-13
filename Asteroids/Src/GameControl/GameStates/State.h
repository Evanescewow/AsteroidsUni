#pragma once
#include <SFML/Graphics.hpp>

/* enum class GameState
 * Brief:
 *	enum for the different states that belong to the state machine
 */
enum class GameState
{
	MENU,
	GAME,
	CONTROLS,
	EXIT,
	INVALID
};

/* Class State
 * Brief:
 *	superclass that all different states will inherit from. Contains the logic
 * for running the state and provides abstract function for the update and drawing logic.
 */
class State
{
public:
	// Assign passed parameters, window for rendering and
	// default state to check for state change each frame
	State(sf::RenderWindow* window, GameState defaultState):
		_window(window), 
		_defaultState(defaultState),
		_nextState(defaultState){}
	~State() {}

	// Main run method for states. Clears display, updates any logic, draws anything
	// needed to the screen then displays it
	// checks if a state change occurs and if so returns that new state to indicate
	// a change to the state machine
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

	// abstract method for any state update logic
	virtual void ComposeFrame() = 0;
	// abstract method for any drawing within a state
	virtual void UpdateModel() = 0;

private:
	// Normal state to compare return values against
	GameState _defaultState = GameState::INVALID;

protected:
	// window to draw objects to
	sf::RenderWindow* _window = nullptr;

	// value to check against the default state
	// if they don't match, a state change is needed
	GameState _nextState = GameState::INVALID;
};

