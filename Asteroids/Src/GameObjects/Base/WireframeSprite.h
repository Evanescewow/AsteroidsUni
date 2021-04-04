#pragma once
#include <SFML/graphics.hpp>
struct Cell;
class Game;
class QuadTree;


class WireframeSprite
{
public:
	friend class CollisionHandler;

	WireframeSprite(size_t pointCount);
	~WireframeSprite();


	virtual void Update();
	virtual void Draw(sf::RenderWindow* window);

	sf::Vector2f GetPosition() const { return this->_position; }
	sf::Vector2f GetVelocity() const { return this->_velocity; }
	float GetRotation() const { return this->_shape.getRotation(); }

	std::vector<sf::Vector2f> GetPoints() const { return this->_pointsNextFrame;  }

	Cell* GetOwnerCell() const { return this->_ownerCell; }
	int GetOwnerCellIndex() const {return this->_ownerCellIndex;}
	QuadTree* GetOwnerTree() const { return this->_ownerTree; }

	sf::FloatRect GetBoundingRectangle() const { return this->_shape.getGlobalBounds(); }


	void SetOwnerTree(QuadTree* tree) { this->_ownerTree = tree; }
	void SetOwnerCell(Cell* cell) { this->_ownerCell = cell; }
	void SetOwnerCellIndex(int index) { this->_ownerCellIndex = index; }

protected:
	void RotateMesh(float RotationDegrees);
	void WrapCoordinates();

protected:
	sf::ConvexShape _shape;
	sf::RectangleShape hitboxShape;

	sf::Vector2f _velocity;
	sf::Vector2f _position{ 0.0f, 0.0f };

	std::vector<sf::Vector2f> _pointsNextFrame;

private:
	static constexpr float HITBOX_THICKNESS = 1.0f;
	bool isHitboxVisible = false;

	int _ownerCellIndex = -1;
	Cell* _ownerCell = nullptr;

	QuadTree* _ownerTree = nullptr;
};

