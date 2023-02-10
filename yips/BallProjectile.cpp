#include "pch.h"
#include "BallProjectile.h"

BallProjectile::BallProjectile(Vector2f origin) 
	: Projectile(ProjectileID::Ball, origin)
{
	_shape.setRadius(10.f);
	_shape.setFillColor(sf::Color::Green);

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
		_shape.move(0.f, -velocity * delta_time.asSeconds());
	}
}

void BallProjectile::SetPosition(float x, float y)
{
	_shape.setPosition(x, y);
	Object::SetPosition(x, y);
}