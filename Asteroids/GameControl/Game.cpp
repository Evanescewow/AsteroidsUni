#include "Game.h"

#define _USE_MATH_DEFINES
#include <math.h>
Game::Game(sf::RenderWindow* window)
	:
	_window(window)
{
	// Seed Random Generation
	srand((unsigned int)time(NULL));

	// Generate player
	this->_player = new Player;


	// Create uniform grid
	this->_grid = new UniformGrid();
	

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

	// Update Asteroids
	this->UpdateAsteroids();

	// Update Bullets
	this->UpdateBullets();

	// Handle all collision logic
	this->UpdateCollisions();

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
	}

	// Input for toggling god mode
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		this->_collidePlayer = !this->_collidePlayer;

	// Input for toggling asteroid collisions
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
		this->_collideAsteroids = !this->_collideAsteroids;
}

void Game::UpdateCollisions()
{
	// Output variables sent back via reference from collision methods
	std::vector<int> asteroidsToBeSplit;
	bool isPlayerColliding = false;
	
	// reset number of collisions before calculations
	_nCollisionsThisFrame = 0;

	// brute force approach
	this->HandleSpriteCollision((this->_collisionMode == CollisionMode::AABB ? TestBoundingBoxCollision : TestSATCollision),
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

void Game::HandleSpriteCollision(std::function<bool(WireframeSprite&, WireframeSprite&)> collisionAlgorithm, std::vector<int>& asteroidsToBeSplit, bool& isPlayerColliding)
{
	int asteroidIndex = 0;

	for (auto itAsteroid = this->_asteroids.begin(); itAsteroid != this->_asteroids.end(); itAsteroid++, asteroidIndex++)
	{
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
				// Check for collision of pair
				if (collisionAlgorithm(**itAsteroid, **itAsteroidTwo))
				{
					this->_nCollisionsThisFrame++;
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

