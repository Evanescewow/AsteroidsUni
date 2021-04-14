#define _USE_MATH_DEFINES
#include "WireframeSprite.h"
#include "../../Global/ApplicationDefines.h"
#include <math.h>

/* Constructor
 * Brief:
 *	assigns the properties to the base convex shape.
 *	also assigns properties to the hitbox shape. Key to note
 *	that the hitbox isn't updated or drawn unless it's flagged as
 *	active.
 * Params:
 *	<size_t> pointCount	-	Number of points the convex shape could contain
 */
WireframeSprite::WireframeSprite(size_t pointCount)
{
	this->_shape.setPointCount(pointCount);

	this->hitboxShape.setFillColor(sf::Color::Transparent);
	this->hitboxShape.setOutlineColor(sf::Color::Blue);
	this->hitboxShape.setOutlineThickness(HITBOX_THICKNESS);

	this->hitboxShape.setOutlineColor(sf::Color::Green);
}

WireframeSprite::~WireframeSprite(){}

/* virtual void Update
 * Brief:
 *	handles the update of the sprite object. sets the position of the sprite
 *	and hitbox in accordance with the private position member.
 *	updates container of internal points for the next frame. This is done to
 *	assist with predictive collision.
 * Method can be ovrriden by children to include extra functionality however
 * a call to the base method should always be included.
 */
void WireframeSprite::Update()
{
	// Update position of the convex shape
	this->_position += this->_velocity;
	this->_shape.setPosition(this->_position);


	//Update position for the hit-box display
	if (this->isHitboxVisible)
	{
		sf::FloatRect rect = this->_shape.getGlobalBounds();
		this->hitboxShape.setPosition(this->_shape.getPosition().x - rect.width / 2, this->_shape.getPosition().y - rect.height / 2);
		this->hitboxShape.setSize({ rect.width, rect.height });
	}
	// Update internal points
	float s = std::sin(((this->_shape.getRotation() * (float)M_PI) / 180.0f));
	float c = std::cos(((this->_shape.getRotation() * (float)M_PI) / 180.0f));
	for (unsigned int i = 0; i < this->_shape.getPointCount(); i++)
	{
		_pointsNextFrame[i].x = (this->_shape.getPoint(i).x * c - this->_shape.getPoint(i).y * s) + this->_position.x + this->_velocity.x;
		_pointsNextFrame[i].y = (this->_shape.getPoint(i).x * s + this->_shape.getPoint(i).y * c) + this->_position.y + this->_velocity.y;
	}
}


/* virtual void Draw
 * Brief:
 * Draws the convex shape to the given window. Also draws the surrounding
 * hitbox shape if it's enabled
 * Params:
 * <renderwindow*> window	-	window to draw the shapes to
 */
void WireframeSprite::Draw(sf::RenderWindow* window)
{
	// Draw sprites
	window->draw(this->_shape);

	if (this->isHitboxVisible)// only draw hit-box if intended
		window->draw(this->hitboxShape);

}

/* void RotateMesh
 * Brief:
 *	rotates the object's sprite by the passed value in degrees
 * Params:
 *	<float> RotateDegrees	-	degrees to rotate object by
 */
void WireframeSprite::RotateMesh(float RotationDegrees)
{
	this->_shape.rotate(RotationDegrees);
}

/* void WrapCoordinates
 * Brief:
 *	if called wraps the object to the screen.
 */
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
