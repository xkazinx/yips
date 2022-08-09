#include "pch.h"
#include "Chunk.h"
#include "Background.h"

Chunk::Chunk(ChunkIdx idx)
	: Object(ObjectID::Chunk), _idx(idx), _origin_x(0), _origin_y(idx * -kChunkSizeY)
{
	//_background_layers.push_back(bg);
}

void Chunk::Render(sf::RenderTarget & wnd, sf::Time delta_time)
{

	Object::Render(wnd, delta_time);
}

void Chunk::Tick(sf::Time delta_time)
{
	Object::Tick(delta_time);
}