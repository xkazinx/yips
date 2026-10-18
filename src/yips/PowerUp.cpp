#include "pch.h"
#include "PowerUp.h"
#include "App.h"

PowerUp::PowerUp(const PowerUpID power_up, const Vector2f origin)
	: Object(ObjectID::PowerUp, origin), _power_up(power_up)
{
	_shape.setFillColor(sf::Color(0, 0, 200, 255 / 2));
	_shape.setSize({ kFontSize * 2, kFontSize * 2 });
	_shape.setPosition(origin);
}

string PowerUp::Label(const PowerUpID power_up)
{
	string output;

	for (const char c : enum_name(power_up))
	{
		if (std::isupper((unsigned char)c) || std::isdigit((unsigned char)c))
			output += c;
	}

	return output;
}

void PowerUp::Render(sf::RenderTarget & wnd, const sf::Time delta_time)
{
	wnd.draw(_shape);

	// The scenario's view is turned 180 degrees: the text is turned back
	const Vector2f pos = _shape.getPosition() + Vector2f(kFontSize * 2.f, kFontSize * 2.f);
	g_app->RenderText(Label(_power_up), pos, (uint)kFontSize, sf::Color::White, [](sf::Text & text) {
		text.rotate(sf::degrees(180.f));
	});
}

void PowerUp::Tick(const sf::Time delta_time)
{
}

void PowerUp::HandleCollision(Object * obj)
{
	if (HasExpired())
		return;

	// Taken by the ship; obstacles, projectiles and other power ups pass over it
	if (obj->_id == ObjectID::Ship)
		Expire();
}
