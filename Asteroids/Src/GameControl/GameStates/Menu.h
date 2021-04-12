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

// Private members
private:
	// Font and Texture info
	sf::Font _buttonTextFont;
	sf::Texture _buttonTexture;
	std::string _fontLocation = "arial.ttf";
	std::string _textureLocation = "Assets/button.png";

	// Buttons
	Button* _playButton = nullptr;
	Button* _controlsButton = nullptr;
	Button* _exitButton = nullptr;

	// Constants
	sf::Color BUTTON_DEFAULT_COLOUR = sf::Color::Magenta;
};

