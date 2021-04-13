#pragma once
#include "State.h"
#include "../Common/Button.h"

/* Class Menu
*  subclass of state
*  Brief:
*  Represents the Menu screen that is presented to the user
*  at application start.
*/
class Menu : public State
{
public:
	/* Constructor
	* Brief:
	* First passes the window to the parent class to store as a 
	* protected variable. Also passed to the parent is a game state with
	* the MENU property. this is to assign the default state so that state
	* changes can be detected.
	* Assigns references to the text font and button texture from the resource manager
	* Creates the buttons and then assigns their properties.
	* Assigns the properties to the display text
	*/
	Menu(sf::RenderWindow* window);

	/* Destructor
	*  cleans up the memory where the buttons are stored
	*/
	~Menu();

// Private Methods
private:
	/* void ComposeFrame override
	*  Brief:
	*  Overridden method of the base class. Handles drawing
	*  of the current frame. method is called inside the parent class
	*  in the Go method.
	*/
	void ComposeFrame() override;

	/* void UpdateModel override
	 * Brief:
	 *	overridden method from parent class.
	 *  handles all update logic within the frame.
	 *  Checks if buttons are nullptr before calling their update methods
	 *  for sanity. If button clicks are detected nextState will be updated
	 *  to flag the scene manager to change the scene.
	 */
	void UpdateModel() override;
	
	/* void SetupDisplayText
	 * Brief:
	 *	setups up the parameters and positioning for the menu display text
	 */
	void SetupDisplayText();

	/* void SetupButtons
	 * Brief:
	 *	sets up the parameters and positioning for the menu buttons.
	 */
	void SetupButtons();

// Private members
private:
	// Font and Texture info
	sf::Font& _buttonTextFont;					// Reference to the font the menu text should use
	sf::Texture& _buttonTexture;				// Reference to the texture the menu buttons should use

	// Buttons
	Button* _playButton = nullptr;				// Button to navigate to the Game state
	Button* _controlsButton = nullptr;			// Button to navigate to the controls state
	Button* _exitButton = nullptr;				// Button to flag the application to close

	// Display Text
	sf::Text _titleText;						// Text that states the title of the application
	sf::Text _creditText;						// Text that states programmer's name

	// Constants
	sf::Color BUTTON_DEFAULT_COLOUR = sf::Color(100, 100, 100);	// background colour of the buttons
};

