#include "Game.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <iostream>

Game::Game(sf::RenderWindow* window)
	:
	State(window, GameState::GAME)
{
	// Seed Random Generation
	srand((unsigned int)time(NULL));

	// Create uniform grid
	this->_uniformGrid = new UniformGrid();

	// Create uniform grid
	this->_quadTree = new QuadTree(sf::FloatRect(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, WINDOW_WIDTH/2, WINDOW_HEIGHT/2), QUADTREE_CELL_LIMIT);

	// Generate player
	this->_player = new Player;
	_uniformGrid->AddObject(this->_player);
	_quadTree->AddObject(this->_player);

	// Generate asteroids
	for (unsigned int i = 0; i < NUMBER_ASTEROIDS; i++)
	{
		this->_asteroids.push_back(new Asteroid(i));
		_uniformGrid->AddObject(_asteroids.back());
		_quadTree->AddObject(_asteroids.back());
	}

	// Create collision handler
	this->_collisionHandler = new CollisionHandler(*_uniformGrid, _asteroids, _bullets, *_player, _quadTree);

	// Create console
	this->_console = new Console();
}

Game::~Game()
{
	// Cleanup console
	if (this->_console)
		delete this->_console;

	// Cleanup grid
	if (this->_uniformGrid)
		delete this->_uniformGrid;

	// Cleanup quadtree
	if (this->_quadTree)
		delete this->_quadTree;

	// Cleanup collisionHandler
	if (this->_collisionHandler)
		delete this->_collisionHandler;

	// iterate over all asteroids as there could be more than originally started
	for (auto it = this->_asteroids.begin(); it != this->_asteroids.end(); it++)
	{
		delete* it;
	}

	// clean up any bullets
	for (auto it = this->_bullets.begin(); it != this->_bullets.end(); it++)
	{
		delete* it;
	}
}



/*void ComposeFrame
 * Brief:
 * Handles drawing for all game objects
*/
void Game::ComposeFrame()
{
	// draw the grid to screen if enabled
	this->DrawGrid();

	// Draw Player
	_player->Draw(_window);

	// Draw Asteroids
	for (auto it = this->_asteroids.begin(); it != this->_asteroids.end(); it++)
	{
		(*it)->Draw(_window);
	}

	// Draw bullets
	for (auto it = this->_bullets.begin(); it != this->_bullets.end(); it++)
	{
		(*it)->Draw(_window);
	}


	// Draw the console if it's open
	if (this->_console->IsOpen())
		this->_console->Draw(_window);

}

/*void UpdateModel
 * Brief:
 * Handles updates for all game objects. Also handles non class specific
 * user input such as spawning bullets. Cleans up any game objects that
 * should no longer be in the game. (bullets)
*/
void Game::UpdateModel()
{
	// Handle input (for the firing of bullets
	this->HandleInput();


	// If console is open pause the rest of the operations
	if (this->_console->IsOpen())
	{
		Console::ParsedCommandData data = this->_console->Update(this->_window);
		this->HandleConsoleCommands(data);
		return;
	}

	// Update Player
	_player->Update();

	// Update Asteroids
	this->UpdateAsteroids();

	// Update Bullets
	this->UpdateBullets();

	this->UpdateGrid();

	// Handle all collision logic
	CollisionPhaseData data = this->_collisionHandler->HandleCollision();

	// temp player collision demonstration
	if (data.isPlayerColliding)
		this->_player->SetColour(sf::Color::Yellow);
	else
		this->_player->SetColour(sf::Color::Cyan);

	std::cout << data.nCollisionTests << std::endl;

	// cleanup any bullets marked invisible
	this->CleanupBullets();
}

