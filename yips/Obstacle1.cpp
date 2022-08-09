#include "pch.h"
#include "Obstacle1.h"

Obstacle1::Obstacle1(float origin_x, float origin_y) : Object(ObjectID::Obstacle)
{
	_shape.setRadius(30.f);
	_shape.setFillColor(sf::Color(25, 25, 25));
	_color = sf::Color::White;

	SetPosition(origin_x, origin_y);
}

void Obstacle1::Render(sf::RenderTarget & wnd, sf::Time delta_time)
{
	wnd.draw(_shape);
	
	Object::Render(wnd, delta_time);
}

void Obstacle1::SetPosition(float x, float y)
{
	_shape.setFillColor(_color);
	_shape.setPosition(x, y);

	Object::SetPosition(x, y);
}

void Obstacle1::Tick(sf::Time delta_time)
{
	Object::Tick(delta_time);
}