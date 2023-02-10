#include "pch.h"
#include "Obstacle.h"

Obstacle::Obstacle(ObjectID object_id) : Object(object_id) {}

Obstacle::Obstacle(ObjectID object_id, Vector2f origin)
	: Object(object_id)
{
	SetPosition(origin.x, origin.y);
}

void Obstacle::Render(sf::RenderTarget& wnd, sf::Time delta_time)
{
	Object::Render(wnd, delta_time);
}

void Obstacle::Tick(sf::Time delta_time)
{
	Object::Tick(delta_time);
}