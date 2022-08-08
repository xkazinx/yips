#include "pch.h"
#include "Scenario.h"
#include "Background.h"
#include "Chunk.h"
#include "App.h"
#include "Obstacle1.h"
#include "Chunk.h"

Scenario * Scenario::Current = nullptr;

Scenario::Scenario(ChunkIdx chunks)
{
	Initialize(chunks);
}

void Scenario::Initialize(ChunkIdx chunk_cnt)
{
	Current = this;

	_chunks.resize(chunk_cnt);
	for(ChunkIdx i = 0; i < (ChunkIdx)_chunks.size(); ++i)
	{
		_chunks[i] = make_unique<Chunk>(i);
	}

	// Two chunks at sight
	SetCameraPoint(0, 768);

	_line.setPrimitiveType(sf::PrimitiveType::Lines);
	_line.resize(2);
	_line[0].color = sf::Color::White;
	_line[1].color = sf::Color::White;
}

void Scenario::Tick(sf::Time delta_time)
{
	//if (_current_chunk > 0)
		//_chunks[_current_chunk - 1]->Tick(delta_time);

	_chunks[_current_chunk]->Tick(delta_time);

	if (_current_chunk + 1 < _chunks.size())
		_chunks[_current_chunk + 1]->Tick(delta_time);

	for(auto & projectile : _projectiles)
	{
		projectile.Tick(delta_time);
	}

	Object::Tick(delta_time);

	for(auto it = _objects.begin(); it != _objects.end();)
	{
		auto & obj = *it;
		for(auto proj_it = _projectiles.begin(); proj_it != _projectiles.end();)
		{
			//if(proj_it->Collides(obj->GetPosition()))
		}
	}
}

void Scenario::Render(sf::RenderTarget & wnd, sf::Time delta_time)
{
	//if (_current_chunk > 0)
		//_chunks[_current_chunk - 1]->Render(wnd, delta_time);

	_chunks[_current_chunk]->Render(wnd, delta_time);

	if (_current_chunk + 1 < _chunks.size())
		_chunks[_current_chunk + 1]->Render(wnd, delta_time);

	_line[0].position = {0, abs(_camera_y + GetChunkWorldPos().y) };
	_line[1].position = {kChunkSizeX, abs(_camera_y + GetChunkWorldPos().y) };

	wnd.draw(_line);

	auto mp = sf::Mouse::getPosition(g_app->_wnd);
	g_app->RenderText(format("_camera_x: {}, _camera_y: {}", _camera_x, _camera_y).c_str(), Vector2f(mp.x, mp.y + 20), 15);
	g_app->RenderText(format("_current_chunk: {}", _current_chunk).c_str(), Vector2f(mp.x, mp.y + 35), 15);
	g_app->RenderText(format("_current_chunk * -kChunkSizeY: {}", _current_chunk * -kChunkSizeY).c_str(), Vector2f(mp.x, mp.y + 50), 15);

	auto chunk_idx = (kChunkSizeY + _camera_y - mp.y) / kChunkSizeY;
	g_app->RenderText(format("CurrentMouseChunk: {}", chunk_idx).c_str(), Vector2f(mp.x, mp.y + 65), 15);


	{
		//float y = (chunk_idx * kChunkSizeY) - (_camera_y - mp.y);
		//float y = abs((int)_camera_y % kChunkSizeY) - mp.y;
		//float y = 
		/*
		float line = abs(_camera_y + GetChunkWorldPos().y);
		float y;
		 if (line > 768 / 2)
			y = kChunkSizeY - (line + mp.y - ((int)_camera_y % kChunkSizeY));
		 else
			 y = kChunkSizeY - line + mp.y + ((int)_camera_y % kChunkSizeY);
			 */

		float y = kChunkSizeY - ((chunk_idx * kChunkSizeY) - (kChunkSizeY * floor(chunk_idx)));
		
		g_app->RenderText(format("CurrentMouseChunkPosY: {}", y).c_str(), Vector2f(mp.x, mp.y + 80), 15);
		g_app->RenderText(format("Mouse Pos: {} {}", mp.x, mp.y).c_str(), Vector2f(mp.x, mp.y + 95), 15);
		//g_app->RenderText(format("Line: {}", line).c_str(), Vector2f(mp.x, mp.y + 110), 15);
	}

	//Print("{} {}", _camera_x, _camera_y);
	Object::Render(wnd, delta_time);
}

sf::Vector2u Scenario::GetChunks()
{
	return {
		_chunks.size() * kChunkSizeX,
		_chunks.size() * kChunkSizeY 
	};
}

void Scenario::Move(float x, float y)
{
	SetCameraPoint(_camera_x + x, _camera_y + y);
}

void Scenario::SetCameraPoint(float x, float y)
{
	_camera_x = x;
	_camera_y = y;

	UpdateCurrentChunkIdx();
}

void Scenario::UpdateCurrentChunkIdx()
{
	_current_chunk = CalcChunkIdx(_camera_y);
}

void Scenario::Load(const Json & json)
{
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
	}
}