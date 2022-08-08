#pragma once
#include "Object.h"

class Background : public Object
{
public:
	Background(ChunkIdx chunk_idx)
		: _chunk_idx(chunk_idx)
	{
		_origin_x = 0;
		_origin_y = chunk_idx * -kChunkSizeY;
	}

	virtual void Tick(sf::Time delta_time) override {
		PrintDebug("Background::Tick");
	}

	ChunkIdx _chunk_idx;
	
	float _origin_x, _origin_y;
};

class StarsBackground : public Background
{
public:
	StarsBackground(ChunkIdx chunk_idx) : Background(chunk_idx) {
		Generate();
	}

	void Render(sf::RenderTarget & wnd, sf::Time delta_time);
	void Tick(sf::Time delta_time) override;
	void Generate();

	vector<sf::Vertex> _vertex_shapes;
	vector<Vector2f> _vertex_positions;
};

