#pragma once
#include <SFML/Graphics.hpp>

/* Class ResourceManager
*  (Singleton)
 * Brief:
 *	Loads all the textures and fonts that will be used in the project. This is done to 
 *  eliminate the need to load the same items multiple times. Class has been made as singleton
 *  to allow access to these common items throughout the project.
 */
class ResourceManager
{
public:
	// Method to get the static instance of the resource manager
	// after first creation the same instance will always be returned.
	static ResourceManager& getInstance()
	{
		static ResourceManager    instance; // Guaranteed to be destroyed.
							  // Instantiated on first use.
		return instance;
	}
private:

	// Load the fonts and textures
	ResourceManager() {
		// load texture
		if (!_buttonTexture.loadFromFile(_textureLocation))
			throw std::exception("Unable to load button texture!");

		// Load font
		if (!_textFont.loadFromFile(_fontLocation))
			throw std::exception("Unable to load font!");
	}                    // Constructor? (the {} brackets) are needed here.

public:
	// delete copy constructor and assignment operator to prevent
	// accidental instance creation
	ResourceManager(ResourceManager const&) = delete;
	void operator=(ResourceManager const&) = delete;


	// Getters for the fonts and textures
	sf::Font& GetFont() { return _textFont; }
	sf::Texture& GetButtonTexture() { return _buttonTexture; }


	//Giving public access to command line args only
public:
	std::vector<char*> _commandLineArgs;					// all args already verified as int

	// Members
private:
	sf::Font _textFont;										// Main font for the text throughout the application
	sf::Texture _buttonTexture;								// texture to assign to buttons
		
	std::string _fontLocation = "arial.ttf";				// Path for the font
	std::string _textureLocation = "Assets/button.png";		// Path for the button texture
};

