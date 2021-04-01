#pragma once
#include <vector>
#include "../GameObjects/WireframeSprite.h"

class Game;

/* Struct Cell
 * Holds a container with pointers to all objects within a grid cell
*/
struct Cell
{
	std::vector<WireframeSprite*> _objects;	// Container for all objects in the cell
};

/* Class UniformGrid
 * Contains the code that controls the spatial partitioning grid for object collision
*/
class UniformGrid
{
	friend class Game;
public:
	UniformGrid();
	~UniformGrid();

	/* void AddObject
	 * Brief:
	 * Adds an object to a specified cell
	 * Params:
	 * <WireframeSprite*> object	- object to add to the cell
	 * <Cell*>						- Cell to add the object to
	*/
	void AddObject(WireframeSprite* object, Cell* cell);

	/* void AddObject
	 * Brief:
	 * Overloaded AddObject. Instead of specifying the cell as a parameter,
	 * calculates the destination cell based upon object position
	 * Params:
	 * <WireframeSprite*> object	- object to add to the cell
	*/
	void AddObject(WireframeSprite* object);

	/* void RemoveObject
	 * Brief:
	 * Removes an object from it's cell. This is achieved by swapping the 
	 * targeted object with the last object in the cell's container. after which it's
	 * popped back and the swapped object is updated with the correct index.
	 * Params:
	 * <WireframeSprite*> object	- object to remove from it's owner cell
	*/
	void RemoveObject(WireframeSprite* object);

	/* Cell* GetCell
	 * Brief:
	 * Returns a reference to a cell based upon cell x and y coordinates
	 * Params:
	 * <int x> - X coordinate of the cell
	 * <int y> - Y coordinate of the cell
	 * Returns:
	*			<Cell&> - Targeted cell
	*/
	Cell& GetCell(int x, int y);

	/* Cell* GetCell
	 * Brief:
	 * Overloaded GetCell. Returns a reference to a cell based upon
	 * window coordinates. Converts window coordinates to cell coordinates and
	 * calls upon the base function internally.
	 * Params:
	 * <const sf::Vector2f& pos> - position contained in target cell
	 * Returns:
	*			<Cell&> - Targeted cell
	*/
	Cell& GetCell(const sf::Vector2f& pos);


private:
	// Members
	int _numXCells;							// Number of cells in the X direction
	int _numYCells;							// Number of cells in the Y direction
	std::vector<Cell> _cells;				// Container for all the cells in the grid


	// constants
	static constexpr int _cellSize = 200;	// Size in window pixels of each cell (both x and y)
};

