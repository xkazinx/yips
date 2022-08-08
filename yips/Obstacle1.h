#pragma once
#include "Object.h"

class Obstacle1 : public Object
{
public:
	Obstacle1();

	void Render(sf::RenderTarget & wnd, sf::Time delta_time);
	void Tick(sf::Time delta_time);

	void SetPosition(float x, float y) override;

	sf::CircleShape _shape;
	sf::Color _color;
};