/*void UpdateAsteroids
 * Brief:
 * Handles updates for all asteroids in the game
*/
void Game::UpdateAsteroids()
{
	// Update Asteroids
	for (auto it = this->_asteroids.begin(); it != this->_asteroids.end(); it++)
	{
		// Split asteroid if needed
		if ((*it)->ShouldSplit())
		{
			// store current index so pointer can be reassigned after deletion
			// iterator maintains in the same place as split asteroid does a swap delete
			unsigned int currentIndex = (*it)->GetObjectContainerIndex();
			this->SplitAsteroid(currentIndex);
			it = _asteroids.begin() + currentIndex;

			// Check to make sure iterator doesn't go out of range
			if (it >= this->_asteroids.end())
				return;
		}

		(*it)->Update(); // Call update on the asteroid

		// Check to see if the ball has changed cells
		if (this->_collisionHandler->GetBroadCollisionMode() == BroadCollisionMode::UNIFORM_GRID)
			UpdateSpriteGrid(*it);
	}
}

/*void UpdateAsteroids
 * Brief:
 * Deletes any bullets marked invisible internally. isVisible = false indicates
 * the bullet has either collided with an asteroid or has escaped the bounds of the
 * window.
*/
void Game::UpdateBullets()
{
	// Update bullets
	for (auto it = this->_bullets.begin(); it != this->_bullets.end(); it++)
	{
		(*it)->Update(); // call update on the bullet


		// Check to see if the ball has changed cells
		if (this->_collisionHandler->GetBroadCollisionMode() == BroadCollisionMode::UNIFORM_GRID)
			UpdateSpriteGrid(*it);
	}
}


/*void SplitAsteroid
 * Brief:
 * Takes an asteroid splits it. If the asteroid isn't the smallest size,
 * the asteroid will split into two asteroids one scale smaller and the original
 * will be removed. If the asteroid is already the minimum size it will just be removed.
 * Params:
 * <int> asteroidIndex - Index of the target asteroid in the asteroid vector
*/
void Game::SplitAsteroid(unsigned int asteroidIndex)
{

	// Check for out of range
	if (asteroidIndex >= this->_asteroids.size())
		return;

	// pointer to asteroid to split
	Asteroid* asteroid = this->_asteroids[asteroidIndex];

	sf::Vector2f pos = asteroid->GetPosition();
	sf::Vector2f vel = { asteroid->GetVelocity().x * 2, asteroid->GetVelocity().y * 2 };
	// Velocity is scaled by a factor of two upon each split

	// Check if asteroid should split or just disappear
	if (asteroid->GetSize() != Asteroid::Size::SMALL)
	{
		// Push back new asteroids with smaller size
		this->_asteroids.push_back(new Asteroid(this->_asteroids.size(), Asteroid::Size((int)asteroid->GetSize() + 1), pos, vel));
		this->_asteroids.push_back(new Asteroid(this->_asteroids.size(), Asteroid::Size((int)asteroid->GetSize() + 1), pos, -vel));

		// Add the new asteroids to their correct cells
		this->_uniformGrid->AddObject(_asteroids[_asteroids.size()- 1]);
		this->_uniformGrid->AddObject(_asteroids[_asteroids.size() - 2]);

		// Add the objects to their correct quadtree
		this->_quadTree->AddObject(_asteroids[_asteroids.size() - 1]);
		this->_quadTree->AddObject(_asteroids[_asteroids.size() - 2]);
	}

	// clean up old asteroid
	this->_uniformGrid->RemoveObject(asteroid);	// Remove from the corresponding cell

	// Delete asteroid from memory
	delete asteroid;
	// Delete asteroid from the vector by copying then popping
	this->_asteroids[asteroidIndex] = this->_asteroids.back();
	this->_asteroids[asteroidIndex]->UpdateObjectContainerIndex(asteroidIndex);
	this->_asteroids.pop_back();

}

/*void UpdateAsteroids
 * Brief:
 * Deletes any bullets marked invisible internally. isVisible = false indicates
 * the bullet has either collided with an asteroid or has escaped the bounds of the
 * window.
*/
void Game::CleanupBullets()
{
for (unsigned int i = 0; i < this->_bullets.size(); i++)
{
	// Check if bullet has disappeared from screen
	if (!this->_bullets[i]->IsVisible())
	{
		// Remove bullet from grid system
		this->_uniformGrid->RemoveObject(this->_bullets[i]);
		this->_quadTree->RemoveObject(this->_bullets[i]);

		// Delete bullet and remove entry from vector
		delete this->_bullets[i];
		this->_bullets.erase(this->_bullets.begin() + i);
	}
}
}

