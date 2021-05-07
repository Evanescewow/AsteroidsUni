#pragma once
#include "SFML/Graphics.hpp"
#include "State.h"
#include "../Common/Button.h"
#include <vector>

class ControlsScreen : public State
{
public:
	ControlsScreen(sf::RenderWindow* window);
	~ControlsScreen();

private:
	void SetupText();
	void ComposeFrame() override;
	void UpdateModel() override;

private:
	std::vector<std::string> _controls;
	sf::Text _controlsText;
	sf::Font& _textFont;
	sf::Texture& _buttonTexture;

	Button* _menuButton = nullptr;

	sf::Color BUTTON_DEFAULT_COLOUR = sf::Color(100, 100, 100);
};

