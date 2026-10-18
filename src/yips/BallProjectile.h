#pragma once
#include "Projectile.h"

class BallProjectile : public Projectile
{
public:
	static constexpr float kRadius = 10.f;
	static constexpr float kVelocity = 50.f + 700.f;	// the background's speed and its own

	explicit BallProjectile(const Vector2f origin);

	void Tick(const sf::Time delta_time) override;
};
