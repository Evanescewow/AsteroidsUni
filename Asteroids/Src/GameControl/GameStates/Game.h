#pragma once
#include "../../GameObjects/Player/Player.h"
#include "../../GameObjects/Other/Asteroid.h"
#include "../Common/Console.h"
#include "State.h"
#include "../Backend/CollisionHandler.h"
#include "../Backend/SpatialPartitioning/UniformGrid.h"
#include "../Backend/SpatialPartitioning/QuadTree.h"
#include "../../Global/ApplicationDefines.h"
#include <functional>
#include <vector>

class Game : public State
{
public:
	// Sets up the player, grid and asteroid objects. seeds the random function and 
	// assigns _renderWindow to the target render device passed to it.
	// takes event reference to hand off to the console class
	Game(sf::RenderWindow* window);

	// cleans up any dynamic memory
	~Game();

private:
	/*void ComposeFrame
	 * Brief:
	 * Handles drawing for all game objects
	*/
	void ComposeFrame() override;

	/*void UpdateModel
	 * Brief:
	 * Handles updates for all game objects. Also handles non class specific
	 * user input such as spawning bullets. Cleans up any game objects that
	 * should no longer be in the game. (bullets)
	*/
	void UpdateModel() override;

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

	/* void DrawGrid
	 * Brief:
	 * Checks if the user wishes to draw the spatial grid to the screen.
	 * if yes then the grid will be drawn depending on the active broad
	 * collision mode.
	 */
	void DrawGrid();

	/* void UpdateGrid
	 * Brief:
	 * Checks which grid system is active and updates it accordingly.
	 */
	void UpdateGrid();

	/* void UpdateSpriteGrid
	 * Brief:
	 *	Takes a sprite and calculates which grid in the uniform grid
	 *	system it should belong to. If that grid doesn't matches
	 *  the stored pointer to it's current grid, removes the sprite from that grid
	 *  and re adds it to the new cell.
	 * Params:
	 *	<wireframeSprite*> sprite	-	sprite to update in grid
	 */
	void UpdateSpriteGrid(WireframeSprite*sprite );

	/* void UpdateQuadTree
	 * Brief:
	 *	Updates the objects in the quadtree by first clearing it
	 *  then reconstructing it. This seems to be a reasonably efficient
	 *  solution to updating the positions of sprites within the system.
	 */
	void UpdateQuadTree();

	/* void HandleConsoleCommands
	 * Brief:
	 *	takes parsed command data from the console class
	 *	and performs command logic based upon that data in the form of a
	 *  switch.
	 * Params:
	 *	<ParsedCommandData> data	-	data to determine what actions to take
	 */
	void HandleConsoleCommands(Console::ParsedCommandData& data);

	/* void SpawnAsteroids
	 * Brief:
	 *	takes in n asteroids and spawns them into the game. firstly adds them to the
	 *  asteroid container followed by adding them to any present grid systems.
	 * Params:
	 *	<unsigned int> nAsteroidsToSpawn	-	number of asteroids to spawn
	 */
	void SpawnAsteroids(const unsigned int nAsteroidsToSpawn);

private:
	// Game Variables
	Player* _player = nullptr;						// player triangle object
	std::vector<Asteroid*> _asteroids;				// container for the asteroids
	std::vector<Bullet*> _bullets;					// container for the bullet objects
	size_t NUMBER_ASTEROIDS = 10;					// Number of asteroids on the screen

	sf::Clock _lastInputClock;						// Timer to limit spam of bullet firing			
	UniformGrid* _uniformGrid = nullptr;			// Grid for uniform spatial partitioning
	QuadTree* _quadTree = nullptr;
	CollisionHandler* _collisionHandler = nullptr;	// Handles all application collision logic
	Console* _console = nullptr;					// The internal console to the app

	bool _drawGrid = false;							// Should the spatial grid be drawn to the screen

	// Constants
	static constexpr float SHOOT_INTERVAL = 0.2f;	// interval in seconds between each bullet firing
	static constexpr int QUADTREE_CELL_LIMIT = 5;	// How many sprites can exist in a quad tree cell
													// Before it subdivides
};

