#pragma once

class App
{
public:
	App(){}
	~App(){}

	void Tick();
	bool HandleEvents();
	void Render(sf::Time delta_time);
	
	sf::RenderWindow _wnd;
	bool _exit = false;

	sf::Font _font;

	sf::Clock _clock;
	
	void RenderText(string text, Vector2f pos, uint size, sf::Color color = sf::Color::White, std::function<void(sf::Text&)> pre_draw_fnc = nullptr);
};
