#pragma once
#include <vector>
#include "SFML/Graphics.hpp"

// class pre-definitions to save header space
class WireframeSprite;

/* class QuadTree
 *	 an nTree structure that divides a region recursively into quad structures.
 *	THe number of objects needed in a cell before it divides is pass into the constructor
 *	as capacity.
*/
class QuadTree
{
	// Makes collisionHandler a friend so that when collision logic is being
	// performed, it can access the sprite containers of the quad tree structure.
	friend class CollisionHandler;

	// Public interface
public:
	/* Constructor
	 * Brief:
	 *	sets up the rectangle shape with default parameters, assigns the 
	 *	internal boundary and assigns the internal capacity needed before splitting
	 */
	QuadTree(sf::FloatRect boundary, unsigned int capacity);

	/* Destructor
	* Brief:
	*	deletes any children trees if this node has divided
	*/
	~QuadTree();

	/*void AddObject
	* Brief:
	*	Takes a pointer to a wire frame sprite and adds it to the appropriate container
	*	within the structure. calls itself recursively on children so it will be stored in 
	*	the correct node of the global structure.
	* *Params:
	* <WireframeSprite& > sprite	- the sprite to add to the tree structure
	*/
	void AddObject(WireframeSprite* sprite);

	/*void AddObject
	* Brief:
	*	Takes a pointer to a wire frame sprite and removes it from the stored
	*	 location in the tree. calls itself recursively on children to find the right 
	*	container
	* <WireframeSprite& > sprite	- the sprite to remove from the tree
	*/
	void RemoveObject(WireframeSprite* sprite);

	/* void Query
	* Brief:
	*	queries the tree recursively to find all wireframe sprites within a given range.
	*	these sprites from each tree are added to the sprites reference vector that is
	*	passed as a parameter. This therefore means the output of the method is stored
	*	in the referenced parameter.
	* Params:
	*	<FloatRect>					range	-	query range
	*	<vector<wireframesprite*>&> sprites	-	vector to store result to
	*/
	void Query(sf::FloatRect range, std::vector<WireframeSprite*>& sprites);

	/* void Draw
	* Brief:
	*	Draws the tree's rectangle shape recursively on any child node below it
	*	Drawing from the super node will therefore draw the entire structure.
	* Params:
	*	<sf::RenderWindow* window>	-	window to draw the structure to
	*/
	void Draw(sf::RenderWindow* window);

	/* void Clear
	 * Brief:
	 *	Clears all the sprites held by the current quad tree (this) and
	 *	deletes all of it's children. Whenever a quad tree is deleted all of it's subsequent 
	 *	children are also deleted
	 *	therefore any branch lower than the node that clear is called upon will be deleted.
	 *	Calling this on the root node therefore clears the entire tree.
	*/
	void Clear();


	// Private methods for internal use
private:

	/* void SubDivide
	* Brief:
	*	If the isDivided flag for this node hasn't already been set to true,
	*	the method will create a new quad tree for each child member and then set
	*	the isDivided flag to true.
	*/
	void SubDivide();

	// Returns true if the position point passed in in contained within the tree's boundary
	bool Contains(sf::Vector2f& position);

	// Returns true if the given range intersects with the tree's boundary
	bool Intersects(sf::FloatRect rect);


	// Members
private:
	std::vector<WireframeSprite*> _sprites;		// sprites in this current boundary
	bool _isDivided = false;					// does this node have children
	sf::FloatRect _boundary;					// boundary of this node
	unsigned int _capacity;						// Maximum capacity of a quad tree before it subdivides
	sf::Color _gridColor = sf::Color::Blue;		// color to draw the grid

	sf::RectangleShape _rect;					// Rectangle shape that is draw when
												// the draw method is called
												// Note: 
												// While boundary has the origin as the middle of the image
												// _rect uses the top left corner as the origin

	QuadTree* _nw = nullptr;					// north-west child node
	QuadTree* _ne = nullptr;					// north-east child node
	QuadTree* _sw = nullptr;					// south-west child node
	QuadTree* _se = nullptr;					// south-east child node
};