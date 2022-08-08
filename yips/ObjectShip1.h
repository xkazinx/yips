#pragma once
#include "Object.h"

class Sprite;
class ObjectShip1 : public Object
{
public:
	constexpr static auto kShipObjectIndex = 0u;

	ObjectShip1();

	virtual bool HandleEvents(sf::Event & event);
	virtual void Render(sf::RenderTarget & wnd, sf::Time delta_time);
	virtual void Tick(sf::Time delta_time);

	Sprite * _ship_sprite;
};

