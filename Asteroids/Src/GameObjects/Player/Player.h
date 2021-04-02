#pragma once
#include "../Base/WireframeSprite.h"
#include "../Other/Bullet.h"

class Player :
	public WireframeSprite
{
public:
	// Setup shape variables and starting position
	Player();
	~Player();

	/*void Update override
	 * Brief:
	 * Overrides WireframeSprite::Update(). Updates the Players rotation and wraps the object
	 * around the screen if needed. Also handles the player movement via input detection.
	*/
	void Update() override;

	void SetColour(sf::Color color);

private:
	/*void CalculateStartingVerticies
	 * Brief:
	 * Generates starting vertex positions to form a triangle based upon 
	 * TRIANGLE_SCALE
	*/
	void CalculateStartingVertecies();

	/*void HandleInput
	 * Brief:
	 * Handles the players rotation and velocity changed based upon
	 * user input.
	*/
	void HandleInput();

private:
	// Constants 
	static constexpr float TRIANGLE_SCALE = 25.0f;		// Scale of the player's triangle
	static constexpr float ROTATION_CONSTANT = 4.0f;	// How fast the player should rotate
	static constexpr float VELOCITY_INCREMENT = 0.2f;	// Acceleration of player velocity
	static constexpr float MAX_VELOCITY = 5.0f;			// Maximum player velocity (directional, not magnitude)
	sf::Color PLAYER_COLOR = sf::Color::Cyan;			// Color of the player triangle
	static constexpr float PLAYER_THICKNESS = 1;		// Thickness of the player triangle outline

};

