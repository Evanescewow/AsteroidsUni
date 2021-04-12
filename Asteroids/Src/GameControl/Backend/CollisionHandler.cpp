#include "CollisionHandler.h"
#include "../../GameObjects/Base/WireframeSprite.h"
#include "../../GameObjects/Player/Player.h"
#include "../../GameObjects/Other/Asteroid.h"
#include "../../GameObjects/Other/Bullet.h"
#include "SpatialPartitioning/QuadTree.h"
#include "SpatialPartitioning/UniformGrid.h"
#include "../../Global/ApplicationDefines.h"

CollisionHandler::CollisionHandler(UniformGrid& grid, std::vector<Asteroid*>& asteroidContainer, std::vector<Bullet*>& bulletContainer, Player& player, QuadTree* quadtree)
	:	// Assign references to game objects
	_grid(grid),
	_asteroids(asteroidContainer),
	_bullets(bulletContainer),
	_player(player),
	_quadTree(quadtree){}

CollisionHandler::~CollisionHandler(){}

CollisionPhaseData CollisionHandler::HandleCollision()
{
	// output data
	CollisionPhaseData output = CollisionPhaseData();
	this->_nCollisionsThisFrame = 0;
	this->_nCollisionTestsThisFrame = 0;

	// Setup output variable to handle player collision stuff
	bool isPlayerColliding = false;

	// Decide what narrow phase detection to use
	std::function<bool(WireframeSprite&, WireframeSprite&)> narrowCollisionMode;

	switch (this->_narrowCollisionMode)
	{
	case NarrowCollisionMode::AABB:
		narrowCollisionMode = AABBCollision;
		break;
	case NarrowCollisionMode::SEPERATED_AXIS_THEOREM:
		narrowCollisionMode = TestSATCollision;
		break;

	default:
		throw std::exception("unknown narrow phase collision detected!");
	}


	// branch to chosen broad phase mode
	switch (this->_broardCollisionMode)
	{
	case BroadCollisionMode::BRUTE_FORCE:
	
		this->HandleBroadPhaseBruteForce(narrowCollisionMode, isPlayerColliding);
		break;
	case BroadCollisionMode::UNIFORM_GRID:
		this->HandleBroadPhaseUniformGrid(narrowCollisionMode, isPlayerColliding);
		break;

	case BroadCollisionMode::QUADTREE:
		this->HandleBroadPhaseQuadtree(narrowCollisionMode, isPlayerColliding);
		break;
	default:
		throw std::exception("unknown broad phase collision detected!");
	}

	output.isPlayerColliding = isPlayerColliding;
	output.nCollisions = _nCollisionsThisFrame;
	output.nCollisionTests = _nCollisionTestsThisFrame;

	return output;
}

void CollisionHandler::HandleBroadPhaseBruteForce(std::function<bool(WireframeSprite&, WireframeSprite&)> collisionAlgorithm, bool& isPlayerColliding)
{
	for (auto itAsteroid = this->_asteroids.begin(); itAsteroid != this->_asteroids.end(); itAsteroid++)
	{
		_nCollisionTestsThisFrame++;// test for player
		// Player collision
		if (this->_collidePlayer)
			if (collisionAlgorithm(this->_player, **itAsteroid))
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
					// mark that bullet for cleanup  and asteroid for splitting now that it's hit something
					(*itBullet)->Disable();
					(*itAsteroid)->MarkForSplitting();
				}
			}

		//AsteroidOnAsteroid Collision
		if (this->_collideAsteroids)
			for (auto itAsteroidTwo = itAsteroid + 1; itAsteroidTwo != this->_asteroids.end(); itAsteroidTwo++)
			{
				_nCollisionTestsThisFrame++; // collision tests for asteroids
				// Check for collision of pair
				if (collisionAlgorithm((WireframeSprite&)**itAsteroid, (WireframeSprite&)**itAsteroidTwo))
				{
					(*itAsteroid)->_shape.setOutlineColor(ASTEROID_COLLISION_COLOR);
					(*itAsteroidTwo)->_shape.setOutlineColor(ASTEROID_COLLISION_COLOR);
					this->_nCollisionsThisFrame++;
				}
			}
	}
}

