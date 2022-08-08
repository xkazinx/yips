#pragma once
#include "Object.h"

class Background;
class Chunk : public Object
{
public:
	Chunk(ChunkIdx idx);
	
	void Render(sf::RenderTarget & wnd, sf::Time delta_time);
	void Tick(sf::Time delta_time);

	ChunkIdx _idx;

	float _origin_x, _origin_y;

	vector<Background*> _background_layers; 
};

