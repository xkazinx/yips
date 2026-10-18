#include "pch.h"
#include "StartScene.h"
#include "App.h"
#include "Scenes.h"
#include "Input.h"

StartScene::StartScene() : Scene(SceneID::Start)
{
	if (!_title_texture.loadFromFile(DataFolder() / "game_title.png"))
		Log::Error("Couldn't load game_title.png");

	_title.emplace(_title_texture);

	const auto texture_size = _title_texture.getSize();
	_title->setPosition({ g_res_x / 2.f - texture_size.x / 2.f, g_res_y - 150.f });

	_final_title_pos = g_res_y / 2.f - texture_size.y / 1.25f;

	g_app->_wnd.setView(g_app->_wnd.getDefaultView());
}

bool StartScene::HandleEvents(const sf::Event & event)
{
	if (const auto key = event.getIf<sf::Event::KeyPressed>())
	{
		if (key->code == sf::Keyboard::Key::Enter)
		{
			g_scene->Set(SceneID::Scenario);
			return true;
		}

		if (key->code == sf::Keyboard::Key::E && (key->control || input::IsCtrl()))
		{
			g_scene->Set(SceneID::ScenarioEditor);
			return true;
		}
	}

	return false;
}

void StartScene::Tick(const sf::Time delta_time)
{
	if (_start_displaying_start_game)
		return;

	_title->move({ 0.f, -500.f * delta_time.asSeconds() });

	if (_title->getPosition().y < _final_title_pos)
	{
		_title->setPosition({ _title->getPosition().x, _final_title_pos });
		_start_displaying_start_game = true;
	}
}

void StartScene::Render(sf::RenderWindow & wnd, const sf::Time delta_time)
{
	wnd.draw(*_title);

	if (!_start_displaying_start_game)
		return;

	auto centred = [](sf::Text & text) {
		const sf::FloatRect rect = text.getLocalBounds();
		text.setOrigin({ rect.position.x + rect.size.x / 2.f, rect.position.y + rect.size.y / 2.f });
		text.setPosition({ g_res_x / 2.f, text.getPosition().y });
	};

	g_app->RenderText("Press Enter to start game", { g_res_x / 2.f, g_res_y / 2.f * 1.5f }, 50, sf::Color::White, centred);
	g_app->RenderText("Ctrl+E: scenario editor   Esc: exit", { g_res_x / 2.f, g_res_y / 2.f * 1.5f + 60.f }, 20, sf::Color(160, 160, 160), centred);
}
