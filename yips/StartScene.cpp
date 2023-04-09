#include "pch.h"
#include "StartScene.h"
#include "App.h"
#include "Scenes.h"

StartScene::StartScene() : Scene(SceneID::Start)
{
	_title_texture.loadFromFile(kDataFolder + "game_title.png"s);

	_title.setTexture(_title_texture);
	_title.setPosition(500.f, 500.f);

	auto texture_size = _title.getTexture()->getSize();
	_title.setPosition(Vector2f(
		g_res_x / 2.f - texture_size.x / 2.f,
		g_res_y - 150.f
	));

	_final_title_pos = g_res_y / 2 - u16(texture_size.y / 1.25);

	_shape.setFillColor(sf::Color::Blue);
	_shape.setSize({ 50.f, 50.f });
	_shape.setPosition({ 300.f, 300.f });
}

bool StartScene::HandleEvents(sf::Event & event)
{
	if (event.type == sf::Event::EventType::KeyPressed)
	{
		if(event.key.code == sf::Keyboard::Enter)
		{
			g_scene->Set(SceneID::Scenario);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && event.key.code == sf::Keyboard::E)
		{
			g_scene->Set(SceneID::ScenarioEditor);
		}
	}

	return false;
}

void StartScene::Tick(sf::Time delta_time)
{

}

void StartScene::Render(sf::RenderWindow & wnd, sf::Time delta_time)
{
	//wnd.draw(_shape);

	wnd.draw(_title);

	_title.move(Vector2f(0, -500.f * delta_time.asSeconds()));

	if(_title.getPosition().y < _final_title_pos)
	{
		_title.setPosition(_title.getPosition().x, _final_title_pos);
		_start_displaying_start_game = true;
		_display_start_game = true;
	}

	if(_start_displaying_start_game)
	{
		/*static auto start_game_tick_tm = timeGetTime() + 1500;
		if(timeGetTime() > start_game_tick_tm)
		{
			_display_start_game = !_display_start_game;

			start_game_tick_tm = timeGetTime() + 1500;
		}*/

		if(true)
		//if (_display_start_game)
		{
			g_app->RenderText("Press to start game", 
					Vector2f(g_res_x / 2.f, ((g_res_y / 2.f) * 1.5f)),
					50, sf::Color::White,
				[&](sf::Text & sf_text)
				{
					sf::FloatRect textRect = sf_text.getLocalBounds();
					sf_text.setOrigin(textRect.left + textRect.width / 2.0f,
											textRect.top  + textRect.height / 2.0f);

					sf_text.setPosition(sf::Vector2f(g_res_x / 2.0f, sf_text.getPosition().y));
				});
		}
	}
}
