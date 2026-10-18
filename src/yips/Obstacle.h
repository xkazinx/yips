#pragma once
#include "Object.h"

enum class MoveType : u8
{
	Wave,
	Linear,

	None = u8(-1)
};

class Obstacle : public Object
{
public:
	friend Object;

	// Destroyed obstacles of one type in a row that drop a power up
	static constexpr u8 kPowerUpStreak = 4;

	explicit Obstacle(const ObstacleType type);
	Obstacle(const ObstacleType type, const Vector2f origin);

	void Render(sf::RenderTarget & wnd, const sf::Time delta_time) override;
	void Tick(const sf::Time delta_time) override;

	void HandleCollision(Object * obj) override;

	ObstacleType _type;
};
