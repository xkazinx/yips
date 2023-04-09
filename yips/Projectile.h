#pragma once
#include "Object.h"

class Projectile : public Object
{
	friend Object;

public:
	Projectile(ProjectileID projectile_id, Vector2f origin);

	bool HandleEvents(sf::Event& event) { return false; }
	virtual void Render(sf::RenderTarget& wnd, sf::Time delta_time) override;
	virtual void Tick(sf::Time delta_time);

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

