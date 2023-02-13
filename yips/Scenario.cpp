#include "pch.h"
#include "Scenario.h"
#include "Background.h"
#include "App.h"
#include "BallObstacle.h"
#include "BallProjectile.h"

Scenario * Scenario::Current = nullptr;

Scenario::Scenario(ChunkIdx chunk_cnt)
	: _chunk_cnt(chunk_cnt)
{
	Current = this;

	_view.reset({ 0.f, 0.f, kChunkSizeX, kChunkSizeY });
	_view.move(0.f, -kChunkSizeY);
	g_app->_wnd.setView(_view);

	AddObject(make_unique<StarsBackground>(0.f, kChunkSizeY * chunk_cnt));
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
	return;
	/*
	size_t chunk_cnt = json["chunk_cnt"];
	Initialize(chunk_cnt);

	_file_name = json["scenario_name"];

	auto & jchunks = json["chunks"];

	for(auto & jchunk : jchunks)
	{
		ChunkIdx chunk_idx = jchunk["idx"];
		if(jchunk.find("objects") == jchunk.end())
			continue;

		auto & objects = jchunk["objects"];	
		for(auto & object : objects)
		{
			auto type = object["type"].get<string>();
			auto id = ObjectID::None;
			{
				size_t idx = 0;
				for(auto name : enum_names<ObjectID>())
				{
					if(name == type)
						id = (ObjectID)idx;

					++idx;
				}
			}

			if(id == ObjectID::None)
			{
				PrintError("Fetching wrong ObjectID for {}", _file_name);
				continue;
			}

			auto x = object["x"].get<float>();
			auto y = object["y"].get<float>();

			switch(id)
			{
			case ObjectID::Obstacle:
				auto obj = _chunks[chunk_idx]->AddObject(make_unique<Obstacle1>());
				obj->SetInitialPosition(x, y);
				break;
			}
		}
	}*/
}