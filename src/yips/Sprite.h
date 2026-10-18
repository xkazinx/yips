#pragma once
#include "Object.h"

class Sprite : public Object
{
public:
	Sprite();

	// Loads the texture and makes the sprite on it (SFML 3: a sprite always has one)
	bool Load(const Path & file);

	void Render(sf::RenderTarget & wnd, const sf::Time delta_time) override;
	void Tick(const sf::Time delta_time) override;

	void Move(const float x, const float y) override;
	Vector2f GetPosition() override;
	void SetPosition(const float x, const float y) override;

	sf::Texture _texture;
	optional<sf::Sprite> _spr;
};
