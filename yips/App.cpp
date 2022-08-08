#include "pch.h"
#include "App.h"
#include "Scenes.h"

#include <thread>
#include <chrono>

void App::Tick()
{
	g_app = this;

	Global::Initialize();

	_font.loadFromFile(kDataFolder + "CascadiaCode.ttf"s);

	sf::ContextSettings settings;

	_wnd.create(sf::VideoMode{ g_res_x, g_res_y }, "Yips", 
		sf::Style::Titlebar | sf::Style::Close, settings
	);

	_wnd.setVerticalSyncEnabled(true);
	_wnd.setFramerateLimit(120);

	ImGui::SFML::Init(_wnd);

	sf::Clock delta_clock;

	while(!_exit)
	{
		HandleEvents();

		auto delta_time = delta_clock.restart();

		ImGui::SFML::Update(_wnd, delta_time);
		
		g_scene->Tick(delta_time);

		Render(delta_time);

		_clock.restart();

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	ImGui::SFML::Shutdown();
	Global::Deinitialize();
}

bool App::HandleEvents()
{
	sf::Event event;
	while(_wnd.pollEvent(event))
	{
		if (event.type == sf::Event::EventType::KeyPressed)
		{
			if(event.key.code == sf::Keyboard::Escape)
			{
				_exit = true;
				return true;
			}
		}

		g_scene->HandleEvents(event);
		ImGui::SFML::ProcessEvent(event);
	}

	return false;
}

void App::Render(sf::Time delta_time)
{
	_wnd.clear();

	g_scene->Render(_wnd, delta_time);

	//ImGui::ShowDemoWindow();

	ImGui::SFML::Render(_wnd);

	_wnd.display();
}

void App::RenderText(string text, Vector2f pos, uint size, 
	sf::Color color, std::function<void(sf::Text&)> pre_draw_fnc)
{
	static sf::Text sf_text;
	sf_text.setFont(_font);
	sf_text.setPosition(pos);
	sf_text.setCharacterSize(size);
	sf_text.setFillColor(color);
	sf_text.setString(text.c_str());
	
	if(pre_draw_fnc)
		pre_draw_fnc(sf_text);

	_wnd.draw(sf_text);
}
