#include "Bullet.h"
#define _USE_MATH_DEFINES
#include <MAth.h>
#include "../Global/ApplicationDefines.h"

Bullet::Bullet(sf::Vector2f playerPosition, float playerRotation)
	:
	WireframeSprite(3)
{
	this->_shape.setRotation(playerRotation);


	// convert degrees to radians
	playerRotation = static_cast<float>((playerRotation * M_PI) / 180);


	//calculate vertex positions for triangle projectile
	this->CalculateStartingVertecies();

	//this->CalculateStartingVertecies();

	this->_shape.setFillColor(BULLET_COLOR);	// set the bullet color
	this->_position = playerPosition;			// set the bullet position
	
	// Setup velocity
	this->_velocity.y = -BULLET_SPEED * cos(playerRotation);
	this->_velocity.x = BULLET_SPEED * sin(playerRotation);
}

Bullet::~Bullet()
{

}


void Bullet::Update()
{
	// call to base update
	WireframeSprite::Update();

	// limit bullets to screen, disable visible after to queue for deletion
	this->CleanupUnseenBullets();
}

void Bullet::CalculateStartingVertecies()
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

void Bullet::CleanupUnseenBullets()
{
	sf::FloatRect rect = this->_shape.getGlobalBounds();

	// Check if bullet is out of any of the bounds
	if ((rect.left + rect.width < 0.0f)
		|| (rect.left >= WINDOW_WIDTH)
		|| (rect.top + rect.height < 0.0f)
		|| (rect.top >= WINDOW_HEIGHT))
	{
		// disable isVisible to queue for deletion on next update
		this->_isVisible = false;
	}


}
