#include "pch.h"
#include "Sprite.h"

Sprite::Sprite()
	: Object(ObjectID::Sprite) 
{
	Print("Sprite::Sprite()");
}

void Sprite::Tick(sf::Time delta_time)
{
	Object::Tick(delta_time);
}

void Sprite::Move(float x, float y)
{
	_spr.move(x, y);
}

Vector2f Sprite::GetPosition()
{
	return _spr.getPosition();
}

void Sprite::SetPosition(float x, float y)
{
	_spr.setPosition(x, y);
}

void Sprite::Render(sf::RenderTarget& wnd, sf::Time delta_time)
{
	wnd.draw(_spr);
}
