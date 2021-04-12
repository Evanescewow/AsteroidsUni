#include "Button.h"

/* Constructor
*  Brief:
*  assigns references of the desired font and texture to be used
*  for the button.
*  References are used as this class depends upon these parameters
*  and thus share a common lifecycle. Behaviour for deleted references
*  is undefined.
*/
Button::Button(sf::Font& textFont, sf::Texture& buttonTexture)
	:
	_buttonFont(textFont),
	_buttonTexture(buttonTexture)
{
	this->_buttonText.setFont(textFont);
	this->_button.setTexture(_buttonTexture);
}

// Overloaded constructor to also take in the text string at assignment
Button::Button(sf::Font& textFont, sf::Texture& buttonTexture, std::string buttonText)
	:
	Button(textFont, buttonTexture)
{
	this->_buttonText.setString(buttonText);
}

/* void SetPosition
* Brief:
*	Sets the position of the outer button and calculates
*	the center position for the text accordingly.
* Params:
* <const vector2f> position - position to assign to the button
*/
void Button::SetPosition(const sf::Vector2f position)
{
	// Set the position of the button sprite
	this->_button.setPosition(position);

	// set the position of the button text to be centered in the sprite
	_buttonText.setPosition(_button.getPosition());
	_buttonText.move({ (_button.getGlobalBounds().width / 2) - (_buttonText.getGlobalBounds().width / 2),
		(_button.getGlobalBounds().height / 2) - (_buttonText.getGlobalBounds().height / 2) });
}

/* void SetButtonColor
* Brief:
*	sets the color of the outer button.
* Params:
* <const color> color- color to assign to the outer button texture
*/
void Button::SetButtonColor(const sf::Color color)
{
	this->_button.setColor(color);
}

/* void SetButtonTextColor
* Brief:
*	sets the color of the button text
* Params:
* <const color> color - color to assign to the text
*/
void Button::SetButtonTextColor(const sf::Color color)
{
	this->_defaultTextColor = color;
}

/* void SetScale
* Brief:
*	sets the scale of both the button sprite and the text
*	within it
* Params:
*	 <const float> scale - scale to set to the button
*/
void Button::SetScale(const float scale)
{
	this->_button.setScale({ scale, scale });
	this->_buttonText.setScale({ scale, scale});
}

/* void Draw
* Brief:
*	sets the color of the outer button.
* Params:
* <const color> color- color to assign to the outer button texture
*/
void Button::Draw(sf::RenderWindow* window)
{
	window->draw(this->_button);
	window->draw(this->_buttonText);
}

/* void Update
* Brief:
*	Checks to see if the mouse position is within the bounds of the button.
*	if this is the case it will hilight the text within the button.
*	if a mouse click is detected while inside the button this method will return true
*	otherwise false.
* Params:
* <renderwindow*>	targetWindow	-	window in which the button and mouse is contained
*/
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

// Getter for the global bounds of the button sprite
sf::FloatRect Button::GetGlobalBounds()
{
	return this->_button.getGlobalBounds();
}
