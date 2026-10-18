#include "pch.h"
#include "Obstacle.h"
#include "ScenarioScene.h"
#include "Scenes.h"
#include "PowerUp.h"
#include "Scenario.h"
#include "Projectile.h"
#include "Random.h"

Obstacle::Obstacle(ObstacleType type)
	: Object(ObjectID::Obstacle), _type(type) {}

Obstacle::Obstacle(ObstacleType type, Vector2f origin)
	: Obstacle(type)
{
	SetPosition(origin.x, origin.y);
}

void Obstacle::Render(sf::RenderTarget& wnd, sf::Time delta_time)
{
	Object::Render(wnd, delta_time);
}

void Obstacle::Tick(sf::Time delta_time)
{
	Object::Tick(delta_time);
}

void Obstacle::HandleCollision(Object * obj)
{
	if (HasExpired())
		return;

	Log::Debug("Obstacle::HandleCollision(ObjectID({}))", enum_name(obj->_id));

	switch (obj->_id)
	{
		using enum ObjectID;
	case Ship:
	case Sprite:
	case Object:
	case Obstacle:
	case PowerUp:
	case None:
	default:
		return;

	case Projectile:
		Expire();
		break;
	}

	auto scene = g_scene->Get<ScenarioScene>();
	if(scene->_last_killed_obstacle_type == _type)
	{
		++scene->_last_killed_obstacle_cnt;

		if (scene->_last_killed_obstacle_cnt >= 4)
		{
			g_scene->Get<ScenarioScene>()->_scenario->AddObject<Object>(
				make_unique<PowerUp>(g_rand->GenerateEnumerator<PowerUpID>(), GetPosition())
			);
		}
	}
	else 
	{
		scene->_last_killed_obstacle_type = _type;
		scene->_last_killed_obstacle_cnt = 1;
	}
}