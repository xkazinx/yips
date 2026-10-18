#include "pch.h"
#include "BallProjectile.h"
#include "PowerUp.h"

BallProjectile::BallProjectile(Vector2f origin) 
	: Projectile(ProjectileID::Ball, origin)
{
	_shape.setRadius(10.f);
	_shape.setFillColor(sf::Color::Green);

	SetPosition(origin.x, origin.y);
	// SetPosition could stay here as well since it could vary depending on the shape size
}

void BallProjectile::Render(sf::RenderTarget& wnd, sf::Time delta_time)
{
	//Projectile::Render(wnd, delta_time);

	wnd.draw(_shape);
}

void BallProjectile::Tick(sf::Time delta_time)
{
	{
		float bg_velocity = 50.f;
		float velocity = bg_velocity + 700.f;
		_shape.move(0.f, velocity * delta_time.asSeconds());
	}
}

void BallProjectile::SetPosition(float x, float y)
{
	_shape.setPosition(x, y);
	Object::SetPosition(x, y);
}

void BallProjectile::HandleCollision(Object* obj)
{
	if (HasExpired())
		return;

	Log::Debug("BallProjectile::HandleCollision(ObjectID({}))", enum_name(obj->_id));

	//auto expire = false;
	auto expire = true;

	using enum ObjectID;
	switch (obj->_id)
	{
	case Ship:
		break;

	case Sprite:
	case Object:
	case Obstacle:
		expire = true;
		break;
	case Projectile:
		switch(obj->Get<::Projectile>()->_projectile_id)
		{
		case ProjectileID::Ball:
		default:
			expire = true;
			break;
		}
		break;
	case PowerUp:
		switch (obj->Get<::PowerUp>()->_id)
		{
		case PowerUpID::Energy:
			break;
		case PowerUpID::Velocity:
			break;
		case PowerUpID::Invincible:
			break;
		}
		break;
	case None:
		break;
	default:
		break;
	}

	if(expire)
		Expire();

	Projectile::HandleCollision(obj);
}
