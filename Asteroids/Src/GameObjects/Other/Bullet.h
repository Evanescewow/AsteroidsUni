#pragma once
#include "../Base/WireframeSprite.h"
class Bullet :
	public WireframeSprite
{
public:
	Bullet(sf::Vector2f playerPosition = {0, 0}, float playerRotation = 0.0f);
	~Bullet();


	bool IsVisible() const { return this->_isVisible; }

	void Update() override;
	void Disable() { this->_isVisible = false; }

private:
	void CalculateStartingVertecies();
	void CleanupUnseenBullets();

private:

	bool _isVisible = true;
	static constexpr float BULLET_SPEED = 5.0f;
	sf::Color BULLET_COLOR = sf::Color::Yellow;
	static constexpr float TRIANGLE_SCALE = 10.0f;

};

