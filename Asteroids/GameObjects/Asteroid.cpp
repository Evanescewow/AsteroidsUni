#define _USE_MATH_DEFINES
#include <MAth.h>
#include "Asteroid.h"
#include "../Global/ApplicationDefines.h"
#include "../GameControl/UniformGrid.h"
#include <ctime>

// Contains the different radius sizes
std::map<Asteroid::Size, int> Asteroid::_radi = { { Asteroid::Size::LARGE, 55 }, { Asteroid::Size::MEDIUM, 32 }, { Asteroid::Size::SMALL, 12 } };


/* Constructor
 * Brief:
 * Assigns basic object variables such as outline thickness and color. After which it will build the shape
 * and generate the initial speed and position variables.
 * Params:
 * <int> size - size of the new asteroid (defaults to large)
 * <Vector2f> pos - position vector for the new asteroid if not random (defaults 0)
 * <Vector2f> velocity - velocity vector for the new asteroid if not random (defaults 0)
*/
Asteroid::Asteroid(Size size, sf::Vector2f pos, sf::Vector2f velocity)
	:
	WireframeSprite(ASTEROID_VERTS),
	_size((Asteroid::Size)size)
{
	// Setup basic properties
	this->_shape.setFillColor(sf::Color::Transparent);
	this->_shape.setOutlineColor(ASTEROID_COLOR);
	this->_shape.setOutlineThickness(ASTEROID_THICKNESS);
	this->hitboxShape.setOutlineColor(sf::Color::Green);

	// Generates random points around a circle to form asteroid
	this->BuildShape();

	// Check if asteroid is a new spawn or a split
	if (pos == sf::Vector2f{ 0, 0 } && velocity == sf::Vector2f{0, 0})
	{
		// Generate random variables for new asteroid
		this->GenerateRandomVariables();
	}
	else
	{
		// assign known position and velocity for split asteroid
		this->_velocity = velocity;
		this->_position = pos;
	}

}

Asteroid::~Asteroid()
{
}

/*void Update override
 * Brief:
 * Overrides WireframeSprite::Update(). Updates the asteroid's rotation and wraps the object
 * around the screen if needed.
*/
void Asteroid::Update()
{
	// Update asteroid rotation
	this->RotateMesh(ROTATION_SPEED);

	// Call to base update for position update
	WireframeSprite::Update();

	// Wrap asteroids to screen
	this->WrapCoordinates();
}

/* void BuildShape
 * Brief:
 * Generates verticies around a circle given a selected radius. Angle between verticies have a minimum
 * and maximum value. After generation of verticies, convex shape is created.
*/
void Asteroid::BuildShape()
{
	std::vector<float> points;

	// Randomly generate points for the new asteroid's shape then sort
	for (int i = 0; i < ASTEROID_VERTS; i++)
	{
		points.push_back(static_cast<float>((double)rand() * 2 * M_PI / ((double)RAND_MAX + 1)));
	}
	std::sort(points.begin(), points.end());

	// Generate the coordinates for the new asteroid's shape
	for (int i = 0; i < ASTEROID_VERTS; i++)
	{
		sf::Vector2f point = { sf::Vector2f(_radi[_size] * sin(points.at(i)),
			_radi[_size] * cos(points.at(i))) };

		this->_shape.setPoint(i, point);
		this->_pointsNextFrame.push_back(point);
	}
}

/* void GenerateRandomVariables
 * Brief:
 * Generates the random variables required for a new asteroid. These include position and velocity.
 * asteroids will never spawn on top of the player spawn.
 * Speed has minimums and maximums.
*/
void Asteroid::GenerateRandomVariables()
{
	// Variables to generate
	float posX, posY, velAngle;

	// Variables to test against spawn deadzone
	float thirdWidth = WINDOW_WIDTH / 3;
	float thirdHeight = WINDOW_HEIGHT / 3;

	// Generate the position and velocity for the new asteroid
	// If generated coordinates are in the middle third of the screen, will regenerate to avoid
	// instant collision with player
	do 
	{
		posX = static_cast<float>(rand() % WINDOW_WIDTH + 1);
	} while (posX > thirdWidth && posX < thirdWidth*2);
	do 
	{
		posY = static_cast<float>(rand() % WINDOW_HEIGHT + 1);
	} while (posY > thirdHeight && posY < thirdHeight*2);

	// Generate velocity values
	velAngle = RandomFloat(0, 2*M_PI);


	// Set the position and velocity
	this->_position = { posX, posY };
	this->_velocity = { MAXIMUM_SPEED * sin(velAngle), MAXIMUM_SPEED * cos(velAngle) };
}

/* void RandomFloat
 * Brief:
 * Generates a random float between two given values. This value is returned.
 * Params:
 * float a - lower bound for random generation
 * float b - upper bound for random generation
 * Returns:
 * <Float> Result of random generation
*/
float Asteroid::RandomFloat(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}
