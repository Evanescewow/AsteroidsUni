#pragma once
#include "Game.h"
#include "Menu.h"
#include "ControlsScreen.h"
#include "../Common/Console.h"

/* Class GameStateMachine
 * Brief:
 *	State machine that holds all possible screen states. handles the logic for 
 * running the updates on each state and switching between them. Handles switching
 * of states depending on the return of the current state's Go method.
 * Params:
 *
 * Returns:
 */
class GameStateMachine
{
public:
	/* Constructor
	 * Brief:
	 *	passes the pointer to the application window to
	 *  all the different states contained within the machine
	 * Params:
	 *	<renderwindow*> window	-	window to pass to all the states
	 */
	GameStateMachine(sf::RenderWindow* window);

	/* bool Go
	 * Brief:
	 *	Main run function for the entire application.
	 * switches which state to run depending on the current state set
	 * by the _currentState member. Takes the returned state from the current
	 * state running and makes a comparison to see if the state should change.
	 * If the returned state flags an exit the function returns false indication to
	 * the main loop that it should cease.
	 * Returns:
	 * <bool> - should the application continue
	 */
	bool Go();

private:
	sf::Clock _resetClock;						// Clock to stop button spam and instance state swap

	Game _game;									// Main game state
	Menu _menu;									// Menu screen state
	ControlsScreen _controlsScreen;				// Controls screen state

	GameState _currentState = GameState::MENU;	// current state of the application
	static constexpr float BUTTON_RESET_TIME_IN_SECONDS = 0.2f;	// amount of time before another button press is registered
};

