#include "pch.h"
#include "Obstacle.h"

Obstacle::Obstacle(ObstacleType type)
	: Object(ObjectID::Obstacle), _type(type) {}

Obstacle::Obstacle(ObstacleType type, Vector2f origin)
	: Obstacle(type)
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