/*void HandleInput
 * Brief:
 * Handles the input for any non class specific functions. The main purpose
 * is to handle the spawning of bullets.
*/
void Game::HandleInput()
{
	// Limit key spam
	if (_lastInputClock.getElapsedTime().asSeconds() <= SHOOT_INTERVAL)
	{
		return;
	}
	
	// flag for if an input has been given
	bool hasKeyBeenPressed = false;

	// Check for console toggle
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
	{
		this->_console->Toggle();
		hasKeyBeenPressed = true;	// to see if the clock should be reset
	}

	// Check for escape to menu
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		this->_nextState = GameState::MENU;
		hasKeyBeenPressed = true;
	}

	// Only allow regular keys when console is closed
	if (!this->_console->IsOpen())
	{
		// Input for firing bullets 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			// create a new bullet and add it to the grid systems
			this->_bullets.push_back(new Bullet(this->_player->GetPosition(), this->_player->GetRotation()));
			_uniformGrid->AddObject(this->_bullets.back());
			_quadTree->AddObject(this->_bullets.back());
			// to see if the clock should be reset
			hasKeyBeenPressed = true;
		}
	}

	// if user pressed a binding reset the cooldown timer
	if (hasKeyBeenPressed)
		_lastInputClock.restart();
}

/* void DrawGrid
 * Brief:
 * Checks if the user wishes to draw the spatial grid to the screen.
 * if yes then the grid will be drawn depending on the active broad
 * collision mode.
 */
void Game::DrawGrid()
{
	// Draw quadtree
	if (this->_drawGrid)
		switch (_collisionHandler->GetBroadCollisionMode())
		{
			// Draw the quad tree
		case(BroadCollisionMode::QUADTREE):
			this->_quadTree->Draw(_window);
			break;

			// draw the uniformgrid
		case(BroadCollisionMode::UNIFORM_GRID):
			this->_uniformGrid->Draw(_window);
			break;
		}
}

/* void UpdateGrid
 * Brief:
 * Checks which grid system is active and updates it accordingly.
 */
void Game::UpdateGrid()
{
	// only update the active grid so it doesn't slow down the other

	switch (this->_collisionHandler->GetBroadCollisionMode())
	{
		// update uniform grid
	case BroadCollisionMode::UNIFORM_GRID:
		UpdateSpriteGrid(_player);
		break;

		// update quadtree
	case BroadCollisionMode::QUADTREE:
		this->UpdateQuadTree();
		break;
	}
}

/* void UpdateSpriteGrid
 * Brief:
 *	Takes a sprite and calculates which grid in the uniform grid
 *	system it should belong to. If that grid doesn't matches
 *  the stored pointer to it's current grid, removes the sprite from that grid
 *  and re adds it to the new cell.
 * Params:
 *	<wireframeSprite*> sprite	-	sprite to update in grid
 */
void Game::UpdateSpriteGrid(WireframeSprite* sprite)
{

	// do nothing if not the right collision mode
	if (this->_collisionHandler->GetBroadCollisionMode() != BroadCollisionMode::UNIFORM_GRID)
		return;

	// Check to see if the ball has changed cells
	Cell* newCell = &_uniformGrid->GetCell((sprite)->GetPosition());
	if (newCell != (sprite)->GetOwnerCell())
	{
		// update owner cell
		this->_uniformGrid->RemoveObject(sprite);
		_uniformGrid->AddObject(sprite, newCell);
	}
}

/* void UpdateQuadTree
 * Brief:
 *	Updates the objects in the quadtree by first clearing it
 *  then reconstructing it. This seems to be a reasonably efficient
 *  solution to updating the positions of sprites within the system.
 */
