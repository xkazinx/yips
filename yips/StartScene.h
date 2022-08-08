#pragma once
#include "Scene.h"

class StartScene : public Scene
{
public:
	StartScene();

	bool HandleEvents(sf::Event & event) override;
	void Render(sf::RenderWindow & wnd, sf::Time delta_time) override;
	void Tick(sf::Time delta_time) override;

	sf::RectangleShape _shape;

	sf::Sprite _title;
	sf::Texture _title_texture;

	u16 _final_title_pos;

	bool _start_displaying_start_game = false;
	bool _display_start_game = true;
};

