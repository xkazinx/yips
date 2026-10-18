#pragma once
#include "Object.h"

class Background : public Object
{
public:
	Background(const float origin_y, const float height)
		: _origin_x(0), _origin_y(origin_y), _end_y(origin_y + height)
	{
	}

	void Tick(const sf::Time delta_time) override {}

	float _origin_x, _origin_y, _end_y;
};

class StarsBackground : public Background
{
public:
	StarsBackground(const float origin_y, const float height)
		: Background(origin_y, height)
	{
		Generate();
	}

	void Render(sf::RenderTarget & wnd, const sf::Time delta_time) override;
	void Tick(const sf::Time delta_time) override;
	void Generate();

	vector<sf::Vertex> _vertex_shapes;
};
