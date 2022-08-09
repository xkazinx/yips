#pragma once
#include "Object.h"

class Obstacle1 : public Object
{
public:
	Obstacle1(float origin_x, float origin_y);

	void Render(sf::RenderTarget & wnd, sf::Time delta_time);
	void Tick(sf::Time delta_time);

	void SetPosition(float x, float y) override;
	
	Vector2f GetPosition() override
	{
		return _shape.getPosition();
	}

	sf::FloatRect GetCollisionRect() const override
	{
		return _shape.getGlobalBounds();
	}

	void HandleCollision()
	{
		Expire();
	}

	sf::CircleShape _shape;
	sf::Color _color;
};

