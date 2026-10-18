#pragma once

class App
{
public:
	App() = default;
	~App() = default;

	void Tick();
	void HandleEvents();
	void ProcessEvent(const sf::Event & event);	// a real event or one Testerino sends
	void Render(const sf::Time delta_time);

	sf::RenderWindow _wnd;
	bool _exit = false;

	sf::Font _font;
	optional<sf::Text> _text;	// RenderText's, made once the font is loaded

	sf::Clock _clock;

	void RenderText(const string & text, const Vector2f pos, const uint size, const sf::Color color = sf::Color::White, const function<void(sf::Text &)> & pre_draw_fnc = nullptr);
};
