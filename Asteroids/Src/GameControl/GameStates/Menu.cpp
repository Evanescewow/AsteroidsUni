#include "Menu.h"
#include "../../Global/ApplicationDefines.h"

Menu::Menu(sf::RenderWindow* window)
	:
	State(window, GameState::MENU)
{
	// load texture
	if (!_buttonTexture.loadFromFile(_textureLocation))
		throw std::exception("Unable to load button texture!");

	// Load font
	if (!_buttonTextFont.loadFromFile(_fontLocation))
		throw std::exception("Unable to load font!");

	// create buttons
	_playButton = new Button(_buttonTextFont, _buttonTexture, "Play");
	_controlsButton = new Button(_buttonTextFont, _buttonTexture, "Controls");
	_exitButton = new Button(_buttonTextFont, _buttonTexture, "Exit");

	// local definition for x position
	float posX = static_cast<float>((WINDOW_WIDTH / 2) - (_playButton->GetGlobalBounds().width / 2));

	// setup buttons
	_playButton->SetPosition({ posX, 100.0f });
	_playButton->SetButtonColor(BUTTON_DEFAULT_COLOUR);

	_controlsButton->SetPosition({ posX, 300.0f });
	_controlsButton->SetButtonColor(BUTTON_DEFAULT_COLOUR);

	_exitButton->SetPosition({ posX, 500.0f });
	_exitButton->SetButtonColor(BUTTON_DEFAULT_COLOUR);
}


Menu::~Menu()
{
	// clear memory of buttons
	if (this->_playButton)
		delete this->_playButton;

	if (this->_controlsButton)
		delete this->_controlsButton;

	if (this->_exitButton)
		delete this->_exitButton;
}

void Menu::ComposeFrame()
{
	// sanity check for nullptr
	// If one is messed up can guess all are
	if (!this->_playButton)
		throw std::exception("Something has gone terribly wrong, the buttons don't exist in the menu!");

	// Draw buttons
	this->_playButton->Draw(_window);
	this->_controlsButton->Draw(_window);
	this->_exitButton->Draw(_window);
}

void Menu::UpdateModel()
{
	// Update play button and if user is pressing change to the game
	if (this->_playButton->Update(_window))
		this->_nextState = GameState::GAME;

	// Update controls button and if user is pressing change to the controls screen
	if (this->_controlsButton->Update(_window))
		this->_nextState = GameState::CONTROL;

	// Update exit button and if user is pressing issue the command to exit application
	if (this->_exitButton->Update(_window))
		this->_nextState = GameState::EXIT;
}
