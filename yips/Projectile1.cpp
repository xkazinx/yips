#include "pch.h"
#include "Projectile1.h"

Projectile1::Projectile1(float origin_x, float origin_y) 
	: Object(ObjectID::Projectile), _origin(origin_x, origin_y)
{
	_shape.setRadius(10.f);
	_shape.setFillColor(sf::Color::Green);

	SetInitialPosition(origin_x, origin_y);
	_shape.setPosition(origin_x, origin_y);
}

void Projectile1::Render(sf::RenderTarget& wnd, sf::Time delta_time)
{
	wnd.draw(_shape);
}

void Projectile1::Tick(sf::Time delta_time)
{
	{
		float velocity = 50.f;
		_shape.move(0.f, -velocity * delta_time.asSeconds());
	}
}

void Projectile1::SetPosition(float x, float y)
{

}

bool Projectile1::Collides(float x, float y) const
{
	return _shape.getGlobalBounds().contains(x, y);
}