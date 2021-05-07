#pragma once
#include "SFML/Graphics.hpp"

// Pre-definitions of classes to keep the header file small
struct Cell;
class Game;
class QuadTree;


/* Class WireframeSprite
 * Brief:
 *	Parent class for any game object. Works by storing an
 *	SFML convex shape made up of several points defined at
 *	child class initialization. friend of Collision handler
 *	to allow access to detailed members for collision calculation
 */
class WireframeSprite
{
public:
	// Needed for collision calculation
	friend class CollisionHandler;

	/* Constructor
	 * Brief:
	 *	assigns the properties to the base convex shape.
	 *	also assigns properties to the hitbox shape. Key to note
	 *	that the hitbox isn't updated or drawn unless it's flagged as
	 *	active.
	 * Params:
	 *	<size_t> pointCount	-	Number of points the convex shape could contain
	 */
	WireframeSprite(size_t pointCount);
	~WireframeSprite();

	/* virtual void Update
	 * Brief:
	 *	handles the update of the sprite object. sets the position of the sprite
	 *	and hitbox in accordance with the private position member.
	 *	updates container of internal points for the next frame. This is done to
	 *	assist with predictive collision.
	 * Method can be overridden by children to include extra functionality however
	 * a call to the base method should always be included.
	 */
	virtual void Update();

	/* virtual void Draw
	 * Brief:
	 * Draws the convex shape to the given window. Also draws the surrounding
	 * hitbox shape if it's enabled
	 * Params:
	 * <renderwindow*> window	-	window to draw the shapes to
	 */
	virtual void Draw(sf::RenderWindow* window);

// Getters

	std::vector<sf::Vector2f> GetPoints() const { return this->_pointsNextFrame;  }			// gets the calculated next frame points
	sf::Vector2f GetPosition() const { return this->_position; }							// Getter for current position
	sf::Vector2f GetVelocity() const { return this->_velocity; }							// Getter for the current velocity
	float GetRotation() const { return this->_shape.getRotation(); }						// Getter for the current sprite rotation
	Cell* GetOwnerCell() const { return this->_ownerCell; }									// Getter for the owner cell (uniformGrid)
	int GetOwnerCellIndex() const {return this->_ownerCellIndex;}							// Getter for the index in the cell array(uniformGrid)
																							// This is to eliminate the need to search the array
	QuadTree* GetOwnerTree() const { return this->_ownerTree; }								// Getter for the owner quad tree
	sf::FloatRect GetBoundingRectangle() const { return this->_shape.getGlobalBounds(); }	// Getter for the global bounds of the sprite shape

// Setters
	void SetOwnerTree(QuadTree* tree) { this->_ownerTree = tree; }							// Setter to update the quad tree in which object is stored
	void SetOwnerCell(Cell* cell) { this->_ownerCell = cell; }								// Setter to update the cell in which object is contained (UG)
	void SetOwnerCellIndex(int index) { this->_ownerCellIndex = index; }					// Setter to update the index within the owner cell array

// Protected methods
protected:
	/* void RotateMesh
	 * Brief:
	 *	rotates the object's sprite by the passed value in degrees
	 * Params:
	 *	<float> RotateDegrees	-	degrees to rotate object by
	 */
	void RotateMesh(float RotationDegrees);

	/* void WrapCoordinates
	 * Brief:
	 *	if called wraps the object to the screen.
	 */
	void WrapCoordinates();

// Protected members
protected:
	sf::ConvexShape _shape;								// Base shape of the object made from several points
	sf::RectangleShape hitboxShape;						// Hitbox shape that surrounds the object

	sf::Vector2f _velocity;								// Velocity of the object
	sf::Vector2f _position{ 0.0f, 0.0f };				// Current position of the object (shape position doesn't update
														// Until after the update that frame)
	std::vector<sf::Vector2f> _pointsNextFrame;			// Points of the convex shape next frame (used for predictive collision)

// Private members
private:
	static constexpr float HITBOX_THICKNESS = 1.0f;		// Thickness of the hitbox shape if drawn
	bool isHitboxVisible = false;						// Should the hitbox shape be drawn

	int _ownerCellIndex = -1;							// Uniform grid owner cell index (array inside the cell)
	Cell* _ownerCell = nullptr;							// Uniform grid cell that owns this object	
	QuadTree* _ownerTree = nullptr;						// Quad tree that owns this object
};

