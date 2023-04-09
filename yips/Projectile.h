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

	void HandleCollision(Object * obj) override
	{
		Log::Debug("Projectile::HandleCollision(ObjectID({}))", enum_name(obj->_id));

		using enum ObjectID;
		switch(obj->_id)
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

		Object::HandleCollision(obj);
	}

	sf::CircleShape _shape;
	Vector2f _origin;
	ProjectileID _projectile_id;
};

