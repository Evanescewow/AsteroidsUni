#include "GameStateMachine.h"

/* Constructor
 * Brief:
 *	passes the pointer to the application window to
 *  all the different states contained within the machine
 * Params:
 *	<renderwindow*> window	-	window to pass to all the states
 */
GameStateMachine::GameStateMachine(sf::RenderWindow* window)
	:
	_game(window),
	_menu(window),
	_controlsScreen(window)
{
}

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
bool GameStateMachine::Go()
{
	bool shouldExit = false;
	

	GameState newState = GameState::INVALID;

		// determine game state and run them
		switch (this->_currentState)
		{
		case GameState::GAME:
			newState = _game.Go();
			break;
		case GameState::MENU:
			newState = _menu.Go();
			break;
		case GameState::CONTROLS:
			newState = _controlsScreen.Go();
			break;
		}

	// Check to see if dead period has passed to prevent auto button spam
	if (_resetClock.getElapsedTime().asSeconds() >= BUTTON_RESET_TIME_IN_SECONDS)
	{
		// Check if a state change has occurred
		if (newState != _currentState)
		{
			// update state and restart spam clock
			_currentState = newState;
			_resetClock.restart();
		}
	}
	// check for exit condition
	if (_currentState == GameState::EXIT)
		return false;
	return true;
}
