#include "pch.h"
#include "Object.h"

void Object::Tick(const sf::Time delta_time)
{
	for (auto & object : _objects)
		object->Tick(delta_time);
}

void Object::Render(sf::RenderTarget & wnd, const sf::Time delta_time)
{
	for (auto & object : _objects)
		object->Render(wnd, delta_time);

	if (_selected)
	{
		const auto rect = GetCollisionRect();
		sf::RectangleShape bounds(rect.size);
		bounds.setPosition(rect.position);
		bounds.setFillColor(sf::Color::Transparent);
		bounds.setOutlineColor(sf::Color::Blue);
		bounds.setOutlineThickness(1.f);
		wnd.draw(bounds);
	}
}

void Object::Move(const float x, const float y)
{
	_pos.x += x;
	_pos.y += y;
}

void Object::SetPosition(const float x, const float y)
{
	_pos = { x, y };
}

Vector2f Object::GetPosition()
{
	return _pos;
}
