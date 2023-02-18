#include "pch.h"
#include "Scenario.h"
#include "Background.h"
#include "App.h"
#include "BallObstacle.h"
#include "BallProjectile.h"

Scenario * Scenario::Current = nullptr;

Scenario::Scenario(ChunkIdx chunk_cnt)
{
	Initialize(chunk_cnt);
}

void Scenario::Initialize(ChunkIdx chunk_cnt)
{
	Current = this;
	
	_chunk_cnt = chunk_cnt;

	_view.reset({ 0.f, 0.f, kChunkSizeX, kChunkSizeY });
	_view.move(0.f, -kChunkSizeY);
	g_app->_wnd.setView(_view);

	AddObject(make_unique<StarsBackground>(0.f, float(kChunkSizeY * chunk_cnt)));
}

void Scenario::Tick(sf::Time delta_time)
{
	for(auto & projectile : _projectiles)
	{
		projectile->Tick(delta_time);
	}

	Object::Tick(delta_time);
	
	for(auto it = _objects.begin(); it != _objects.end();)
	{
		auto & obj = *it;

		// #todo if not in sight, continue

		for(auto proj_it = _projectiles.begin(); proj_it != _projectiles.end();)
		{
			if(proj_it->get()->IsCollision(obj.get()))
			{
				obj->HandleCollision();
				proj_it->get()->HandleCollision();
			}
			
			if(proj_it->get()->HasExpired())
				proj_it = _projectiles.erase(proj_it);
			else
				++proj_it;
		}

		if (obj->HasExpired())
			it = _objects.erase(it);
		else 
			++it;
	}
}

void Scenario::Render(sf::RenderTarget & wnd, sf::Time delta_time)
{
	g_app->_wnd.setView(_view);

	auto mp = g_app->_wnd.mapPixelToCoords(sf::Mouse::getPosition(g_app->_wnd));

	//Print("{} {}", mp.x, mp.y);
	
	g_app->RenderText(format("_camera_x: {}, _camera_y: {}", 
		_camera_x, _camera_y).c_str(), Vector2f(mp.x, mp.y + 20), 15);
	
	g_app->RenderText(format("_current_chunk: {}", 
		_current_chunk).c_str(), Vector2f(mp.x, mp.y + 35), 15);

	g_app->RenderText(format("_current_chunk * -kChunkSizeY: {}", 
		_current_chunk * -kChunkSizeY).c_str(), Vector2f(mp.x, mp.y + 50), 15);

	auto chunk_idx = (kChunkSizeY + _camera_y - mp.y) / kChunkSizeY;
	g_app->RenderText(format("CurrentMouseChunk: {}", chunk_idx).c_str(), Vector2f(mp.x, mp.y + 65), 15);

	float y = kChunkSizeY - ((chunk_idx * kChunkSizeY) - (kChunkSizeY * floor(chunk_idx)));
	g_app->RenderText(format("CurrentMouseChunkPosY: {}", y).c_str(), Vector2f(mp.x, mp.y + 80), 15);

	//mp.y += kChunkSizeY;
	g_app->RenderText(format("Mouse Pos: {} {}", mp.x, mp.y).c_str(), Vector2f(mp.x, mp.y + 95), 15);

	Object::Render(wnd, delta_time);
	
	/*
	for(auto & chunk : _chunks)
	{
		chunk->Render(wnd, delta_time);
	}
	*/
	
	//PrintDebug("Projectile.size: {}", _projectiles.size());
	for(auto & projectile : _projectiles)
	{
		projectile->Render(wnd, delta_time);
	}
}

void Scenario::Move(float x, float y)
{
	_view.move(x, y);

	_camera_x += x;
	_camera_y += y;

	UpdateCurrentChunkIdx();
}

void Scenario::SetCameraPoint(float x, float y)
{
	
}

void Scenario::UpdateCurrentChunkIdx()
{
	_current_chunk = CalcChunkIdx(_camera_y);
}

void Scenario::Load(const Json & json)
{
	_name = json["scenario_name"];
	
	Initialize(json["chunk_cnt"]);

	if (json.find("objects") != json.end())
	{
		for (auto& jobject : json.at("objects"))
		{
			// TODO: Read more than Obstacles in the future
			//auto id = ObjectID::Obstacle;

			auto type_name = jobject.at("type").get<string>();

			for (auto obstacle_value : enum_values<ObstacleType>())
			{
				if (type_name == enum_name(obstacle_value))
				{
					ObstacleType type = obstacle_value;

					if (type == ObstacleType::Ball)
					{
						auto x = jobject.value("x", 0.f);
						auto y = jobject.value("y", 0.f);

						//x += kChunkSizeX / 2.f;
						y -= kChunkSizeY;
						auto final_pos = g_app->_wnd.mapCoordsToPixel({ x, y });

						Log::Debug("Loaded object at {}/{} : {}/{}", x, y, final_pos.x, final_pos.y);
						_objects.push_back(make_unique<BallObstacle>(Vector2f{ (float)x, (float)y }));
					}
				}
			}
		}
	}
}