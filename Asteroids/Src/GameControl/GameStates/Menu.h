#pragma once
#include "State.h"
#include "../Common/Button.h"

class Menu : public State
{
public:

	Menu(sf::RenderWindow* window);
	~Menu();

// Private Methods
private:
	void ComposeFrame() override;
	void UpdateModel() override;

	void SetupDisplayText();
	void SetupButtons();

// Private members
private:
	// Font and Texture info
	sf::Font& _buttonTextFont;
	sf::Texture& _buttonTexture;


	// Buttons
	Button* _playButton = nullptr;
	Button* _controlsButton = nullptr;
	Button* _exitButton = nullptr;

	// Display Text
	sf::Text _titleText;
	sf::Text _creditText;

	// Constants
	sf::Color BUTTON_DEFAULT_COLOUR = sf::Color(100, 100, 100);
};

