#include "pch.h"
#include "BallProjectile.h"

BallProjectile::BallProjectile(const Vector2f origin)
	: Projectile(ProjectileID::Ball, origin)
{
	_shape.setRadius(kRadius);
	_shape.setFillColor(sf::Color::Green);

	SetPosition(origin.x, origin.y);
}

void BallProjectile::Tick(const sf::Time delta_time)
{
	const Vector2f step{ 0.f, kVelocity * delta_time.asSeconds() };
	_shape.move(step);
	_pos += step;
}
