#include "ControlsScreen.h"

ControlsScreen::ControlsScreen(sf::RenderWindow* window, GameState state)
	:
	State(window, GameState::CONTROLS)
{
	// push back controls screen text to a vector
	// a little inefficient compared to simply inserting but here it doesn't matter
	// and it's easier to read
	_controls.push_back("Key bindings(in game):");
	_controls.push_back("Esc			-		return to menu");
	_controls.push_back("Tab			-		open console");
	_controls.push_back("(w)(a)(s)(d)	-		player movement");
	_controls.push_back("space bar		-		fire bullet");

	
}

ControlsScreen::~ControlsScreen()
{
	// clear up memory
	if (this->_menuButton)
		delete this->_menuButton;
}
