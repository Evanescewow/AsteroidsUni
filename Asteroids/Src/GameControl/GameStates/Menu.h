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
	sf::Font _buttonTextFont;
	sf::Texture _buttonTexture;
	std::string fontLocation = "arial.ttf";
	std::string _textureLocation = "Assets/button.png";

	sf::Color _buttonDefaultColor = sf::Color::Magenta;

	Button* playButton = nullptr;
};

