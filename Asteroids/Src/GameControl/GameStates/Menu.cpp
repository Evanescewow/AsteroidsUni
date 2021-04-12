#include "Menu.h"
#include "../../Global/ApplicationDefines.h"
#include "../Common/ResourceManager.h"

Menu::Menu(sf::RenderWindow* window)
	:
	State(window, GameState::MENU),
	_buttonTextFont(ResourceManager::getInstance().GetFont()),
	_buttonTexture(ResourceManager::getInstance().GetButtonTexture())
{


	// create buttons
	_playButton = new Button(_buttonTextFont, _buttonTexture, "Play");
	_controlsButton = new Button(_buttonTextFont, _buttonTexture, "Controls");
	_exitButton = new Button(_buttonTextFont, _buttonTexture, "Exit");

	// setup buttons
	this->SetupButtons();

	// setup display text
	this->SetupDisplayText();
}


Menu::~Menu()
{
	// clear memory of buttons
	// there should be no reason these don't exist but I check for sanity sake
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

	// Draw display text
	this->_window->draw(this->_titleText);
	this->_window->draw(this->_creditText);
}

void Menu::UpdateModel()
{
	// Update play button and if user is pressing change to the game
	if (this->_playButton->Update(_window))
		this->_nextState = GameState::GAME;

	// Update controls button and if user is pressing change to the controls screen
	if (this->_controlsButton->Update(_window))
		this->_nextState = GameState::CONTROLS;

	// Update exit button and if user is pressing issue the command to exit application
	if (this->_exitButton->Update(_window))
		this->_nextState = GameState::EXIT;
}

void Menu::SetupDisplayText()
{
	// set text font
	_titleText.setFont(_buttonTextFont);
	_creditText.setFont(_buttonTextFont);

	// setup title text
	_titleText.setString("                     ASTEROIDS\nA Spatial Partitioning Demonstration");
	_titleText.setPosition({ (WINDOW_WIDTH / 2) - (_titleText.getGlobalBounds().width / 2) , 20.0f});
	_titleText.setFillColor(sf::Color(95, 100, 110));

	// setup the credit text
	_creditText.setString("created by Jack Clements");
	_creditText.setScale({ 0.7f, 0.7f });
	_creditText.setPosition({  WINDOW_WIDTH -(_creditText.getGlobalBounds().width + WINDOW_WIDTH / 16),
		WINDOW_HEIGHT -((_creditText.getGlobalBounds().height + 10.0f))});
	_creditText.setFillColor(sf::Color(95, 100, 110));
}

void Menu::SetupButtons()
{
	// local definition for x position (readability)
	float posX = static_cast<float>((WINDOW_WIDTH / 2) - (_playButton->GetGlobalBounds().width / 2));

	// setup buttons
	_playButton->SetPosition({ posX, 100.0f });
	_playButton->SetButtonColor(BUTTON_DEFAULT_COLOUR);

	_controlsButton->SetPosition({ posX, 300.0f });
	_controlsButton->SetButtonColor(BUTTON_DEFAULT_COLOUR);

	_exitButton->SetPosition({ posX, 500.0f });
	_exitButton->SetButtonColor(BUTTON_DEFAULT_COLOUR);
}
