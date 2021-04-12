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
	playButton = new Button(_buttonTextFont, _buttonTexture, "Play");

	// local definition for x position
	float posX = static_cast<float>((WINDOW_WIDTH / 2) - (playButton->GetGlobalBounds().width / 2));

	// setup buttons
	playButton->SetPosition({ posX, 0.0f });
	playButton->SetButtonColor(BUTTON_DEFAULT_COLOUR);
}


Menu::~Menu(){}

void Menu::ComposeFrame()
{
	// Draw buttons
	this->playButton->Draw(_window);
}

void Menu::UpdateModel()
{
	// Update play button and if user is pressing change gamestate
	if (this->playButton->Update(_window))
	{
		this->_nextState = GameState::GAME;
	}
}
