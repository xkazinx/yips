#include "pch.h"
#include "Background.h"
#include "Random.h"

void StarsBackground::Generate()
{
	constexpr auto kStarsCount = 256;
	_vertex_shapes.reserve(kStarsCount);

	for (int i = 0; i < kStarsCount; ++i)
	{
		const Vector2f pos{ (float)g_rand->Generate<int>(0, kViewSizeX), g_rand->Generate<float>(_origin_y, _end_y) };
		_vertex_shapes += sf::Vertex{ pos, sf::Color::White };
	}
}

void StarsBackground::Render(sf::RenderTarget & wnd, const sf::Time delta_time)
{
	// All the stars in one draw call
	if (!_vertex_shapes.empty())
		wnd.draw(_vertex_shapes.data(), _vertex_shapes.size(), sf::PrimitiveType::Points);

	// #todo: only draw the ones on sight
}

void StarsBackground::Tick(const sf::Time delta_time)
{
	// #todo: could change star color gradually
}
