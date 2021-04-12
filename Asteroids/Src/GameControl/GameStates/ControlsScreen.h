#pragma once
#include <SFML/Graphics.hpp>
#include "State.h"
#include "../Common/Button.h"
#include <vector>

class ControlsScreen : public State
{
public:
	ControlsScreen(sf::RenderWindow* window, GameState state);
	~ControlsScreen();

private:
	std::vector<std::string> _controls;
	sf::Text _controlsText;

	Button* _menuButton = nullptr;

	sf::Color BUTTON_DEFAULT_COLOUR = sf::Color(100, 100, 100);
};

