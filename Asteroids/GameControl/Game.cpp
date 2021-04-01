#include "Game.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <algorithm>

Game::Game(sf::RenderWindow* window)
	:
	_window(window)
{
	// Seed Random Generation
	srand((unsigned int)time(NULL));

	// Create uniform grid
	this->_grid = new UniformGrid();

	// Generate player
	this->_player = new Player;
	_grid->AddObject(this->_player);

	// Generate asteroids
	for (unsigned int i = 0; i < NUMBER_ASTEROIDS; i++)
	{
		this->_asteroids.push_back(new Asteroid());
		_grid->AddObject(_asteroids.back());
	}
}

Game::~Game()
{
	// Cleanup grid
	if (this->_grid)
		delete this->_grid;

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

/*void Go
 * Brief:
 * Public run function for the game class. Each call to this function
 * represents one game frame. This method is called in program entry point.
*/
void Game::Go()
{
	this->_window->clear(sf::Color::Black);
	this->UpdateModel();
	this->ComposeFrame();
	this->_window->display();
}

/*void ComposeFrame
 * Brief:
 * Handles drawing for all game objects
*/
void Game::ComposeFrame()
{
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

	// Update Player
	_player->Update();
	UpdateSpriteGrid(_player);

	// Update Asteroids
	this->UpdateAsteroids();

	// Update Bullets
	this->UpdateBullets();

	// Handle all collision logic
	this->UpdateCollisions();

	// cleanup any bullets marked invisible
	this->CleanupBullets();

	std::cout << _nCollisionTestsThisFrame << std::endl;
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
		(*it)->Update(); // Call update on the asteroid

		// Check to see if the ball has changed cells
		Cell* newCell = &_grid->GetCell((*it)->GetPosition());
		if (newCell != (*it)->GetOwnerCell())
		{
			// update owner cell
			this->_grid->RemoveObject(*it);
			_grid->AddObject(*it, newCell);
		}
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
		Cell* newCell = &_grid->GetCell((*it)->GetPosition());
		if (newCell != (*it)->GetOwnerCell())
		{
			// update owner cell
			this->_grid->RemoveObject(*it);
			_grid->AddObject(*it, newCell);
		}
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
		this->_asteroids.push_back(new Asteroid(Asteroid::Size((int)asteroid->GetSize() + 1), pos, vel));
		this->_asteroids.push_back(new Asteroid(Asteroid::Size((int)asteroid->GetSize() + 1), pos, -vel));

		// Add the new asteroids to their correct cells
		this->_grid->AddObject(_asteroids[_asteroids.size()- 1]);
		this->_grid->AddObject(_asteroids[_asteroids.size() - 2]);
	}

	// clean up old asteroid
	this->_grid->RemoveObject(asteroid);	// Remove from the corresponding cell

	// Delete asteroid from memory
	delete asteroid;
	// Delete asteroid from the vector by copying then popping
	this->_asteroids[asteroidIndex] = this->_asteroids.back();
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
for (int i = 0; i < this->_bullets.size(); i++)
{
	// Check if bullet has disappeared from screen
	if (!this->_bullets[i]->IsVisible())
	{
		// Remove bullet from grid system
		this->_grid->RemoveObject(this->_bullets[i]);

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
	if (_lastInputClock.getElapsedTime().asSeconds() >= SHOOT_INTERVAL)
	{
		_lastInputClock.restart();
	}
	else
	{
		return;
	}

	// Input for firing bullets 
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		this->_bullets.push_back(new Bullet(this->_player->GetPosition(), this->_player->GetRotation()));
		_grid->AddObject(this->_bullets.back());
	}

	// Input for toggling god mode
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		this->_collidePlayer = !this->_collidePlayer;

	// Input for toggling asteroid collisions
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
		this->_collideAsteroids = !this->_collideAsteroids;

	// Input for toggling broad phase collision mode
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
		this->_broardCollisionMode = (this->_broardCollisionMode == BroadCollisionMode::BRUTE_FORCE ? BroadCollisionMode::UNIFORM_GRID : BroadCollisionMode::BRUTE_FORCE);
}

void Game::UpdateSpriteGrid(WireframeSprite* sprite)
{
	// Check to see if the ball has changed cells
	Cell* newCell = &_grid->GetCell((sprite)->GetPosition());
	if (newCell != (sprite)->GetOwnerCell())
	{
		// update owner cell
		this->_grid->RemoveObject(sprite);
		_grid->AddObject(sprite, newCell);
	}
}

void Game::UpdateCollisions()
{
	// Output variables sent back via reference from collision methods
	std::vector<int> asteroidsToBeSplit;
	bool isPlayerColliding = false;
	
	// reset number of collisions before calculations
	_nCollisionsThisFrame = 0;
	_nCollisionTestsThisFrame = 0;

	if (_broardCollisionMode == BroadCollisionMode::BRUTE_FORCE)
	// brute force approach
 		this->HandleSpriteCollisionBruteForce((this->_narrowCollisionMode == NarrowCollisionMode::AABB ? TestBoundingBoxCollision : TestSATCollision),
 		asteroidsToBeSplit, isPlayerColliding);

	else
		this->HandleSpriteCollisionUniformGrid((this->_narrowCollisionMode == NarrowCollisionMode::AABB ? TestBoundingBoxCollision : TestSATCollision),
			asteroidsToBeSplit, isPlayerColliding);

	// Split all asteroids where needed
	for (unsigned int i = 0; i < asteroidsToBeSplit.size(); i++)
	{
		SplitAsteroid(asteroidsToBeSplit[i]);
	}

	// temp player collision demonstration
	if (isPlayerColliding)
	{
		this->_player->SetColour(sf::Color::Yellow);
	}
	else
	{
		this->_player->SetColour(sf::Color::Cyan);
	}
}

void Game::HandleSpriteCollisionBruteForce(std::function<bool(WireframeSprite&, WireframeSprite&)> collisionAlgorithm, std::vector<int>& asteroidsToBeSplit, bool& isPlayerColliding)
{
	int asteroidIndex = 0;

	for (auto itAsteroid = this->_asteroids.begin(); itAsteroid != this->_asteroids.end(); itAsteroid++, asteroidIndex++)
	{
		_nCollisionTestsThisFrame++;// test for player
		// Player collision
		if (this->_collidePlayer)
			if (collisionAlgorithm(*this->_player, **itAsteroid))
			{
				this->_nCollisionsThisFrame++;
				isPlayerColliding = true;
			}
		// Bullet Collision
		if (this->_collideBullets)
			for (auto itBullet = this->_bullets.begin(); itBullet != this->_bullets.end(); itBullet++)
			{
				_nCollisionTestsThisFrame++; // collision tests for bullets
				// if asteroid colliding with bullet, add the index to the list of asteroids to be split after collision testing
				if (collisionAlgorithm(**itBullet, **itAsteroid))
				{
					this->_nCollisionsThisFrame++;
					asteroidsToBeSplit.push_back(asteroidIndex);
					// mark that bullet for cleanup now that it's hit something
					(*itBullet)->Disable();
				}
			}

		//AsteroidOnAsteroid Collision
		if (this->_collideAsteroids)
			for (auto itAsteroidTwo = itAsteroid + 1; itAsteroidTwo != this->_asteroids.end(); itAsteroidTwo++)
			{
				_nCollisionTestsThisFrame++; // collision tests for asteroids
				// Check for collision of pair
				if (collisionAlgorithm(**itAsteroid, **itAsteroidTwo))
				{
					this->_nCollisionsThisFrame++;
				}
			}
	}
}

void Game::HandleSpriteCollisionUniformGrid(std::function<bool(WireframeSprite&, WireframeSprite&)> collisionAlgorithm, std::vector<int>& asteroidsToBeSplit, bool& isPlayerColliding)
{
	auto cells = &this->_grid->_cells;

	// loop through all cells
	for (int i = 0; i < cells->size(); i++)
	{
		Cell* cell = &(*cells)[i];

		// get cell x and y
		int x = i % this->_grid->_numXCells;
		int y = i / this->_grid->_numXCells;

		// loop through cell objects
		for (int j = 0; j < cell->_objects.size(); j++)
		{
			WireframeSprite* sprite = cell->_objects[j];	// local pointer to current ball in current cell

			// update collisions with starting cell
			CheckCollision(sprite, cell->_objects, j + 1, collisionAlgorithm, asteroidsToBeSplit, isPlayerColliding);

			// update collision with neighbor cells
			if (x > 0)	// checks to the left
			{
				// check left
				CheckCollision(sprite, _grid->GetCell(x - 1, y)._objects, 0, collisionAlgorithm, asteroidsToBeSplit, isPlayerColliding);
				if (y > 0)
				{
					// check top left cell x-1 y-1
					CheckCollision(sprite, _grid->GetCell(x - 1, y - 1)._objects, 0, collisionAlgorithm, asteroidsToBeSplit, isPlayerColliding);
				}
				// Check bottom left
				if (y < _grid->_numYCells - 1)
				{
					CheckCollision(sprite, _grid->GetCell(x - 1, y + 1)._objects, 0, collisionAlgorithm, asteroidsToBeSplit, isPlayerColliding);
				}
			}
			// Check top
			if (y > 0)
			{
				CheckCollision(sprite, _grid->GetCell(x, y - 1)._objects, 0, collisionAlgorithm, asteroidsToBeSplit, isPlayerColliding);
			}
		}
	}
}

void Game::CheckCollision(WireframeSprite* spriteA, std::vector<WireframeSprite*>& spritesToCheck, unsigned int startingIndex,std::function<bool(WireframeSprite&, WireframeSprite&)> collisionAlgorithm, std::vector<int>& asteroidsToBeSplit, bool& isPlayerColliding)
{
	for (unsigned int i = startingIndex; i < spritesToCheck.size(); i++)
	{
		// test being performed, increase number for data collection
		this->_nCollisionTestsThisFrame++;
		if (collisionAlgorithm(*spriteA, *spritesToCheck[i]))
		{
			// Collision happened, increase number for data collection
			this->_nCollisionsThisFrame++;

			WireframeSprite* pSpriteA = spriteA;
			WireframeSprite* pSpriteB = spritesToCheck[i];

			// loop through to check in both directions
			for (int l = 0; l < 2; l++)
			{
				// on 2nd iteration swap pointers
				if (l == 1)
				{
					pSpriteA = spritesToCheck[i];
					pSpriteB = spriteA;
				}

				const char* spriteAtype = typeid(*pSpriteA).name();
				const char* spriteBtype = typeid(*pSpriteB).name();

				// Test for player collision
				if (this->_collidePlayer)
					if (std::strcmp(spriteAtype, "class Player") == 0 && std::strcmp(spriteBtype, "class Asteroid") == 0)
					{
						isPlayerColliding = true;
					}

				// Test for bullet against asteroid collision
				if (this->_collideBullets)
					if (std::strcmp(spriteAtype, "class Bullet") == 0 && std::strcmp(spriteBtype, "class Asteroid") == 0)
					{
						// find index in array for deletion
						auto itr = std::find(this->_asteroids.begin(), this->_asteroids.end(), pSpriteB);
						int index = std::distance(this->_asteroids.begin(), itr);

						// clean up bullet after hitting an asteroid
						Bullet* bulletToDestroy = dynamic_cast<Bullet*>(pSpriteA);
						if (bulletToDestroy)
							bulletToDestroy->Disable();

						// mark the asteroid to be split
						asteroidsToBeSplit.push_back(index);
					}

				// Implement asteroid collision
			}
		}
	}
}

bool Game::TestBoundingBoxCollision(WireframeSprite& spriteA, WireframeSprite& spriteB)
{
	// Check for simple bounding box collision
	if (spriteA.GetBoundingRectangle().intersects(spriteB.GetBoundingRectangle()))
	{
		return true;
	}

	return false;
}
bool Game::TestSATCollision(WireframeSprite& spriteA, WireframeSprite& spriteB)
{
	WireframeSprite* pSpriteA = &spriteA;
	WireframeSprite* pSpriteB = &spriteB;
	float overlap = INFINITY;

	// Loop over both shapes given
	for (int i = 0; i < 2; i++)
	{
		// invert the order of operations on the 2nd iteration
		if (i == 1)
		{
			pSpriteA = &spriteB;
			pSpriteB = &spriteA;
		}
		// Create references of the points belonging to the sprites (translated)
		auto spriteAPoints = pSpriteA->GetPoints();
		auto spriteBPoints = pSpriteB->GetPoints();

		// Loop through points and test the collisions against them
		for (unsigned int a = 0; a < spriteAPoints.size(); a++)
		{
			int b = (a + 1) % spriteAPoints.size();

			// calculate the projected axis points
			sf::Vector2f axisProj = { -(spriteAPoints[b].y - spriteAPoints[a].y), spriteAPoints[b].x - spriteAPoints[a].x };

			// Normalize the axis values for better stability
			float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
			axisProj = { axisProj.x / d, axisProj.y / d };

			// Work out min and max 1D points for sprite 1
			float min_sprite1 = INFINITY, max_sprite1 = -INFINITY;
			for (unsigned int p = 0; p < spriteAPoints.size(); p++)
			{
				float q = (spriteAPoints[p].x * axisProj.x + spriteAPoints[p].y * axisProj.y);
				min_sprite1 = std::min(min_sprite1, q);
				max_sprite1 = std::max(max_sprite1, q);
			}

			// Work out min and max 1D points for sprite 2
			float min_sprite2 = INFINITY, max_sprite2 = -INFINITY;
			for (unsigned int p = 0; p < spriteBPoints.size(); p++)
			{
				float q = (spriteBPoints[p].x * axisProj.x + spriteBPoints[p].y * axisProj.y);
				min_sprite2 = std::min(min_sprite2, q);
				max_sprite2 = std::max(max_sprite2, q);
			}

			

			if (!(max_sprite2 >= min_sprite1 && max_sprite1 >= min_sprite2))
				return false;
		}
	}

	return true;
}

