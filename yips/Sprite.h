#pragma once
#include "Object.h"

class Sprite : public Object
{
public:
	Sprite();
	
	void Render(sf::RenderTarget & wnd, sf::Time delta_time);
	void Tick(sf::Time delta_time);

	void Move(float x, float y) override;
	Vector2f GetPosition() override;
	void SetPosition(float x, float y) override;

	sf::Sprite _spr;
	sf::Texture _texture;
};

