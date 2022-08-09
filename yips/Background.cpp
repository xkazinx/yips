#include "pch.h"
#include "Background.h"
#include "App.h"
#include "Random.h"
#include "Scenario.h"
#include "Chunk.h"

void StarsBackground::Generate()
{
	constexpr auto kStarsCount = 256;
	for(int i = 0; i < kStarsCount; ++i)
	{
		_vertex_positions.push_back({ 
			(float)g_rand->Generate<int>(0, kChunkSizeX), 
			(float)g_rand->Generate<int>(_end_y, _origin_y)
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

	//PrintDebug("StarsBackground Render");
	// todo: only draw the ones on sight
	
	/*for(size_t i = 0; i < _vertex_shapes.size(); ++i)
	{
		auto & star = _vertex_shapes[i];

		if (_chunk_idx == Scenario::Current->_current_chunk)
		{
			star.position.y = { abs(Scenario::Current->_camera_y + (_origin_y - Scenario::Current->_camera_y) + (_chunk_idx * kChunkSizeY)) + _vertex_positions[i].y + (abs((int)Scenario::Current->_camera_y % (int)kChunkSizeY)) };
		}
		else if (_chunk_idx == Scenario::Current->_current_chunk + 1)
		{
			star.position.y = { _origin_y + abs(Scenario::Current->_camera_y) + _vertex_positions[i].y };
		}
		
		//if(Scenario::Current->IsOnSight(star.position.x, star.position.y))
		{
			wnd.draw(&star, 1, sf::PrimitiveType::Points);
		}
	}*/

	//g_app->_wnd.setView(view);
}

void StarsBackground::Tick(sf::Time delta_time)
{
	/*if(Scenario::Current->_camera_y < _chunk_idx * -kChunkSizeY &&
		Scenario::Current->_camera_y > )*/

	/*Print("StarsBackground::Tick");
	for(auto & star : _vertex_shapes)
	{
		star.position = { _origin_x, _origin_y + Scenario::Current->_camera_y + star.position.x };
		//if(star.position.x)
		//wnd.draw(&star, 1, sf::PrimitiveType::Points);
	}*/
}