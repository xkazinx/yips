#include "pch.h"
#include "Background.h"
#include "App.h"
#include "Random.h"
#include "Scenario.h"
#include "Projectile.h"

void StarsBackground::Generate()
{
	constexpr auto kStarsCount = 256;
	for(int i = 0; i < kStarsCount; ++i)
	{
		_vertex_positions.push_back({ 
			(float)g_rand->Generate<int>(0, kViewSizeX), 
			g_rand->Generate<float>(_origin_y, _end_y)
		});

		_vertex_shapes.push_back(sf::Vertex(
			{
				_vertex_positions.back()
			}, sf::Color::White)
		);
	}
}

void StarsBackground::Render(sf::RenderTarget & wnd, sf::Time delta_time)
{
	for(size_t i = 0; i < _vertex_shapes.size(); ++i)
	{
		auto & star = _vertex_shapes[i];
		wnd.draw(&star, 1, sf::PrimitiveType::Points);
	}
	
	//auto & view = g_app->_wnd.getDefaultView();
	
	// #todo: only draw the ones on sight
}

void StarsBackground::Tick(sf::Time delta_time)
{
	// #todo: could change star color gradually
}