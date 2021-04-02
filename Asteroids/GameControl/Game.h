#pragma once
#include <SFML/Graphics.hpp>
#include "../GameObjects/Player.h"
#include "../GameObjects/Asteroid.h"
#include "CollisionHandler.h"
#include "UniformGrid.h"
#include <functional>
#include <vector>


class Game
{
public:
	// Sets up the player, grid and asteroid objects. seeds the random function and 
	// assigns _renderWindow to the target render device passed to it.
	Game(sf::RenderWindow* window);
	~Game();

	/*void Go
	 * Brief:
	 * Public run function for the game class. Each call to this function
	 * represents one game frame. This method is called in program entry point.
	*/
	void Go();


private:
	
	/*void ComposeFrame
	 * Brief:
	 * Handles drawing for all game objects
	*/
	void ComposeFrame();

	/*void UpdateModel
	 * Brief:
	 * Handles updates for all game objects. Also handles non class specific
	 * user input such as spawning bullets. Cleans up any game objects that
	 * should no longer be in the game. (bullets)
	*/
	void UpdateModel();

	/*void UpdateAsteroids
	 * Brief:
	 * Handles updates for all asteroids in the game
	*/
	void UpdateAsteroids();

	/*void UpdateAsteroids
	 * Brief:
	 * Handles updates for all bullets in the game
	*/
	void UpdateBullets();

	/*void UpdateAsteroids
	 * Brief:
	 * Deletes any bullets marked invisible internally. isVisible = false indicates
	 * the bullet has either collided with an asteroid or has escaped the bounds of the
	 * window.
	*/
	void CleanupBullets();
	

	/*void SplitAsteroid
	 * Brief:
	 * Takes an asteroid splits it. If the asteroid isn't the smallest size,
	 * the asteroid will split into two asteroids one scale smaller and the original 
	 * will be removed. If the asteroid is already the minimum size it will just be removed.
	 * Params:
	 * <int> asteroidIndex - Index of the target asteroid in the asteroid vector
	*/
	void SplitAsteroid(unsigned int asteroidIndex);

	/*void HandleInput
	 * Brief:
	 * Handles the input for any non class specific functions. The main purpose 
	 * is to handle the spawning of bullets.
	*/
	void HandleInput();

	void UpdateSpriteGrid(WireframeSprite*);

private:
	sf::RenderWindow* _window = nullptr;


	// Game Variables
	Player* _player = nullptr;						// player triangle object
	std::vector<Asteroid*> _asteroids;				// container for the asteroids
	std::vector<Bullet*> _bullets;					// container for the bullet objects
	size_t NUMBER_ASTEROIDS = 5;					// Number of asteroids on the screen

	bool _collidePlayer = true;						// Should tests for collision with the player be performed (godmode)
	bool _collideAsteroids = true;					// Should collision between asteroids be performed
	bool _collideBullets = true;					// should the bullets collide with the asteroids

	sf::Clock _lastInputClock;						// Timer to limit spam of bullet firing			
	UniformGrid* _grid = nullptr;					// Grid for uniform spatial partitioning
	CollisionHandler* _collisionHandler = nullptr;	// Handles all application collision logic

	// Constants
	static constexpr float SHOOT_INTERVAL = 0.2f;	// interval in seconds between each bullet firing
};

