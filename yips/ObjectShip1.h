#pragma once
#include "Object.h"

class Sprite;
class ObjectShip1 : public Object
{
public:
	constexpr static auto kShipObjectIndex = 0u;

	ObjectShip1();

	bool HandleEvents(sf::Event & event) override;
	void Render(sf::RenderTarget & wnd, sf::Time delta_time) override;
	void Tick(sf::Time delta_time) override;

	void Move(float x, float y) override;
	Vector2f GetPosition() override;
	void SetPosition(float x, float y) override;

	sf::FloatRect GetCollisionRect() const override;

	virtual void HandleCollision(Object * obj);

	Sprite * _ship_sprite;

	vector<PowerUpID> _power_ups;

	bool IsPowerUp(PowerUpID power_up)
	{
		return ranges::find(_power_ups, power_up) != _power_ups.end();
	}

	
};

