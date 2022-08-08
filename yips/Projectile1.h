#pragma once
#include "Object.h"

class Projectile1 : public Object
{
public:
	Projectile1(float origin_x, float origin_y);

	bool HandleEvents(sf::Event & event) { return false; }
	void Render(sf::RenderTarget & wnd, sf::Time delta_time) override;
	void Tick(sf::Time delta_time) override;

	void SetPosition(float x, float y) override;

	bool Collides(float x, float y) const;

	sf::CircleShape _shape;
	Vector2f _origin;
};

