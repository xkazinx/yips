#include "pch.h"
#include "BallObstacle.h"

BallObstacle::BallObstacle(const Vector2f origin)
	: Obstacle(kType, origin)
{
	_shape.setRadius(30.f);
	_color = sf::Color::White;
	_shape.setFillColor(_color);

	SetPosition(origin.x, origin.y);
}

void BallObstacle::Render(sf::RenderTarget & wnd, const sf::Time delta_time)
{
	wnd.draw(_shape);
	Obstacle::Render(wnd, delta_time);
}

void BallObstacle::SetPosition(const float x, const float y)
{
	_shape.setPosition({ x, y });
	Object::SetPosition(x, y);
}

void BallObstacle::Move(const float x, const float y)
{
	SetPosition(x + _shape.getPosition().x, y + _shape.getPosition().y);
}

void BallObstacle::HandleCollision(Object * obj)
{
	if (HasExpired() || obj->_id == ObjectID::PowerUp)
		return;

	Obstacle::HandleCollision(obj);
}

void BallObstacle::Tick(const sf::Time delta_time)
{
	if (_move_type == MoveType::Wave)
	{
		constexpr float kLength = 2000.f;
		constexpr float kAngleStep = 5.f;

		const float x = kLength * std::cos(_angle_x);
		const float y = 250.f;

		if (timeGetTime() > _move_timer)
		{
			_angle_x += kAngleStep;
			if (_angle_x >= 256)
				_angle_x = 0.f;

			_angle_y += kAngleStep;
			if (_angle_y >= 256)
				_angle_y = 0.f;

			_move_timer = timeGetTime() + 100;
		}

		Move(-x * delta_time.asSeconds(), -y * delta_time.asSeconds());
	}
	else if (_move_type == MoveType::Linear)
	{
		Move(0.f, -_velocity_y * delta_time.asSeconds());
	}

	Obstacle::Tick(delta_time);
}
