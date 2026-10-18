#pragma once
#include "Obstacle.h"

class BallObstacle : public Obstacle
{
public:
	static constexpr auto kType = ObstacleType::Ball;

	explicit BallObstacle(const Vector2f origin);

	void Render(sf::RenderTarget & wnd, const sf::Time delta_time) override;
	void Tick(const sf::Time delta_time) override;

	void SetPosition(const float x, const float y) override;
	void Move(const float x, const float y) override;

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

	float _velocity_x = 0.f, _velocity_y = 0.f;

	float _angle_x = 0.f, _angle_y = 0.f;

	// Placed by the editor or a scenario file: it stays where it was put
	MoveType _move_type = MoveType::None;
	u32 _move_timer = 0;
};
