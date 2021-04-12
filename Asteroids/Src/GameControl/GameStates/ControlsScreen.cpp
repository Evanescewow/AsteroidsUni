#include "ControlsScreen.h"
#include "../Common/ResourceManager.h"
#include "../../Global/ApplicationDefines.h"

ControlsScreen::ControlsScreen(sf::RenderWindow* window)
	:
	State(window, GameState::CONTROLS),
	_textFont(ResourceManager::getInstance().GetFont()),
	_buttonTexture(ResourceManager::getInstance().GetButtonTexture())
{
	// push back controls screen text to a vector
	// a little inefficient compared to simply inserting but here it doesn't matter
	// and it's easier to read
	_controls.push_back("Key bindings(in game):");
	_controls.push_back("Esc                  - return to menu");
	_controls.push_back("Tab                  - open console");
	_controls.push_back("(w)(a)(s)(d)      - player movement");
	_controls.push_back("space bar        - fire bullet");
	_controls.push_back("\nUsing the console:");
	_controls.push_back("Type help into the console to get information on the available commands.");

	// create button
	this->_menuButton = new Button(_textFont, _buttonTexture, "Menu");
	// local definition for x position (readability)
	float posX = static_cast<float>((WINDOW_WIDTH / 2) - (_menuButton->GetGlobalBounds().width / 2));
	this->_menuButton->SetButtonColor(BUTTON_DEFAULT_COLOUR);
	this->_menuButton->SetPosition({ posX, 500.0f });

	// setup the controls text
	this->SetupText();
}

ControlsScreen::~ControlsScreen()
{
	// clear up memory
	if (this->_menuButton)
		delete this->_menuButton;
}

void ControlsScreen::SetupText()
{
	std::string output;


	// form the controls string
	for (auto it = this->_controls.begin(); it != _controls.end(); it++)
	{
		output += *it;
		output += "\n";
	}

	// set text object properties
	this->_controlsText.setString(output);
	this->_controlsText.setFont(_textFont);

	// local definition for x position (readability)
	float posX = static_cast<float>((WINDOW_WIDTH / 2) - (_controlsText.getGlobalBounds().width / 2));
	this->_controlsText.setPosition({ posX, 100.0f });
}

void ControlsScreen::ComposeFrame()
{
	// sanity check for nullptr
	// If one is messed up can guess all are
	if (!this->_menuButton)
		throw std::exception("Something has gone terribly wrong, the buttons don't exist in the controls screen!");

	// Draw button and text to screen
	this->_menuButton->Draw(_window);
	this->_window->draw(_controlsText);
}

void ControlsScreen::UpdateModel()
{
	// update the button
	if (this->_menuButton->Update(_window))
		this->_nextState = GameState::MENU;
}
