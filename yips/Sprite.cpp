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

void Sprite::Render(sf::RenderTarget& wnd, sf::Time delta_time)
{
	wnd.draw(_spr);
}
