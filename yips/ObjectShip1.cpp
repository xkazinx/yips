#include "pch.h"
#include "ObjectShip1.h"
#include "Sprite.h"
#include "Item.h"

ObjectShip1::ObjectShip1() 
	: Object(ObjectID::Ship)
{
	_ship_sprite = AddObject<Sprite>(make_unique<Sprite>());
	_ship_sprite->_texture.loadFromFile(kDataFolder + "ship_1.png"s);
	
	const float scale = 0.15f;

	_ship_sprite->_spr.setTexture(_ship_sprite->_texture);
	_ship_sprite->_spr.setScale({ scale, scale });
	_ship_sprite->_spr.setPosition(g_res_x / 2.f, 15.f + (_ship_sprite->_spr.getTexture()->getSize().y * scale));
	_ship_sprite->_spr.setRotation(180);
}

bool ObjectShip1::HandleEvents(sf::Event & event) 
{ 
	return Object::HandleEvents(event); 
}

void ObjectShip1::Tick(sf::Time delta_time)
{
	{
      float velocity = 50.f;
      _ship_sprite->Move(0, velocity * delta_time.asSeconds());
   }

	Object::Tick(delta_time);
}

void ObjectShip1::Move(float x, float y)
{
	_ship_sprite->Move(x, y);
}

Vector2f ObjectShip1::GetPosition()
{
	return _ship_sprite->GetPosition();
}

void ObjectShip1::SetPosition(float x, float y)
{
	_ship_sprite->SetPosition(x, y);
}

sf::FloatRect ObjectShip1::GetCollisionRect() const
{
	return _ship_sprite->_spr.getGlobalBounds();
}

void ObjectShip1::HandleCollision(Object* obj)
{
	if (HasExpired())
		return;

	Log::Debug("HandleCollision(ObjectID({}))", enum_name(obj->_id));

	using enum ObjectID;
	switch (obj->_id)
	{
	case Ship:
		break;
	case Sprite:
		break;
	case Object:
		break;
	case Obstacle:
		break;
	case Projectile:
		break;
	case Item:
		Log::Debug("Ship collided with Item!");
		switch (obj->Get<::Item>()->_type)
		{
		using enum ItemType;
		case GreenEnergy:
			break;
		case Velocity:
			_power_ups.push_back(PowerUp::Velocity);
			break;
		case Invincible:
			break;
		}
		break;
	case None:
		break;
	default:
		break;
	}

	Object::HandleCollision(obj);
}

void ObjectShip1::Render(sf::RenderTarget& wnd, sf::Time delta_time)
{
	//wnd.draw(*_test_sprite);

	Object::Render(wnd, delta_time);

	float velocity = 250.f;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || IsPowerUp(PowerUp::Velocity))
		velocity *= 2.f;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		_ship_sprite->Move(-velocity * delta_time.asSeconds(), 0.f);
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		_ship_sprite->Move(velocity * delta_time.asSeconds(), 0.f);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		_ship_sprite->Move(0.f, velocity * delta_time.asSeconds());
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		_ship_sprite->Move(0.f, -velocity * delta_time.asSeconds());
	}
	/*
	auto rect = _ship_sprite->_spr.getGlobalBounds(); // make GetRect function
	if(rect.left + rect.width > g_res_x)
	{
		_ship_sprite->SetPosition(g_res_x - rect.width, _ship_sprite->GetPosition().y);
	}

	if (rect.left < 0)
	{
		_ship_sprite->SetPosition(0.f, _ship_sprite->GetPosition().y);
	}
	
	if (rect.top + rect.height > g_res_y)
	{
		_ship_sprite->SetPosition(_ship_sprite->GetPosition().x, g_res_y - rect.height);
	}

	if (rect.top < 0)
	{
		_ship_sprite->SetPosition(_ship_sprite->GetPosition().x, 0);
	}*/
}
