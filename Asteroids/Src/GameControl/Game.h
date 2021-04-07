#pragma once
#include <SFML/Graphics.hpp>
#include "../GameObjects/Player/Player.h"
#include "../GameObjects/Other/Asteroid.h"
#include "Common/Console.h"
#include "State.h"
#include "Backend/CollisionHandler.h"
#include "Backend/SpatialPartitioning/UniformGrid.h"
#include "Backend/SpatialPartitioning/QuadTree.h"
#include "../Global/ApplicationDefines.h"
#include <functional>
#include <vector>

class Game : public State
{
public:
	// Sets up the player, grid and asteroid objects. seeds the random function and 
	// assigns _renderWindow to the target render device passed to it.
	// takes event reference to hand off to the console class
	Game(sf::RenderWindow* window);
	~Game();

	/*void Go
	 * Brief:
	 * Public run function for the game class. Each call to this function
	 * represents one game frame. This method is called in program entry point.
	*/
	GameState Go();


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

	void UpdateSpriteGrid(WireframeSprite*sprite );
	void UpdateQuadTree();

	void HandleConsoleCommands(Console::ParsedCommandData& data);

private:
	sf::RenderWindow* _window = nullptr;


	// Game Variables
	Player* _player = nullptr;						// player triangle object
	std::vector<Asteroid*> _asteroids;				// container for the asteroids
	std::vector<Bullet*> _bullets;					// container for the bullet objects
	size_t NUMBER_ASTEROIDS = 10;					// Number of asteroids on the screen

	sf::Clock _lastInputClock;						// Timer to limit spam of bullet firing			
	UniformGrid* _grid = nullptr;					// Grid for uniform spatial partitioning
	QuadTree* _quadTree = nullptr;
	CollisionHandler* _collisionHandler = nullptr;	// Handles all application collision logic
	Console* _console = nullptr;					// The interal console to the app

	bool _drawGrid = false;

	// Constants
	static constexpr float SHOOT_INTERVAL = 0.2f;	// interval in seconds between each bullet firing
	static constexpr int QUADTREE_CELL_LIMIT = 5;
};

