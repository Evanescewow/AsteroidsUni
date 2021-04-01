#include "UniformGrid.h"
#include "../Global/ApplicationDefines.h"

UniformGrid::UniformGrid()
{
	// Calculate number of cells in x and y directions
	_numXCells = static_cast<int>(ceil((float)WINDOW_WIDTH / _cellSize));
	_numYCells = static_cast<int>(ceil((float)WINDOW_HEIGHT / _cellSize));


	// Fill the cells container with correct size
	_cells.resize(_numXCells * _numYCells);
}

UniformGrid::~UniformGrid()
{

}

/* void AddObject
 * Brief:
 * Overloaded AddObject. Instead of specifying the cell as a parameter,
 * calculates the destination cell based upon object position
 * Params:
 * <WireframeSprite*> object	- object to add to the cell
*/
void UniformGrid::AddObject(WireframeSprite* object)
{
	// Find cell object is contained in
	Cell* cell = &GetCell(object->GetPosition());

	// add object to that cell
	cell->_objects.push_back(object);

	// setup owner and index links inside the object
	// (index is stored to speed up vector searches when removing)
	object->SetOwnerCell(cell);
	object->SetOwnerCellIndex(cell->_objects.size() - 1);
}

/* void AddObject
 * Brief:
 * Adds an object to a specified cell
 * Params:
 * <WireframeSprite*> object	- object to add to the cell
 * <Cell*>						- Cell to add the object to
*/
void UniformGrid::AddObject(WireframeSprite* object, Cell* cell)
{
	// add object to desired cell
	cell->_objects.push_back(object);

	//setup owner and index links for the object
	object->SetOwnerCell(cell);
	object->SetOwnerCellIndex(cell->_objects.size() - 1);
}

/* void RemoveObject
 * Brief:
 * Removes an object from it's cell. This is achieved by swapping the
 * targeted object with the last object in the cell's container. after which it's
 * popped back and the swapped object is updated with the correct index.
 * Params:
 * <WireframeSprite*> object	- object to remove from it's owner cell
*/
void UniformGrid::RemoveObject(WireframeSprite* object)
{
	// Creat reference to object container for readability
	std::vector<WireframeSprite*>& objects = object->GetOwnerCell()->_objects;

	// do a vector swap to put target object at the back (if not already there)
	objects[object->GetOwnerCellIndex()] = objects.back();
	objects.pop_back();	// Remove object

	// Check there are still objects in the list and that target object isn't last
	// in the list (if it is, would cause an out of range exception)
	if ((unsigned int)object->GetOwnerCellIndex() < objects.size())
	{
		// Update the cell index for the swapped object
		objects[object->GetOwnerCellIndex()]->SetOwnerCellIndex(object->GetOwnerCellIndex());
	}
	
	// unassigns links to owner for removed object
	object->SetOwnerCellIndex(-1);
	object->SetOwnerCell(nullptr);
}

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
Cell& UniformGrid::GetCell(const sf::Vector2f& pos)
{
	// Check x position is in range
	if (pos.x < 0) return _cells[0];
	if (pos.x >= WINDOW_WIDTH) return _cells[0];

	// Check y position is in range
	if (pos.y < 0) return _cells[0];
	if (pos.y >= WINDOW_HEIGHT) return _cells[0];

	// calculate cell x and y positions from window coordinates
	int cellX = static_cast<int>(pos.x / _cellSize);
	int cellY = static_cast<int>(pos.y / _cellSize);

	// return the corresponding cell
	return GetCell(cellX, cellY);
}

/* Cell* GetCell
 * Brief:
 * Returns a reference to a cell based upon cell x and y coordinates
 * Params:
 * <int x> - X coordinate of the cell
 * <int y> - Y coordinate of the cell
 * Returns:
*			<Cell&> - Targeted cell
*/
Cell& UniformGrid::GetCell(int x, int y)
{
	// Check x position is in range
	// Binds x to the correct range if not
	if (x < 0) x = 0;
	if (x >= _numXCells) x = _numXCells;

	// Check y position is in range
	// Binds y to the correct range if not
	if (y < 0) y = 0;
	if (y >= _numYCells) y = _numYCells;

	// Return the targeted cell
	// y * _numXCells + x is used as using one vector like this
	// is less intensive than using a multi-dimensional vector
	return _cells[y * _numXCells + x];
}
