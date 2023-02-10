#pragma once
#include "Projectile.h"

class BallProjectile : public Projectile
{
public:
	BallProjectile(Vector2f origin);

	bool HandleEvents(sf::Event & event) { return false; }
	void Render(sf::RenderTarget & wnd, sf::Time delta_time) override;
	void Tick(sf::Time delta_time) override;

	void SetPosition(float x, float y) override;
	Vector2f GetPosition() override
	{
		return _shape.getPosition();
	}

	sf::FloatRect GetCollisionRect() const override
	{
		return _shape.getGlobalBounds();
	}

	void HandleCollision() override
	{
		Expire();
	}

	sf::CircleShape _shape;
	Vector2f _origin;
};

