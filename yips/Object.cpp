#include "pch.h"
#include "Object.h"
#include "Sprite.h"
#include "Scenario.h"
#include "BallProjectile.h"
#include "Scenes.h"

void Object::Tick(sf::Time delta_time)
{
	for(auto & object : _objects)
	{
		switch(object->_id)
		{
		case ID::None:
		default:
			object->Tick(delta_time);
			break;
		}
	}
}

void Object::Render(sf::RenderTarget& wnd, sf::Time delta_time)
{
	for(auto & object : _objects)
	{
		switch(object->_id)
		{
		case ObjectID::Sprite:
			wnd.draw(((Sprite*)(object.get()))->_spr);
			break;

		case ObjectID::Obstacle:
		case ObjectID::Object:
		default:
			object->Render(wnd, delta_time);
			break;
		}
	}
}

void Object::Move(float x, float y)
{
	switch(_id)
	{
	case ObjectID::Sprite:
		((Sprite*)(this))->_spr.move(x, y);
		break;

	default:
		break;
	}

	_pos.x += x;
	_pos.y += y;
}

void Object::SetPosition(float x, float y)
{
	switch(_id)
	{
	case ObjectID::Sprite:
		return ((Sprite*)(this))->_spr.setPosition(x, y);
		
	default:
		break;
	}

	_pos = { x, y };
}

Vector2f Object::GetPosition()
{
	switch(_id)
	{
	case ObjectID::Sprite:
		return ((Sprite*)(this))->_spr.getPosition();

	default:
		break;	
	}

	return _pos;
}
