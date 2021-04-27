#include "QuadTree.h"
#include "../../../GameObjects/Base/WireframeSprite.h"
#include <algorithm>


/* Constructor
 * Brief:
 *	sets up the rectangle shape with default parameters, assigns the
 *	internal boundary and assigns the internal capacity needed before splitting
 */
QuadTree::QuadTree(sf::FloatRect boundary, unsigned int capacity)
	:
_boundary(boundary),
_capacity(capacity){

	this->_rect.setFillColor(sf::Color::Transparent);
	this->_rect.setOutlineColor(_gridColor);
	this->_rect.setOutlineThickness(1);

	this->_rect.setPosition({ boundary.left - boundary.width, boundary.top - boundary.height });
	this->_rect.setSize({ boundary.width*2, boundary.height*2 });
}

/* Destructor
* Brief:
*	deletes any children trees if this node has divided
*/
QuadTree::~QuadTree()
{
	if (this->_isDivided)
	{
		delete this->_nw;
		delete this->_ne;
		delete this->_sw;
		delete this->_se;
	}
}

/*void AddObject
* Brief:
*	Takes a pointer to a wire frame sprite and adds it to the appropriate container
*	within the structure. calls itself recursively on children so it will be stored in
*	the correct node of the global structure.
* *Params:
* <WireframeSprite& > sprite	- the sprite to add to the tree structure
*/
void QuadTree::AddObject(WireframeSprite* sprite)
{
	sf::Vector2f pos = sprite->GetPosition();
	if (!this->Contains(pos))
		return;

	// node has capacity, just add to sprites
	if (this->_sprites.size() < this->_capacity)
	{
		this->_sprites.push_back(sprite);
		sprite->SetOwnerTree(this);
	}
	// There's no room in this node
	else
	{
		// if no children yet make them
		if (!this->_isDivided)
			this->SubDivide();

		// pass object to children
		this->_nw->AddObject(sprite);
		this->_ne->AddObject(sprite);
		this->_sw->AddObject(sprite);
		this->_se->AddObject(sprite);
	}
}

/*void AddObject
* Brief:
*	Takes a pointer to a wire frame sprite and removes it from the stored
*	 location in the tree. calls itself recursively on children to find the right
*	container
* <WireframeSprite& > sprite	- the sprite to remove from the tree
*/
void QuadTree::RemoveObject(WireframeSprite* sprite)
{
	// Check that object is within quadtree bounds
	sf::Vector2f pos = sprite->GetPosition();
	if (!this->Contains(pos))
		return;

	// Check if this is the owner tree
	if (sprite->GetOwnerTree() == this)
	{
		// Erase sprite from the container
		this->_sprites.erase(std::find(this->_sprites.begin(), this->_sprites.end(), sprite));
		sprite->SetOwnerTree(nullptr);
	}

	// Remove object from children
	else if (this->_isDivided)
	{
		this->_nw->RemoveObject(sprite);
		this->_ne->RemoveObject(sprite);
		this->_sw->RemoveObject(sprite);
		this->_se->RemoveObject(sprite);
	}
}

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
void QuadTree::Query(sf::FloatRect range, std::vector<WireframeSprite*>& sprites)
{
	// if this boundary doesn't intersect the range just return
	if (!this->Intersects(range))
		return;


	// if it does add the points from this quadtree that are contained within the range
	else
	{
		for (unsigned int i = 0; i < this->_sprites.size(); i++)
		{
			// Broad phase test for if there's any point checking for further collision
			// AABB
			if (_sprites[i]->GetBoundingRectangle().intersects(range))
				sprites.push_back(_sprites[i]);	// push back the sprite for narrow phase collision
		}
	}

	// If this node has divided also query the children to find more
	// possibly colliding objects
	if (this->_isDivided)
	{
		this->_nw->Query(range, sprites);
		this->_ne->Query(range, sprites);
		this->_sw->Query(range, sprites);
		this->_se->Query(range, sprites);
	}
}

/* void Clear
 * Brief:
 *	Clears all the sprites held by the current quad tree (this) and
 *	deletes all of it's children. Whenever a quad tree is deleted all of it's subsequent
 *	children are also deleted
 *	therefore any branch lower than the node that clear is called upon will be deleted.
 *	Calling this on the root node therefore clears the entire tree.
*/
void QuadTree::Clear()
{
	// if tree has children, recursively delete the children also
	if (this->_isDivided)
	{
		delete this->_nw;
		delete this->_ne;
		delete this->_sw;
		delete this->_se;

		this->_isDivided = false;
	}

	// clear sprites of this node as it's not deleted
	this->_sprites.clear();
}

/* void SubDivide
* Brief:
*	If the isDivided flag for this node hasn't already been set to true,
*	the method will create a new quad tree for each child member and then set
*	the isDivided flag to true.
*/
void QuadTree::SubDivide()
{
	// local variables to help readability
	using namespace sf;
	float x = this->_boundary.left;
	float y = this->_boundary.top;
	float w = this->_boundary.width;
	float h = this->_boundary.height;

	// Calculate boundaries for children
	FloatRect neBoundary = FloatRect(x+w/2, y-h/2, w/2, h/2);
	FloatRect nwBoundary = FloatRect(x-w/2, y-h/2, w/2, h/2);
	FloatRect seBoundary = FloatRect(x+w/2, y+h/2, w/2, h/2);
	FloatRect swBoundary = FloatRect(x-w/2, y+h/2, w/2, h/2);
	
	// Create the children
	this->_nw = new QuadTree(nwBoundary, this->_capacity);
	this->_ne = new QuadTree(neBoundary, this->_capacity);
	this->_sw = new QuadTree(swBoundary, this->_capacity);
	this->_se = new QuadTree(seBoundary, this->_capacity);

	// indicate this quadtree has divided
	this->_isDivided = true;
}


/* void Draw
* Brief:
*	Draws the tree's rectangle shape recursively on any child node below it
*	Drawing from the super node will therefore draw the entire structure.
* Params:
*	<sf::RenderWindow* window>	-	window to draw the structure to
*/
void QuadTree::Draw(sf::RenderWindow* window)
{
	// Draw this quadtree
	window->draw(this->_rect);

	// Draw children quadtrees
	if (this->_isDivided)
	{
		this->_nw->Draw(window);
		this->_ne->Draw(window);
		this->_sw->Draw(window);
		this->_se->Draw(window);
	}
}

// Returns true if the position point passed in in contained within the tree's boundary
bool QuadTree::Contains(sf::Vector2f& pos)
{
	return (pos.x >= this->_boundary.left - this->_boundary.width &&
		pos.x <= this->_boundary.left + this->_boundary.width &&
		pos.y >= this->_boundary.top - this->_boundary.height &&
		pos.y <= this->_boundary.top + this->_boundary.height);
}

// Returns true if the given range intersects with the tree's boundary
bool QuadTree::Intersects(sf::FloatRect rect)
{
	sf::FloatRect bdry = this->_boundary;

	return !(rect.left > this->_boundary.left + this->_boundary.width ||
		rect.left + rect.width < this->_boundary.left - this->_boundary.width ||
		rect.top > this->_boundary.top + this->_boundary.height ||
		rect.top + rect.height < this->_boundary.top - this->_boundary.height);
}