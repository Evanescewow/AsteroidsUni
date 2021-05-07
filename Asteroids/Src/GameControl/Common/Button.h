#pragma once
#include "SFML/Graphics.hpp"

/* Class Button
 * Creates a simple button with a passed texture and font 
 */
class Button
{
public:
	/* Constructor
	*  Brief:
	*  assigns references of the desired font and texture to be used
	*  for the button.
	*  References are used as this class depends upon these parameters
	*  and thus share a common lifecycle. Behavior for deleted references
	*  is undefined.
	*/
	Button(sf::Font& textFont, sf::Texture& buttonTexture);

	// Overloaded constructor to also take in the text string at assignment
	Button(sf::Font& textFont, sf::Texture& buttonTexture, std::string buttonText);

	/* void SetPosition
	* Brief:
	*	Sets the position of the outer button and calculates
	*	the center position for the text accordingly.
	* Params:
	* <const vector2f> position - position to assign to the button
	*/
	void SetPosition(const sf::Vector2f position);

	/* void SetButtonColor
	* Brief:
	*	sets the color of the outer button.
	* Params:
	* <const color> color- color to assign to the outer button texture
	*/
	void SetButtonColor(const sf::Color color);

	/* void SetButtonTextColor
	* Brief:
	*	sets the color of the button text
	* Params:
	* <const color> color - color to assign to the text
	*/
	void SetButtonTextColor(const sf::Color color);

	/* void SetScale
	* Brief:
	*	sets the scale of both the button sprite and the text
	*	within it
	* Params:
	*	 <const float> scale - scale to set to the button
	*/
	void SetScale(const float scale);

	/* void Draw
	* Brief:
	*	sets the color of the outer button.
	* Params:
	* <const color> color- color to assign to the outer button texture
	*/
	void Draw(sf::RenderWindow* window);

	/* void Update
	* Brief:
	*	Checks to see if the mouse position is within the bounds of the button.
	*	if this is the case it will hilight the text within the button.
	*	if a mouse click is detected while inside the button this method will return true
	*	otherwise false.
	* Params:
	* <renderwindow*>	targetWindow	-	window in which the button and mouse is contained
	*/
	bool Update(sf::RenderWindow* targetWindow);

	// Getter for the global bounds of the button sprite
	sf::FloatRect GetGlobalBounds();

	// Private members
private:
	sf::Font& _buttonFont;								// Font of the button text
	sf::Text _buttonText;								// The object contained within the button
	sf::Texture& _buttonTexture;						// Texture of the button sprite
	sf::Sprite _button;									// sprite which holds the global bounds

	sf::Color _defaultTextColor = sf::Color::White;		// default colour of the button text
	sf::Color _hoverColor = sf::Color::Red;				// highlight button text colour
};

