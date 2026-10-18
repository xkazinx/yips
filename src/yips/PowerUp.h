#pragma once
#include "Object.h"

class PowerUp : public Object
{
public:
	PowerUp(const PowerUpID power_up, const Vector2f origin);

	void Render(sf::RenderTarget & wnd, const sf::Time delta_time) override;
	void Tick(const sf::Time delta_time) override;

	static constexpr auto kFontSize = 20.f;

	void Move(const float x, const float y) override
	{
		_shape.move({ x, y });
		Object::Move(x, y);
	}
	Vector2f GetPosition() override
	{
		return _shape.getPosition();
	}
	void SetPosition(const float x, const float y) override
	{
		_shape.setPosition({ x, y });
		Object::SetPosition(x, y);
	}

	sf::FloatRect GetCollisionRect() const override
	{
		return _shape.getGlobalBounds();
	}

	void HandleCollision(Object * obj) override;

	// The label drawn on it: the name's capitals and digits (Ammo_Ability5: AA5)
	static string Label(const PowerUpID power_up);

	sf::RectangleShape _shape;

	PowerUpID _power_up;
};
