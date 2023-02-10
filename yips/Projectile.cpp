#include "pch.h"
#include "Projectile.h"

Projectile::Projectile(ProjectileID projectile_id, Vector2f origin)
	: Object(ObjectID::Projectile, origin)
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

