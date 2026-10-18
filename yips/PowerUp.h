#pragma once
#include "Object.h"

class PowerUp : public Object
{
public:
	PowerUp(PowerUpID id, Vector2f origin);

	void Render(sf::RenderTarget& wnd, sf::Time delta_time) override;
	void Tick(sf::Time delta_time) override;

	static constexpr auto kFontSize = 20.f;

	void Move(float x, float y) override
	{
		_shape.move(x, y);
	}
	Vector2f GetPosition() override
	{
		return _shape.getPosition();
	}
	void SetPosition(float x, float y) override
	{
		_shape.setPosition(x, y);
	}

	sf::FloatRect GetCollisionRect() const override
	{
		return _shape.getGlobalBounds();
	}

	void HandleCollision(Object * obj) override;

	sf::RectangleShape _shape;

	PowerUpID _id;
};

