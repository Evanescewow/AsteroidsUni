#include "Button.h"

Button::Button(sf::Font& textFont, sf::Texture& buttonTexture)
	:
	_buttonFont(textFont),
	_buttonTexture(buttonTexture)
{
	this->_buttonText.setFont(textFont);
	this->_button.setTexture(_buttonTexture);
}

Button::Button(sf::Font& textFont, sf::Texture& buttonTexture, std::string buttonText)
	:
	Button(textFont, buttonTexture)
{
	this->_buttonText.setString(buttonText);
}

void Button::SetPosition(const sf::Vector2f position)
{
	// Set the position of the button sprite
	this->_button.setPosition(position);

	// set the position of the button text to be centered in the sprite
	_buttonText.setPosition(_button.getPosition());
	_buttonText.move({ (_button.getGlobalBounds().width / 2) - (_buttonText.getGlobalBounds().width / 2),
		(_button.getGlobalBounds().height / 2) - (_buttonText.getGlobalBounds().height / 2) });
}

void Button::SetButtonColor(const sf::Color color)
{
	this->_button.setColor(color);
}

void Button::SetButtonTextColor(const sf::Color color)
{
	this->_defaultTextColor = color;
}

void Button::SetScale(const float scale)
{
	this->_button.setScale({ scale, scale });
	this->_buttonText.setScale({ scale, scale});
}

void Button::Draw(sf::RenderWindow* window)
{
	window->draw(this->_button);
	window->draw(this->_buttonText);
}

bool Button::Update(sf::RenderWindow* targetWindow)
{
	// check if mouse is on button
	if (this->_button.getGlobalBounds().contains(targetWindow->mapPixelToCoords(sf::Mouse::getPosition(*targetWindow))))
	{
		// if it is change text to hover color
		this->_buttonText.setFillColor(hoverColor);
		
		// check if the user is clicking
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			return true;
	}
	else
	{
		// otherwise reset text colour and return false
		this->_buttonText.setFillColor(_defaultTextColor);
	}

	return false;
}

sf::FloatRect Button::GetGlobalBounds()
{
	return this->_button.getGlobalBounds();
}
