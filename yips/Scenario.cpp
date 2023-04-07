#include "pch.h"
#include "Scenario.h"
#include "Background.h"
#include "App.h"
#include "BallObstacle.h"
#include "BallProjectile.h"

Scenario * Scenario::Current = nullptr;

// From ScenarioEditor
Scenario::Scenario(Scenario&& scenario)
{
	Current = this;
	
	_chunk_cnt = scenario._chunk_cnt;

	{
		_view.setCenter(0.f, 0.f);
		_view.reset({ 0.f, 0.f, kViewSizeX, kViewSizeY });
		_view.move(0.f, 0.f);

		g_app->_wnd.setView(_view);
	}

	_objects = std::move(scenario._objects);
}

Scenario::Scenario(ChunkIdx chunk_cnt)
{
	Initialize(chunk_cnt);
}

void Scenario::Initialize(ChunkIdx chunk_cnt)
{
	Current = this;
	
	_chunk_cnt = chunk_cnt;

	{
		_view.reset({ 0.f, 0.f, kViewSizeX, kViewSizeY });
		_view.move(0.f, 0.f);

		g_app->_wnd.setView(_view);
	}

	AddObject(make_unique<StarsBackground>(0.f, float(kViewSizeY * chunk_cnt)));
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
	g_app->_wnd.setView(g_app->_wnd.getDefaultView());

	auto mc = g_app->_wnd.mapPixelToCoords(sf::Mouse::getPosition(g_app->_wnd));
	auto mp = g_app->_wnd.mapCoordsToPixel(mc);

	{
		Vector2f text_pos = { mc.x, mc.y + 20 };
		constexpr auto text_size = 15u;

		auto view_center = _view.getCenter();
		g_app->RenderText(format(
			"view_center: {}/{}", 
			view_center.x, view_center.y
		).c_str(), text_pos, text_size);

		text_pos.y += text_size;

		auto view_viewport = _view.getViewport();
		g_app->RenderText(format(
			"_view_viewport: l:{} t:{} r:{} b:{}", 
			view_viewport.left, view_viewport.top, view_viewport.width, view_viewport.height
		).c_str(), text_pos, text_size);

		text_pos.y += text_size;
	
		auto view_rotation = _view.getRotation();
		g_app->RenderText(format(
			"view_rotation: {}", 
			view_rotation
		).c_str(), text_pos, text_size);
	
		text_pos.y += text_size;

		g_app->RenderText(format(
			"mouse_coord: {}/{}", 
			mc.x, mc.y
		).c_str(), text_pos, text_size);

		text_pos.y += text_size;

		g_app->RenderText(format(
			"mouse_pos: {}/{}",
			mp.x, mp.y
		).c_str(), text_pos, text_size);
	}

	{
		_view.setRotation(180);

		g_app->_wnd.setView(_view);

		Object::Render(wnd, delta_time);

		for (auto& projectile : _projectiles)
		{
			projectile->Render(wnd, delta_time);
		}
	}
}

void Scenario::Move(float x, float y)
{
	_view.move(x, y);
}

void Scenario::Load(const Json & json)
{
	_name = json["scenario_name"];
	
	Initialize(json["chunk_cnt"]);

	if (json.find("objects") != json.end())
	{
		for (auto& jobject : json.at("objects"))
		{
			// TODO: Read more than Obstacles in the future
			//auto id = ObjectID::Obstacle;

			auto type_name = jobject.at("type").get<string>();

			for (auto obstacle_value : enum_values<ObstacleType>())
			{
				if (type_name == enum_name(obstacle_value))
				{
					ObstacleType type = obstacle_value;

					if (type == ObstacleType::Ball)
					{
						auto x = jobject.value("x", 0.f);
						auto y = jobject.value("y", 0.f);

						_objects.push_back(make_unique<BallObstacle>(Vector2f{ (float)x, (float)y }));

						auto & object = _objects.back();
					}
				}
			}
		}
	}
}