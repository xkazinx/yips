#include "pch.h"
#include "BallObstacle.h"

BallObstacle::BallObstacle(Vector2f origin) : Obstacle(ObjectID::Obstacle, origin)
{
	_shape.setRadius(30.f);
	_shape.setFillColor(sf::Color(25, 25, 25));
	_color = sf::Color::White;


}

void BallObstacle::Render(sf::RenderTarget & wnd, sf::Time delta_time)
{
	wnd.draw(_shape);
	
	Obstacle::Render(wnd, delta_time);
}

void BallObstacle::SetPosition(float x, float y)
{
	_shape.setFillColor(_color);
	_shape.setPosition(x, y);

	Object::SetPosition(x, y);
}

void BallObstacle::Move(float x, float y)
{
	SetPosition(x + _shape.getPosition().x, y + _shape.getPosition().y);
}

void BallObstacle::Tick(sf::Time delta_time)
{
	{
		static auto tm = timeGetTime() + 100;
		//if (timeGetTime() > tm)
		{
			if (_move_type == MoveType::Wave)
			{			
				//static float x = angle;
				//static float y = length * sin(angle);

				//float x = 25.f + cos(_angle_x) * _velocity_x;
				//float y = sin(_angle_y) * _velocity_y;
			
				/*static int ticks = 0;

				const long double pi = acos(-1.L);
				float x = cos(++ticks * 0.5 * pi / 40);
				float y = 0.f;*/

				/* Circle movement
				constexpr float length = 500.f;
				constexpr float angle_step = 0.1f;
				const long double pi = acos(-1.L);
				
				float x = length * cos (_angle_x);
		      float y = length * sin (_angle_x);
				
				_angle_x += angle_step;
				if(_angle_x >= 2 * pi)
					_angle_x = 0.0;*/

				constexpr float length = 2000.f;
				constexpr float angle_step = 5.f;
				const long double pi = acos(-1.L);
				
				float x = length * cos (_angle_x);
		      float y = 250.f;//length * sin (_angle_y);
				
				if (timeGetTime() > _move_timer)
				{
					_angle_x += angle_step;
					if(_angle_x >= 256)
						_angle_x = 0.0;

					_angle_y += angle_step;
					if(_angle_y >= 256)
						_angle_y = 0.0;

					_move_timer = timeGetTime() + 100;
				}

				Move(x * delta_time.asSeconds(), y * delta_time.asSeconds());

				/*
				{
					tm = timeGetTime() + 100;

					_angle_x += angle_step;
					if(_angle_x >= 360.f)
						_angle_x = 0;

					_angle_y += angle_step;
					if(_angle_y >= 360.f)
						_angle_y = 0;
				}
				*/
			}
			else
			{
				Move(_velocity_x * delta_time.asSeconds(), _velocity_y * delta_time.asSeconds());
			}
		}
	}

	Obstacle::Tick(delta_time);
}