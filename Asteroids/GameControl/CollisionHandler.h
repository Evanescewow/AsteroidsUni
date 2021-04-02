#pragma once
#include <vector>
#include <functional>
#include <SFML/Graphics.hpp>

// pre definitions to keep header file size small
class WireframeSprite;
class Asteroid;
class Bullet;
class Player;
class UniformGrid;


struct CollisionPhaseData
{
	unsigned int nCollisions = 0;
	unsigned int nCollisionTests = 0;
	bool isPlayerColliding = false;
};

enum class NarrowCollisionMode
{
	AABB,
	SEPERATED_AXIS_THEOREM
};

enum class BroadCollisionMode
{
	BRUTE_FORCE,
	UNIFORM_GRID
};

class CollisionHandler
{
	// public methods
public:
	CollisionHandler(UniformGrid& grid, std::vector<Asteroid*>& asteroidContainer, std::vector<Bullet*>& bulletContainer, Player& player);
	~CollisionHandler();

	CollisionPhaseData HandleCollision();

	// private methods
private:
	void HandleBroadPhaseBruteForce(std::function<bool(WireframeSprite&, WireframeSprite&)> collisionAlgorithm, bool& isPlayerColliding);
	void HandleBroadPhaseUniformGrid(std::function<bool(WireframeSprite&, WireframeSprite&)> collisionAlgorithm, bool& isPlayerColliding);

	void CheckCollision(std::function<bool(WireframeSprite&, WireframeSprite&)> collisionAlgorithm,
		WireframeSprite* spriteA, std::vector<WireframeSprite*>& spritesToCheck, unsigned int startingIndex, bool& isPlayerColliding);

	// narrow phase algorithms
	static bool AABBCollision(WireframeSprite& spriteA, WireframeSprite& spriteB);
	static bool TestSATCollision(WireframeSprite& spriteA, WireframeSprite& spriteB);

	// private members
private:
	// Collision mode options
	BroadCollisionMode _broardCollisionMode = BroadCollisionMode::BRUTE_FORCE;				// type of broad phase collision to be used
	NarrowCollisionMode _narrowCollisionMode = NarrowCollisionMode::SEPERATED_AXIS_THEOREM;	// Type of narrow phase collision to be used

	// Collision performance options
	bool _collidePlayer = true;						// Should tests for collision with the player be performed (godmode)
	bool _collideAsteroids = true;					// Should collision between asteroids be performed
	bool _collideBullets = true;					// should the bullets collide with the asteroids

	// Collision counters
	unsigned int _nCollisionsThisFrame = 0;			// Amount of collisions on the current frame
	unsigned int _nCollisionTestsThisFrame = 0;		// Amount of collision tests performed on the current frame

	// game object container references
	std::vector<Asteroid*>& _asteroids;				// references to all the asteroids in the game
	std::vector<Bullet*>& _bullets;					// reference to all the bullets in the game
	UniformGrid& _grid;
	Player& _player;								// reference to the player object

	// constants
	sf::Color ASTEROID_COLLISION_COLOR = sf::Color::Green;
};

