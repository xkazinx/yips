#pragma once
#include "Object.h"

class Chunk;
class Background;
class Projectile;

class Scenario : public Object
{
public:
	static Scenario * Current;
	static constexpr auto kCount = 32u;

	Scenario(ChunkIdx chunk_cnt);
	Scenario(const Json & json) { Load(json); }
	void Load(const Json & json);

	void Render(sf::RenderTarget & wnd, sf::Time delta_time) override;
	void Tick(sf::Time delta_time) override;
	void Move(float x, float y) override;

public:
	sf::View _view;

	float _camera_x, _camera_y;
	void SetCameraPoint(float x, float y);
	
	bool IsOnSight(float x, float y) const {
		return x >= _camera_x && x <= _camera_x + kChunkSizeX
			&& y >= _camera_y && y <= _camera_y + kChunkSizeY;
	}
	
public:
	ChunkIdx _chunk_cnt;
	
	void UpdateCurrentChunkIdx();
	ChunkIdx _current_chunk;

	string _name;

public:
	sf::VertexArray _line;

	static Vector2f GetChunkWorldPos(ChunkIdx chunk_idx)
	{
		return { 0, float(chunk_idx * -kChunkSizeY) };
	}

	Vector2f GetChunkWorldPos()
	{
		return GetChunkWorldPos(_current_chunk);
	}

public:
	vector<unique_ptr<Projectile>> _projectiles;

public:
	vector<Background*> _background_layers;
};

