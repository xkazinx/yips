#include "pch.h"
#include "Projectile.h"
#include "PowerUp.h"

Projectile::Projectile(ProjectileID projectile_id, Vector2f origin)
	: Object(ObjectID::Projectile, origin), _projectile_id(projectile_id)
{

}

void Projectile::Render(sf::RenderTarget& wnd, sf::Time delta_time)
{
	//Object::Render(wnd, delta_time);
}

void Projectile::Tick(sf::Time delta_time)
{
	Object::Tick(delta_time);
}

void Projectile::HandleCollision(Object* obj)
{
	if (HasExpired())
		return;

	Log::Debug("Projectile::HandleCollision(ObjectID({}))", enum_name(obj->_id));

	auto expire = false;

	using enum ObjectID;
	switch (obj->_id)
	{
	case Ship:
		break;
	case Sprite:
		break;
	case Object:
		break;
	case Obstacle:
		expire = true;
		break;
	case Projectile:
		switch (obj->Get<::Projectile>()->_projectile_id)
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

	if (expire)
		Expire();

	Object::HandleCollision(obj);
}
