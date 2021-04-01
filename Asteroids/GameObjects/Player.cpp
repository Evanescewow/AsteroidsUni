#include "Player.h"
#define _USE_MATH_DEFINES
#include <MAth.h>
#include <iostream>
#include "../Global/ApplicationDefines.h"

Player::Player()
	:
	WireframeSprite(3)
{
	this->CalculateStartingVertecies();
	this->_shape.setFillColor(sf::Color::Transparent);
	this->_shape.setOutlineColor(PLAYER_COLOR);
	this->_shape.setOutlineThickness(PLAYER_THICKNESS);

	this->hitboxShape.setOutlineColor(sf::Color::Blue);

	this->_position = { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f };
}

Player::~Player(){}

/*void Update override
 * Brief:
 * Overrides WireframeSprite::Update(). Updates the Players rotation and wraps the object
 * around the screen if needed. Also handles the player movement via input detection.
*/
void Player::Update()
{
	// Handles all user input to the program
	this->HandleInput();

	WireframeSprite::Update();	//Call to base update
	this->WrapCoordinates();
}

void Player::SetColour(sf::Color color)
{
	this->_shape.setOutlineColor(color);
}

/*void CalculateStartingVerticies
 * Brief:
 * Generates starting vertex positions to form a triangle based upon
 * TRIANGLE_SCALE
*/
void Player::CalculateStartingVertecies()
{
	sf::Vector2f pointOne = sf::Vector2f(this->_position.x, this->_position.y - TRIANGLE_SCALE);
	sf::Vector2f pointTwo = sf::Vector2f(this->_position.x - TRIANGLE_SCALE / 2, this->_position.y + TRIANGLE_SCALE / 2);
	sf::Vector2f pointThree = sf::Vector2f(this->_position.x + TRIANGLE_SCALE / 2, this->_position.y + TRIANGLE_SCALE / 2);

	this->_shape.setPoint(0, pointOne);
	this->_shape.setPoint(1, pointTwo);
	this->_shape.setPoint(2, pointThree);

	this->_pointsNextFrame.push_back(pointOne);
	this->_pointsNextFrame.push_back(pointTwo);
	this->_pointsNextFrame.push_back(pointThree);
}

/*void HandleInput
 * Brief:
 * Handles the players rotation and velocity changed based upon
 * user input.
*/
void Player::HandleInput()
{
	// Handles rotation after receiving user input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->RotateMesh(-ROTATION_CONSTANT);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->RotateMesh(ROTATION_CONSTANT);

	// handles velocity change
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		// Change player rotation to radians for sin and cos functions
		float angleInRadians = this->_shape.getRotation() * (float)(M_PI / 180);

		// Increase the velocity
		float newVelocityX = this->_velocity.x + VELOCITY_INCREMENT * sin(angleInRadians);
		float newVelocityY = this->_velocity.y + VELOCITY_INCREMENT * -cos(angleInRadians);

		// Check if directional velocity is under the limit
		if (newVelocityX < MAX_VELOCITY && newVelocityY < MAX_VELOCITY)
		{
			// Update the velocity to the new values
			this->_velocity.x = newVelocityX;
			this->_velocity.y = newVelocityY;
		}
	}
}
