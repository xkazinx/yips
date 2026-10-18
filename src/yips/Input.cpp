#include "pch.h"
#include "Input.h"
#include "App.h"

namespace input
{
	namespace
	{
		bitset<sf::Keyboard::KeyCount> g_held;

		bool InRange(const sf::Keyboard::Key key)
		{
			return Under(key) >= 0 && Under(key) < sf::Keyboard::KeyCount;
		}
	}

	bool IsDown(const sf::Keyboard::Key key)
	{
		if (!InRange(key))
			return false;

		if (g_held[Under(key)])
			return true;

		return g_app && g_app->_wnd.hasFocus() && sf::Keyboard::isKeyPressed(key);
	}

	void Hold(const sf::Keyboard::Key key, const bool down)
	{
		if (InRange(key))
			g_held[Under(key)] = down;
	}

	void ReleaseAll()
	{
		g_held.reset();
	}

	bool IsHeld(const sf::Keyboard::Key key)
	{
		return InRange(key) && g_held[Under(key)];
	}
}
