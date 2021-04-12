#pragma once
#include <SFML/Graphics.hpp>

class Button
{
public:
	Button(sf::Font& textFont, sf::Texture& buttonTexture);
	Button(sf::Font& textFont, sf::Texture& buttonTexture, std::string buttonText);

	void SetPosition(const sf::Vector2f position);
	void SetButtonColor(const sf::Color color);
	void SetButtonTextColor(const sf::Color color);
	void SetScale(const float scale);

	void Draw(sf::RenderWindow* window);

	bool Update(sf::RenderWindow* targetWindow);

	sf::FloatRect GetGlobalBounds();
private:
	sf::Font& _buttonFont;
	sf::Text _buttonText;
	sf::Texture& _buttonTexture;
	sf::Sprite _button;

	sf::Color _defaultTextColor = sf::Color::White;
	sf::Color hoverColor = sf::Color::Red;
};

