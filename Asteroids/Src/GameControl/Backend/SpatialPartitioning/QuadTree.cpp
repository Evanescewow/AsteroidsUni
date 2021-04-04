#include "QuadTree.h"
#include "../../../GameObjects/Base/WireframeSprite.h"
#include <algorithm>

QuadTree::QuadTree(sf::FloatRect boundary, unsigned int capacity)
	:
_boundary(boundary),
_capacity(capacity){

	this->_rect.setFillColor(sf::Color::Transparent);
	this->_rect.setOutlineColor(sf::Color::Blue);
	this->_rect.setOutlineThickness(1);

	this->_rect.setPosition({ boundary.left - boundary.width, boundary.top - boundary.height });
	this->_rect.setSize({ boundary.width*2, boundary.height*2 });
}

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

bool QuadTree::Contains(sf::Vector2f& pos)
{
	return (pos.x >= this->_boundary.left - this->_boundary.width &&
		pos.x <= this->_boundary.left + this->_boundary.width &&
		pos.y >= this->_boundary.top - this->_boundary.height &&
		pos.y <= this->_boundary.top + this->_boundary.height);
}


void QuadTree::AddObject(WireframeSprite* sprite)
{
	std::vector<sf::Vector2f> spritePoints = sprite->GetPoints();

	bool hasPointInRect = false;

	// loop over all points in sprite to check if rects contains points not the entire shape
	for (int i = 0; i < spritePoints.size(); i++)
		if (this->Contains(spritePoints[i]))
		{
			hasPointInRect = true;
			break;
		}

	// if no points in boundary return
	if (!hasPointInRect)
	{
		return;
	}

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

void QuadTree::Draw(sf::RenderWindow* window)
{
	window->draw(this->_rect);

	if (this->_isDivided)
	{
		this->_nw->Draw(window);
		this->_ne->Draw(window);
		this->_sw->Draw(window);
		this->_se->Draw(window);
	}
}

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

void QuadTree::GetQuadTrees(sf::Vector2f position, std::vector<QuadTree*>& trees)
{

	// if points are in within these bounds, add this tree to the list
	// Dont add to the list if there are no sprites here
	if (this->Contains(position) && this->_sprites.size() > 0)
	{
		trees.push_back(this);
	}

	// bounds not within this tree, ignore it
	else
	{
		return;
	}

	// if has children check if point is within their bounds
	if (this->_isDivided)
	{
		_nw->GetQuadTrees(position, trees);
		_ne->GetQuadTrees(position, trees);
		_sw->GetQuadTrees(position, trees);
		_se->GetQuadTrees(position, trees);
	}
}

void QuadTree::SubDivide()
{
	using namespace sf;
	float x = this->_boundary.left;
	float y = this->_boundary.top;
	float w = this->_boundary.width;
	float h = this->_boundary.height;

	FloatRect neBoundary = FloatRect(x+w/2, y-h/2, w/2, h/2);
	FloatRect nwBoundary = FloatRect(x-w/2, y-h/2, w/2, h/2);
	FloatRect seBoundary = FloatRect(x+w/2, y+h/2, w/2, h/2);
	FloatRect swBoundary = FloatRect(x-w/2, y+h/2, w/2, h/2);
	

	this->_nw = new QuadTree(nwBoundary, this->_capacity);
	this->_ne = new QuadTree(neBoundary, this->_capacity);
	this->_sw = new QuadTree(swBoundary, this->_capacity);
	this->_se = new QuadTree(seBoundary, this->_capacity);

	this->_isDivided = true;
}
