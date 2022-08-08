#include "pch.h"
#include "Object.h"
#include "Sprite.h"
#include "Chunk.h"
#include "Scenario.h"

void Object::Tick(sf::Time delta_time)
{
	for(auto & object : _objects)
	{
		switch(object->_id)
		{
		case ObjectID::Projectile:
			object->Tick(delta_time);
			break;

		case ObjectID::Object:
		case ObjectID::Obstacle:
		default:
			if (_id == ObjectID::Chunk)
			{
				float x = object->_initial_pos.x, y = object->_initial_pos.y;
				auto * chunk = (Chunk*)this;
				if (chunk->_idx == Scenario::Current->_current_chunk)
				{
					// both work
					y = abs(Scenario::Current->_camera_y + Scenario::Current->GetChunkWorldPos().y) + object->_initial_pos.y; // line formula
					//y = { abs(Scenario::Current->_camera_y + (chunk->_origin_y - Scenario::Current->_camera_y) + (chunk->_idx * kChunkSizeY)) + object->_initial_pos.y + (abs((int)Scenario::Current->_camera_y % (int)kChunkSizeY)) };
				}
				else if (chunk->_idx == Scenario::Current->_current_chunk + 1)
				{
					y = { chunk->_origin_y + abs(Scenario::Current->_camera_y) + object->_initial_pos.y };
				}

				object->SetPosition(x, y);
			}

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

	case ObjectID::Object:
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
	case ObjectID::Ship:
		break;

	case ObjectID::Sprite:
		return ((Sprite*)(this))->_spr.setPosition(x, y);
		
	case ObjectID::Object:
	default:
		break;
	}

	_pos = { x, y };
}

Vector2f Object::GetPosition()
{
	switch(_id)
	{
	case ObjectID::Ship:
		break;

	case ObjectID::Sprite:
		return ((Sprite*)(this))->_spr.getPosition();

	case ObjectID::Object:
	default:
		return _pos;
	}

	return {0.f, 0.f};
}