void CollisionHandler::HandleBroadPhaseUniformGrid(std::function<bool(WireframeSprite&, WireframeSprite&)> collisionAlgorithm, bool& isPlayerColliding)
{
	auto cells = this->_grid._cells;

	// loop through all cells
	for (unsigned int i = 0; i < cells.size(); i++)
	{
		Cell* cell = &cells[i];

		// get cell x and y
		int x = i % this->_grid._numXCells;
		int y = i / this->_grid._numXCells;

		// loop through cell objects
		for (unsigned int j = 0; j < cell->_objects.size(); j++)
		{
			WireframeSprite* sprite = cell->_objects[j];	// local pointer to current ball in current cell

			// update collisions with starting cell
			CheckCollision(collisionAlgorithm, sprite, cell->_objects, j + 1, isPlayerColliding);

			// update collision with neighbor cells
			if (x > 0)	// checks to the left
			{
				// check left
				CheckCollision(collisionAlgorithm, sprite, _grid.GetCell(x - 1, y)._objects, 0, isPlayerColliding);
				if (y > 0)
				{
					// check top left cell x-1 y-1
					CheckCollision(collisionAlgorithm, sprite, _grid.GetCell(x - 1, y - 1)._objects, 0, isPlayerColliding);
				}
				// Check bottom left
				if (y < _grid._numYCells - 1)
				{
					CheckCollision(collisionAlgorithm, sprite, _grid.GetCell(x - 1, y + 1)._objects, 0, isPlayerColliding);
				}
			}
			// Check top
			if (y > 0)
			{
				CheckCollision(collisionAlgorithm, sprite, _grid.GetCell(x, y - 1)._objects, 0, isPlayerColliding);
			}
		}
	}
}

void CollisionHandler::HandleBroadPhaseQuadtree(std::function<bool(WireframeSprite&, WireframeSprite&)> collisionAlgorithm, bool& isPlayerColliding)
{
	std::vector<WireframeSprite*> others;

	// player collision
	this->_quadTree->Query(this->_player.GetBoundingRectangle(), others);
	// if the collision mode is AABB, quadtree already uses aabb to narrow down search so can just ignore the detailed test
	this->CheckCollision((this->_narrowCollisionMode == NarrowCollisionMode::AABB? BypassNarrowTest:collisionAlgorithm),
		&this->_player, others, 0, isPlayerColliding);
	others.clear();

	// Asteroid collision
	for (unsigned int i = 0; i < this->_asteroids.size(); i++)
	{
		// find any possible collisions with the asteroid
		this->_quadTree->Query(this->_asteroids[i]->GetBoundingRectangle(), others);
		// if the collision mode is AABB, quadtree already uses aabb to narrow down search so can just ignore the detailed test
		this->CheckCollision((this->_narrowCollisionMode == NarrowCollisionMode::AABB ? BypassNarrowTest : collisionAlgorithm),
			this->_asteroids[i], others, 0, isPlayerColliding);
		others.clear();
	}

	// Bullet collision
	for (unsigned int i = 0; i < this->_bullets.size(); i++)
	{
		// find any possible collisions with the asteroid
		this->_quadTree->Query(this->_bullets[i]->GetBoundingRectangle(), others);
		// if the collision mode is AABB, quadtree already uses aabb to narrow down search so can just ignore the detailed test
		this->CheckCollision((this->_narrowCollisionMode == NarrowCollisionMode::AABB ? BypassNarrowTest : collisionAlgorithm),
			this->_bullets[i], others, 0, isPlayerColliding);
		others.clear();
	}

}

void CollisionHandler::CheckCollision(std::function<bool(WireframeSprite&, WireframeSprite&)> collisionAlgorithm,
	WireframeSprite* spriteA, std::vector<WireframeSprite*>& spritesToCheck, unsigned int startingIndex, bool& isPlayerColliding)
{
	if (!spriteA)
		return;

	for (unsigned int i = startingIndex; i < spritesToCheck.size(); i++)
	{
		if (!spritesToCheck[i])
			return;

		// no point checking collision against yourself
		if (spritesToCheck[i] == spriteA)
			continue;

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

				// Get the names of the derived class so specific action can be performed on specific collision types
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
						Asteroid* asteroidToSplit = dynamic_cast<Asteroid*>(pSpriteB);
						if (asteroidToSplit)
							asteroidToSplit->MarkForSplitting();
					}

				// Test for asteroid against asteroid collision
				// Only need to test once as both objects are the same type so either way round will trigger the method
				if (this->_collideAsteroids && l == 0)
				{
					if (strcmp(spriteAtype, "class Asteroid") == 0 && std::strcmp(spriteBtype, "class Asteroid") == 0)
					{
						pSpriteA->_shape.setOutlineColor(ASTEROID_COLLISION_COLOR);
						pSpriteB->_shape.setOutlineColor(ASTEROID_COLLISION_COLOR);
					}
				}
			}
		}
	}
}

bool CollisionHandler::AABBCollision(WireframeSprite& spriteA, WireframeSprite& spriteB)
{
	// Check for simple bounding box collision
	if (spriteA.GetBoundingRectangle().intersects(spriteB.GetBoundingRectangle()))
	{
		return true;
	}

	return false;
}

bool CollisionHandler::TestSATCollision(WireframeSprite& spriteA, WireframeSprite& spriteB)
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

bool CollisionHandler::BypassNarrowTest(WireframeSprite& spriteA, WireframeSprite& spriteB)
{
	return true;
}
