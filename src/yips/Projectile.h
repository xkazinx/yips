#pragma once
#include "Object.h"

class Projectile : public Object
{
	friend Object;

public:
	Projectile(const ProjectileID projectile_id, const Vector2f origin);

	void Render(sf::RenderTarget & wnd, const sf::Time delta_time) override;
	void Tick(const sf::Time delta_time) override;

	void SetPosition(const float x, const float y) override;
	Vector2f GetPosition() override
	{
		return _shape.getPosition();
	}

	sf::FloatRect GetCollisionRect() const override
	{
		return _shape.getGlobalBounds();
	}

	void HandleCollision(Object * obj) override;

	sf::CircleShape _shape;
	Vector2f _origin;
	ProjectileID _projectile_id;
};
