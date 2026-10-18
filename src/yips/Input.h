#pragma once

// Keys held down: the real keyboard while the window has focus, plus the keys a
// Testerino routine holds (so a test drives the ship in a window behind the others)
namespace input
{
	bool IsDown(const sf::Keyboard::Key key);
	inline bool IsCtrl() { return IsDown(sf::Keyboard::Key::LControl) || IsDown(sf::Keyboard::Key::RControl); }
	inline bool IsShift() { return IsDown(sf::Keyboard::Key::LShift) || IsDown(sf::Keyboard::Key::RShift); }

	void Hold(const sf::Keyboard::Key key, const bool down);
	void ReleaseAll();
	bool IsHeld(const sf::Keyboard::Key key);
}
