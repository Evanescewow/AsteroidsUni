#pragma once
#include <SFML/Graphics.hpp>
#include <deque>

class Console
{
public:
	enum class CommandType
	{
		INVALID_COMMAND,
		TOGGLE_PLAYER_COLLISION,
		TOGGLE_ASTEROID_COLLISION,
		TOGGLE_BULLET_COLLISION,
		SET_BROAD_COLLISION_BRUTE_FORCE,
		SET_BROAD_COLLISION_UNIFORM_GRID,
		SET_BROAD_COLLISION_QUADTREE,
		SET_NARROW_COLLISION_AABB,
		SET_NARROW_COLLISION_SAT
	};

	struct ParsedCommandData
	{
		ParsedCommandData() {};
		ParsedCommandData(CommandType type, float value = 0.0f)
		:
		commandType(type),
		additionalValue(value){}

		CommandType commandType;	// type of command returned
		float additionalValue = 0.0f;	// container for set commands
	};

public:
	Console();
	~Console();

	ParsedCommandData Update(sf::RenderWindow* window);
	void Draw(sf::RenderWindow* window);

	void Toggle() { this->_isEnabled = !this->_isEnabled; }
	bool IsOpen() const { return this->_isEnabled; }


private:
	ParsedCommandData ParseCommand(std::string& text);
	void PushBackMesage(std::string& text, sf::Color textColour = sf::Color::White);

	ParsedCommandData ParseSet(std::deque<std::string>& parameters);
	ParsedCommandData ParseToggle(std::deque<std::string>& parameters);

private:
	bool _isEnabled = false;
	sf::RectangleShape _consoleRect;

	std::string _inputText;
	std::string fontLocation = "arial.ttf";
	std::string _commandPrefix = "-> ";

	sf::Font _consoleFont;
	sf::Text _currentCommand;
	std::deque<sf::Text> _consoleMessages;

	float xPixelSize = 500.0f;
	float yPixelSize = 300.0f;
	sf::Color CONSOLE_BACKGROUND = sf::Color(20, 20, 20, 200);
	sf::Color MAIN_TEXT_COLOUR = sf::Color::White;
	sf::Color ERROR_COLOUR = sf::Color::Red;
	sf::Color COMMAND_SUCCESS_COLOUR = sf::Color::Green;

	unsigned int nMaxMessages = 0;
	const unsigned int TEXT_SIZE = 16;
};

