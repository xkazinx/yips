#pragma once
#include "Obstacle.h"

class BallObstacle : public Obstacle
{
public:
	static constexpr auto kType = ObstacleType::Ball;

	BallObstacle(Vector2f origin);

	void Render(sf::RenderTarget & wnd, sf::Time delta_time);
	void Tick(sf::Time delta_time);

	void SetPosition(float x, float y) override;
	void Move(float x, float y) override;

	Vector2f GetPosition() override
	{
		return _shape.getPosition();
	}

	sf::FloatRect GetCollisionRect() const override
	{
		return _shape.getGlobalBounds();
	}

	void HandleCollision(Object * obj) override;

	sf::CircleShape _shape;
	sf::Color _color;

	float _velocity_x, _velocity_y;

	float _angle_x = 0.f, _angle_y = 0.f;

	MoveType _move_type;
	u32 _move_timer = 0;
};

