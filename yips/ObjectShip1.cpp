#include "pch.h"
#include "ObjectShip1.h"
#include "Sprite.h"

ObjectShip1::ObjectShip1() 
	: Object(ObjectID::Ship)
{
	_ship_sprite = AddObject<Sprite>(make_unique<Sprite>());
	_ship_sprite->_texture.loadFromFile(kDataFolder + "ship_1.png"s);
	
	const float scale = 0.15f;

	_ship_sprite->_spr.setTexture(_ship_sprite->_texture);
	_ship_sprite->_spr.setScale({ scale, scale });
	_ship_sprite->_spr.setPosition(g_res_x / 2.f, -(kChunkSizeY / 4) + (_ship_sprite->_spr.getTexture()->getSize().y * scale));
}

bool ObjectShip1::HandleEvents(sf::Event & event) 
{ 
	return Object::HandleEvents(event); 
}

void ObjectShip1::Tick(sf::Time delta_time)
{
	{
      float velocity = 50.f;
      _ship_sprite->Move(0, -velocity * delta_time.asSeconds());
   }

	Object::Tick(delta_time);
}

void ObjectShip1::Render(sf::RenderTarget& wnd, sf::Time delta_time)
{
	//wnd.draw(*_test_sprite);

	Object::Render(wnd, delta_time);

	float velocity = 150.f;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		velocity *= 2.f;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		_ship_sprite->Move(velocity * delta_time.asSeconds(), 0.f);
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		_ship_sprite->Move(-velocity * delta_time.asSeconds(), 0.f);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		_ship_sprite->Move(0.f, -velocity * delta_time.asSeconds());
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		_ship_sprite->Move(0.f, velocity * delta_time.asSeconds());
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
