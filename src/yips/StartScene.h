#pragma once
#include "Scene.h"

class StartScene : public Scene
{
public:
	StartScene();

	bool HandleEvents(const sf::Event & event) override;
	void Render(sf::RenderWindow & wnd, const sf::Time delta_time) override;
	void Tick(const sf::Time delta_time) override;

	sf::Texture _title_texture;
	optional<sf::Sprite> _title;

	float _final_title_pos = 0.f;

	bool _start_displaying_start_game = false;
};
