#pragma once
#include "Object.h"

enum class MoveType
{
	Wave,

	None = u8(-1)
};

class Obstacle : public Object
{
public:
	friend Object;

	Obstacle(ObstacleType type);
	Obstacle(ObstacleType type, Vector2f origin);

	virtual void Render(sf::RenderTarget& wnd, sf::Time delta_time);
	virtual void Tick(sf::Time delta_time);

	ObstacleType _type;
	/*
	virtual void SetPosition(float x, float y)
	{
		Object::SetPosition(x, y);
	}

	virtual Vector2f GetPosition();
	virtual void Move(float x, float y) override;
	*/

	//virtual bool HandleEvents(sf::Event& event) { return false; }
};

