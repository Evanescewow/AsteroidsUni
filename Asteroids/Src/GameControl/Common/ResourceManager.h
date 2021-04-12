#pragma once
#include <SFML/Graphics.hpp>

class ResourceManager
{
public:
	static ResourceManager& getInstance()
	{
		static ResourceManager    instance; // Guaranteed to be destroyed.
							  // Instantiated on first use.
		return instance;
	}
private:
	ResourceManager() {
		// load texture
		if (!_buttonTexture.loadFromFile(_textureLocation))
			throw std::exception("Unable to load button texture!");

		// Load font
		if (!_textFont.loadFromFile(_fontLocation))
			throw std::exception("Unable to load font!");
	}                    // Constructor? (the {} brackets) are needed here.

	// C++ 11
	// =======
	// We can use the better technique of deleting the methods
	// we don't want.
public:
	ResourceManager(ResourceManager const&) = delete;
	void operator=(ResourceManager const&) = delete;

	// Note: Scott Meyers mentions in his Effective Modern
	//       C++ book, that deleted functions should generally
	//       be public as it results in better error messages
	//       due to the compilers behavior to check accessibility
	//       before deleted status


	// Getter for the stuff
	sf::Font& GetFont() { return _textFont; }
	sf::Texture& GetButtonTexture() { return _buttonTexture; }


	// Members
private:
	sf::Font _textFont;
	sf::Texture _buttonTexture;

	std::string _fontLocation = "arial.ttf";
	std::string _textureLocation = "Assets/button.png";
};

