#pragma once
#include "../Base/WireframeSprite.h"

class Asteroid :
	public WireframeSprite
{
public:
	enum class Size {
		LARGE,
		MEDIUM,
		SMALL
	};
public:
	/* Constructor
	 * Brief:
	 * Assigns basic object variables such as outline thickness and color. After which it will build the shape
	 * and generate the initial speed and position variables.
	 * Params:
	 * <uint> arrayIndex - index of itself within the game array
	 * <int> size - size of the new asteroid (defaults to large)
	 * <Vector2f> pos - position vector for the new asteroid if not random (defaults 0)
	 * <Vector2f> velocity - velocity vector for the new asteroid if not random (defaults 0)
	*/
	Asteroid(unsigned int arrayIndex, Size size = Size::LARGE, sf::Vector2f pos = { 0,0 }, sf::Vector2f velocity = { 0,0 });
	~Asteroid();

	/*void Update override
	 * Brief:
	 * Overrides WireframeSprite::Update(). Updates the asteroid's rotation and wraps the object
	 * around the screen if needed.
	*/
	void Update() override;

	Size GetSize() const { return this->_size; } // Getter for the asteroid size
	unsigned int GetObjectContainerIndex() const { return this->_indexInGameObjectArray; }	// getter for object index
	void UpdateObjectContainerIndex(unsigned int index) { this->_indexInGameObjectArray = index; }

	// Getter and setter for asteroid splitting
	bool ShouldSplit() const { return this->_shouldBeSplitNextFrame; }
	void MarkForSplitting() { this->_shouldBeSplitNextFrame = true; }


private:
	/* void BuildShape
	 * Brief:
	 * Generates verticies around a circle given a selected radius. Angle between verticies have a minimum
	 * and maximum value. After generation of verticies, convex shape is created.
	*/
	void BuildShape();

	/* void GenerateRandomVariables
	 * Brief:
	 * Generates the random variables required for a new asteroid. These include position and velocity.
	 * asteroids will never spawn on top of the player spawn.
	 * Speed has minimums and maximums.
	*/
	void GenerateRandomVariables();

	/* void RandomFloat
	 * Brief:
	 * Generates a random float between two given values. This value is returned.
	 * Params:
	 * float a - lower bound for random generation
	 * float b - upper bound for random generation
	 * Returns:
	 * <Float> Result of random generation
	*/
	float RandomFloat(float a, float b);

private:
	// Constants
	static constexpr float ROTATION_SPEED = 1.0f;		// Speed of asteroid spin
	static const int ASTEROID_VERTS = 11;				// Number of verticies on the asteroid
	sf::Color ASTEROID_COLOR = sf::Color::Red;			// color of the asteroid
	static constexpr float ASTEROID_THICKNESS = 1;		// Thickness of the asteroid outline
	static constexpr double MINIMUM_VERT_ANGLE = 0.1;	// Minimum angle between verticies (radians)
	static constexpr float MAXIMUM_SPEED = 0.0f;		// Maximum spawn speed		// Minimum spawn speed

	bool _shouldBeSplitNextFrame = false;				// is the asteroid marked to be split
	unsigned int _indexInGameObjectArray = 0;			// Stores the index of itself within the game array
														// This is a performance enhancement to cut back searches

	// Member Variables
	static std::map<Size, int> _radi;					// Contains the different radius sizes
	Size _size;											// Size of the asteroid
};

