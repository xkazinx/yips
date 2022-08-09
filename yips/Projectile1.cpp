#include "pch.h"
#include "Projectile1.h"

Projectile1::Projectile1(float origin_x, float origin_y) 
	: Object(ObjectID::Projectile), _origin(origin_x, origin_y)
{
	_shape.setRadius(10.f);
	_shape.setFillColor(sf::Color::Green);

	SetPosition(origin_x, origin_y);
}

void Projectile1::Render(sf::RenderTarget& wnd, sf::Time delta_time)
{
	wnd.draw(_shape);

	//Object::Render(wnd, delta_time);
}

void Projectile1::Tick(sf::Time delta_time)
{
	{
		float bg_velocity = 50.f;
		float velocity = bg_velocity + 700.f;
		_shape.move(0.f, -velocity * delta_time.asSeconds());
	}
}

void Projectile1::SetPosition(float x, float y)
{
	_shape.setPosition(x, y);
	Object::SetPosition(x, y);
}