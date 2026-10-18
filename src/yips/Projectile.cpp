#include "pch.h"
#include "Projectile.h"

Projectile::Projectile(const ProjectileID projectile_id, const Vector2f origin)
	: Object(ObjectID::Projectile, origin), _origin(origin), _projectile_id(projectile_id)
{
}

void Projectile::Render(sf::RenderTarget & wnd, const sf::Time delta_time)
{
	wnd.draw(_shape);
}

void Projectile::Tick(const sf::Time delta_time)
{
	Object::Tick(delta_time);
}

void Projectile::SetPosition(const float x, const float y)
{
	_shape.setPosition({ x, y });
	Object::SetPosition(x, y);
}

void Projectile::HandleCollision(Object * obj)
{
	if (HasExpired())
		return;

	// A projectile ends on what it hits, but not on the power ups it flies through
	if (obj->_id == ObjectID::Obstacle || obj->_id == ObjectID::Projectile)
		Expire();
}
