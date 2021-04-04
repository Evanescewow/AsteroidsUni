#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class WireframeSprite;

class QuadTree
{
	friend class CollisionHandler;

public:
	QuadTree(sf::FloatRect boundary, unsigned int capacity);
	~QuadTree();

	void AddObject(WireframeSprite* sprite);

	void Draw(sf::RenderWindow* window);

	void RemoveObject(WireframeSprite* sprite);


	void GetQuadTrees(sf::Vector2f position, std::vector<QuadTree*>& trees);

	void Query(sf::FloatRect range, std::vector<WireframeSprite*>& sprites);

	void Clear();

private:
	void SubDivide();
	bool Contains(sf::Vector2f& position);

	bool Intersects(sf::FloatRect rect);

private:
	std::vector<WireframeSprite*> _sprites;		// sprites in this current boundary
	bool _isDivided = false;					// does this node have children
	sf::FloatRect _boundary;					// boundary of this node
	unsigned int _capacity;

	sf::RectangleShape _rect;

	QuadTree* _nw = nullptr;
	QuadTree* _ne = nullptr;
	QuadTree* _sw = nullptr;
	QuadTree* _se = nullptr;
};