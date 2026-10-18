#include "pch.h"
#include "Sprite.h"

Sprite::Sprite()
	: Object(ObjectID::Sprite)
{
}

bool Sprite::Load(const Path & file)
{
	const bool ok = _texture.loadFromFile(file);
	if (!ok)
		Log::Error("Couldn't load {}", file.string());

	_spr.emplace(_texture);
	return ok;
}

void Sprite::Tick(const sf::Time delta_time)
{
	Object::Tick(delta_time);
}

void Sprite::Move(const float x, const float y)
{
	if (_spr)
		_spr->move({ x, y });
}

Vector2f Sprite::GetPosition()
{
	return _spr ? _spr->getPosition() : Vector2f{};
}

void Sprite::SetPosition(const float x, const float y)
{
	if (_spr)
		_spr->setPosition({ x, y });
}

void Sprite::Render(sf::RenderTarget & wnd, const sf::Time delta_time)
{
	if (_spr)
		wnd.draw(*_spr);
}
