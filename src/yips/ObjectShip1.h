#pragma once
#include "Object.h"

class Sprite;
class ObjectShip1 : public Object
{
public:
	static constexpr float kScale = 0.15f;
	static constexpr float kVelocity = 250.f;	// Shift doubles it
	static constexpr float kScrollVelocity = 50.f;	// the background's: the ship keeps its place on screen

	ObjectShip1();

	bool HandleEvents(const sf::Event & event) override;
	void Render(sf::RenderTarget & wnd, const sf::Time delta_time) override;
	void Tick(const sf::Time delta_time) override;

	void Move(const float x, const float y) override;
	Vector2f GetPosition() override;
	void SetPosition(const float x, const float y) override;

	sf::FloatRect GetCollisionRect() const override;

	void HandleCollision(Object * obj) override;

	// Where a shot leaves: the middle of the ship's nose
	Vector2f Muzzle(const float projectile_radius) const;

	Sprite * _ship_sprite;

	vector<PowerUpID> _power_ups;

	bool IsPowerUp(const PowerUpID power_up) const
	{
		return ranges::find(_power_ups, power_up) != _power_ups.end();
	}
};