void Game::UpdateQuadTree()
{
	// do nothing if not the right collision mode
	if (this->_collisionHandler->GetBroadCollisionMode() != BroadCollisionMode::QUADTREE)
		return;

	// Delete the old tree
	this->_quadTree->Clear();

	// add player to tree
	this->_quadTree->AddObject(_player);

	// add asteroids to tree
	for (auto it = this->_asteroids.begin(); it != this->_asteroids.end(); it++)
	{
		this->_quadTree->AddObject(*it);
	}

	// add bullets to tree
	for (auto it = this->_bullets.begin(); it != this->_bullets.end(); it++)
	{
		this->_quadTree->AddObject(*it);
	}
}

/* void HandleConsoleCommands
 * Brief:
 *	takes parsed command data from the console class
 *	and performs command logic based upon that data in the form of a
 *  switch.
 * Params:
 *	<ParsedCommandData> data	-	data to determine what actions to take
 */
void Game::HandleConsoleCommands(Console::ParsedCommandData& data)
{
	switch (data.commandType)
	{

		// Toggle player Collision
	case (Console::CommandType::TOGGLE_PLAYER_COLLISION):
		this->_collisionHandler->TogglePlayerCollision();
		break;

		// Toggle Asteroid Collision
	case (Console::CommandType::TOGGLE_ASTEROID_COLLISION):
		this->_collisionHandler->ToggleAsteroidCollision();
		break;

		// Toggle Bullet Collision
	case (Console::CommandType::TOGGLE_BULLET_COLLISION):
		this->_collisionHandler->ToggleBulletCollision();
		break;

		// Toggle Draw grid
	case (Console::CommandType::TOGGLE_DRAW_GRID):
		this->_drawGrid = !this->_drawGrid;
		break;
		
		// Set col-broad uniformgrid
	case (Console::CommandType::SET_BROAD_COLLISION_UNIFORM_GRID):
		this->_collisionHandler->SetBroadCollisionMode(BroadCollisionMode::UNIFORM_GRID);
		break;

		// Set col-broad bruteforce
	case (Console::CommandType::SET_BROAD_COLLISION_BRUTE_FORCE):
		this->_collisionHandler->SetBroadCollisionMode(BroadCollisionMode::BRUTE_FORCE);
		break;

		// Set col-broad quadtree
	case (Console::CommandType::SET_BROAD_COLLISION_QUADTREE):
		this->_collisionHandler->SetBroadCollisionMode(BroadCollisionMode::QUADTREE);
		break;

		// Set col-narrow AABB
	case (Console::CommandType::SET_NARROW_COLLISION_AABB):
		this->_collisionHandler->SetNarrowCollisionMode(NarrowCollisionMode::AABB);
		break;

		// Set col-narrow AABB
	case (Console::CommandType::SET_NARROW_COLLISION_SAT):
		this->_collisionHandler->SetNarrowCollisionMode(NarrowCollisionMode::SEPERATED_AXIS_THEOREM);
		break;

		// Spawn asteroids
	case (Console::CommandType::SPAWN_ASTEROID):
		this->SpawnAsteroids(static_cast<unsigned int>(data.additionalValue));
		break;

	case (Console::CommandType::INVALID_COMMAND):
		break;

	//default:
		//throw std::exception("Error processing console command.");
		//break;
	}
}

/* void SpawnAsteroids
 * Brief:
 *	takes in n asteroids and spawns them into the game. firstly adds them to the
 *  asteroid container followed by adding them to any present grid systems.
 * Params:
 *	<unsigned int> nAsteroidsToSpawn	-	number of asteroids to spawn
 */
void Game::SpawnAsteroids(const unsigned int nAsteroidsToSpawn)
{
	// Loop through n asteroids to spawn for asteroid spawning
	for (unsigned int i = 0; i < nAsteroidsToSpawn; i++)
	{
		// add the new asteroid to the cointainer and add to the grid systems
		this->_asteroids.push_back(new Asteroid(this->_asteroids.size()));
		this->_uniformGrid->AddObject(this->_asteroids.back());
		this->_quadTree->AddObject(this->_asteroids.back());
	}
}
