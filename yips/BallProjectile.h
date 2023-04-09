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

	void HandleCollision(Object * obj) override
	{
		Log::Debug("BallProjectile::HandleCollision(ObjectID({}))", enum_name(obj->_id));
		using enum ObjectID;
		switch (obj->_id)
		{
		case Ship:
			break;
		case Sprite:
			break;
		case Chunk:
			break;
		case Object:
			break;
		case Obstacle:
			break;
		case Projectile:
			break;
		case Item:
			break;
		case None:
			break;
		default:
			break;
		}

		Expire();

		Projectile::HandleCollision(obj);
	}

	sf::CircleShape _shape;
	Vector2f _origin;
};

