#include "pch.h"
#include "Obstacle.h"
#include "ScenarioScene.h"
#include "Scenes.h"
#include "PowerUp.h"
#include "Scenario.h"
#include "Random.h"

Obstacle::Obstacle(const ObstacleType type)
	: Object(ObjectID::Obstacle), _type(type) {}

Obstacle::Obstacle(const ObstacleType type, const Vector2f origin)
	: Obstacle(type)
{
	_pos = origin;
}

void Obstacle::Render(sf::RenderTarget & wnd, const sf::Time delta_time)
{
	Object::Render(wnd, delta_time);
}

void Obstacle::Tick(const sf::Time delta_time)
{
	Object::Tick(delta_time);
}

void Obstacle::HandleCollision(Object * obj)
{
	if (HasExpired() || obj->_id != ObjectID::Projectile)
		return;

	Expire();

	// Played from the Scenario scene only (the editor's scenario never ticks)
	auto scene = g_scene->Get<ScenarioScene>();
	if (!scene)
		return;

	++scene->_destroyed_obstacles;

	if (scene->_last_destroyed_obstacle_type != _type)
	{
		scene->_last_destroyed_obstacle_type = _type;
		scene->_last_destroyed_obstacle_cnt = 0;
	}

	if (++scene->_last_destroyed_obstacle_cnt >= kPowerUpStreak)
	{
		// Queued: the scenario is iterating its objects right now
		scene->_scenario->Spawn(make_unique<PowerUp>(g_rand->GenerateEnumerator<PowerUpID>(), GetPosition()));
		scene->_last_destroyed_obstacle_cnt = 0;
	}
}
