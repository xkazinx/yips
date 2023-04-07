#pragma once
#include "Object.h"

class Background : public Object
{
public:
	Background(float origin_y, float height) 
		: _origin_x(0), _origin_y(origin_y), _end_y(origin_y + height)
	{

	}

	virtual void Tick(sf::Time delta_time) override {
		PrintDebug("Background::Tick");
	}

	float _origin_x, _origin_y, _end_y;
};

class StarsBackground : public Background
{
public:
	StarsBackground(float origin_y, float height) 
		: Background(origin_y, height) 
	{
		Generate();
	}

	void Render(sf::RenderTarget & wnd, sf::Time delta_time);
	void Tick(sf::Time delta_time) override;
	void Generate();

	vector<sf::Vertex> _vertex_shapes;
	vector<Vector2f> _vertex_positions;
};

