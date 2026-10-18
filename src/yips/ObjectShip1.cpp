#include "pch.h"
#include "ObjectShip1.h"
#include "Sprite.h"
#include "PowerUp.h"
#include "Scenes.h"
#include "ScenarioScene.h"
#include "Input.h"

ObjectShip1::ObjectShip1()
	: Object(ObjectID::Ship)
{
	_ship_sprite = AddObject(make_unique<Sprite>());
	_ship_sprite->Load(DataFolder() / "ship_1.png");

	auto & spr = *_ship_sprite->_spr;
	spr.setScale({ kScale, kScale });
	spr.setPosition({ g_res_x / 2.f, 15.f + _ship_sprite->_texture.getSize().y * kScale });
	spr.setRotation(sf::degrees(180.f));
}

bool ObjectShip1::HandleEvents(const sf::Event & event)
{
	return Object::HandleEvents(event);
}

void ObjectShip1::Tick(const sf::Time delta_time)
{
	const float dt = delta_time.asSeconds();
	_ship_sprite->Move(0.f, kScrollVelocity * dt);

	// The view is turned 180 degrees: right on screen is -x, up is +y
	const float velocity = input::IsShift() ? kVelocity * 2.f : kVelocity;

	if (input::IsDown(sf::Keyboard::Key::Right))
		_ship_sprite->Move(-velocity * dt, 0.f);

	if (input::IsDown(sf::Keyboard::Key::Left))
		_ship_sprite->Move(velocity * dt, 0.f);

	if (input::IsDown(sf::Keyboard::Key::Up))
		_ship_sprite->Move(0.f, velocity * dt);

	if (input::IsDown(sf::Keyboard::Key::Down))
		_ship_sprite->Move(0.f, -velocity * dt);

	Object::Tick(delta_time);
}

void ObjectShip1::Move(const float x, const float y)
{
	_ship_sprite->Move(x, y);
}

Vector2f ObjectShip1::GetPosition()
{
	return _ship_sprite->GetPosition();
}

void ObjectShip1::SetPosition(const float x, const float y)
{
	_ship_sprite->SetPosition(x, y);
}

sf::FloatRect ObjectShip1::GetCollisionRect() const
{
	return _ship_sprite->_spr ? _ship_sprite->_spr->getGlobalBounds() : sf::FloatRect{};
}

Vector2f ObjectShip1::Muzzle(const float projectile_radius) const
{
	const sf::FloatRect rect = GetCollisionRect();
	return { rect.position.x + rect.size.x / 2.f - projectile_radius, rect.position.y + rect.size.y };
}

void ObjectShip1::HandleCollision(Object * obj)
{
	if (HasExpired() || obj->_id != ObjectID::PowerUp || obj->HasExpired())
		return;

	auto scene = g_scene->Get<ScenarioScene>();
	if (!scene)
		return;

	const PowerUpID power_up = obj->Get<PowerUp>()->_power_up;
	++scene->_power_ups_taken;
	Log::Debug("Ship took the power up {}", enum_name(power_up));

	switch (power_up)
	{
		using enum PowerUpID;
	case Energy:
		scene->_total_energies++;
		break;
	case Velocity:
	case Invincible:
		_power_ups += power_up;
		break;

	case Ammo_Ability1:
	case Ammo_Ability2:
	case Ammo_Ability3:
	case Ammo_Ability4:
	case Ammo_Ability5:
	case Ammo_Ability6:
	case Ammo_Ability7:
	case Ammo_Ability8:
		scene->ChargeAbilityAmmo(power_up, 3);
		break;

	default:
		break;
	}
}

void ObjectShip1::Render(sf::RenderTarget & wnd, const sf::Time delta_time)
{
	Object::Render(wnd, delta_time);
}
