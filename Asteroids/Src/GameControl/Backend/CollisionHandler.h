#pragma once
#include <vector>
#include <functional>
#include "SFML/Graphics.hpp"

// pre definitions to keep header file size small
class WireframeSprite;
class Asteroid;
class Bullet;
class Player;
class UniformGrid;
class QuadTree;

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
	UNIFORM_GRID,
	QUADTREE
};

class CollisionHandler
{
	// public methods
public:
	CollisionHandler(UniformGrid& grid, std::vector<Asteroid*>& asteroidContainer, std::vector<Bullet*>& bulletContainer,
		Player& player, QuadTree* quadtree);
	~CollisionHandler();

	CollisionPhaseData HandleCollision();

	void TogglePlayerCollision() { this->_collidePlayer = !this->_collidePlayer; }
	void ToggleBulletCollision() { this->_collideBullets = !this->_collideBullets; }
	void ToggleAsteroidCollision() { this->_collideAsteroids = !this->_collideAsteroids; }

	void SetBroadCollisionMode(BroadCollisionMode mode) { _broardCollisionMode = mode; }
	void SetNarrowCollisionMode(NarrowCollisionMode mode) { _narrowCollisionMode = mode; }

	void UpdateQuadTree(QuadTree* qt) { this->_quadTree = qt; }

	BroadCollisionMode GetBroadCollisionMode() const { return this->_broardCollisionMode; }

	// private methods
private:
	void HandleBroadPhaseBruteForce(std::function<bool(WireframeSprite&, WireframeSprite&)> collisionAlgorithm, bool& isPlayerColliding);
	void HandleBroadPhaseUniformGrid(std::function<bool(WireframeSprite&, WireframeSprite&)> collisionAlgorithm, bool& isPlayerColliding);
	void HandleBroadPhaseQuadtree(std::function<bool(WireframeSprite&, WireframeSprite&)> collisionAlgorithm, bool& isPlayerColliding);

	void CheckCollision(std::function<bool(WireframeSprite&, WireframeSprite&)> collisionAlgorithm,
		WireframeSprite* spriteA, std::vector<WireframeSprite*>& spritesToCheck, unsigned int startingIndex, bool& isPlayerColliding);

	// narrow phase algorithms
	static bool AABBCollision(WireframeSprite& spriteA, WireframeSprite& spriteB);
	static bool TestSATCollision(WireframeSprite& spriteA, WireframeSprite& spriteB);
	static bool BypassNarrowTest(WireframeSprite& spriteA, WireframeSprite& spriteB);

	// private members
private:
	// Collision mode options
	BroadCollisionMode _broardCollisionMode = BroadCollisionMode::UNIFORM_GRID;				// type of broad phase collision to be used
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
	QuadTree* _quadTree;
	Player& _player;								// reference to the player object

	// constants
	sf::Color ASTEROID_COLLISION_COLOR = sf::Color::Green;
};

