#pragma once
#include "Object.h"

class Chunk;
class Background;
class Projectile;

class Scenario : public Object
{
public:
	static Scenario * Current;

	Scenario(uint end_y);
	Scenario(const Json & json) { Load(json); }
	void Load(const Json & json);
	void Initialize(uint end_y);

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
	void UpdateCurrentChunkIdx();
	ChunkIdx _current_chunk;

	string _file_name;

	vector<unique_ptr<Chunk>> _chunks;

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

