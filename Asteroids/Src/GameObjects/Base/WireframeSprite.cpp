#define _USE_MATH_DEFINES
#include "WireframeSprite.h"
#include "../../Global/ApplicationDefines.h"
#include <math.h>

WireframeSprite::WireframeSprite(size_t pointCount)
{
	this->_shape.setPointCount(pointCount);

	this->hitboxShape.setFillColor(sf::Color::Transparent);
	this->hitboxShape.setOutlineColor(sf::Color::Blue);
	this->hitboxShape.setOutlineThickness(HITBOX_THICKNESS);

	this->hitboxShape.setOutlineColor(sf::Color::Green);
}

WireframeSprite::~WireframeSprite()
{

}

void WireframeSprite::Update()
{
	// Update position of the convex shape
	this->_position += this->_velocity;
	this->_shape.setPosition(this->_position);


	//Update position for the hit-box display
	sf::FloatRect rect = this->_shape.getGlobalBounds();
	this->hitboxShape.setPosition(this->_shape.getPosition().x - rect.width / 2, this->_shape.getPosition().y - rect.height / 2);
	this->hitboxShape.setSize({ rect.width, rect.height });

	// Update internal points
	float s = std::sin(((this->_shape.getRotation() * (float)M_PI) / 180.0f));
	float c = std::cos(((this->_shape.getRotation() * (float)M_PI) / 180.0f));
	for (unsigned int i = 0; i < this->_shape.getPointCount(); i++)
	{
		_pointsNextFrame[i].x = (this->_shape.getPoint(i).x * c - this->_shape.getPoint(i).y * s) + this->_position.x + this->_velocity.x;
		_pointsNextFrame[i].y = (this->_shape.getPoint(i).x * s + this->_shape.getPoint(i).y * c) + this->_position.y + this->_velocity.y;
	}
}

void WireframeSprite::Draw(sf::RenderWindow* window)
{
	// Draw sprites
	window->draw(this->_shape);

	if (this->isHitboxVisible)// only draw hit-box if intended
		window->draw(this->hitboxShape);

}

void WireframeSprite::RotateMesh(float RotationDegrees)
{
	this->_shape.rotate(RotationDegrees);
}

void WireframeSprite::WrapCoordinates()
{
	sf::FloatRect rect = this->_shape.getGlobalBounds();

	if (rect.left + rect.width < 0.0f)
		this->_position += sf::Vector2f(rect.width + WINDOW_WIDTH, 0.0f);

	else if (rect.left >= WINDOW_WIDTH)
		this->_position += sf::Vector2f(-WINDOW_WIDTH - rect.width, 0.0f);

	else if (rect.top + rect.height < 0.0f)
		this->_position += sf::Vector2f(0.0f, WINDOW_HEIGHT + rect.height);

	else if (rect.top >= WINDOW_HEIGHT)
		this->_position += sf::Vector2f(0.0f, -WINDOW_HEIGHT - rect.height);


}
