#include "pch.h"
#include "PowerUp.h"
#include "App.h"

PowerUp::PowerUp(PowerUpID id, Vector2f origin) 
	: Object(ObjectID::PowerUp, origin), _id(id)
{
	_shape.setFillColor(sf::Color(0, 0, 200, 255/2));
	_shape.setSize({ kFontSize * 2, kFontSize * 2 });
	_shape.setPosition(origin.x, origin.y);
}

void PowerUp::Render(sf::RenderTarget& wnd, sf::Time delta_time)
{
	wnd.draw(_shape);

	auto enum_name_to_capitals_and_numbers = [&](string name)
	{
		string output;
		for(auto & c : name)
		{
			if(std::isupper(c) || std::isdigit(c))
				output += c;
		}

		return output;
	};
	
	{
		auto pos = _shape.getPosition();
		pos += Vector2f(kFontSize * 2.f, kFontSize * 2.f);

		g_app->RenderText(
			enum_name_to_capitals_and_numbers(enum_name(_id).data()), 
			pos, (uint)kFontSize, sf::Color::White, [&](sf::Text & text)
			{
				text.rotate(180);
			});
	}
	/*{
		auto rect = _shape.getGlobalBounds();
		auto current_view = const_cast<sf::View&>(g_app->_wnd.getView());
		current_view.rotate(180.f);
		g_app->_wnd.setView(current_view);
		
		g_app->RenderText(letters, _shape.getPosition(), 25u);
		g_app->RenderText(letters, { _shape.getGlobalBounds().left, _shape.getGlobalBounds().top }, 25u);

		auto pixel = g_app->_wnd.mapCoordsToPixel({ rect.left, rect.top });
		auto coords = g_app->_wnd.mapPixelToCoords({ pixel.x, pixel.y });

		//g_app->RenderText(letters, { (float)pixel.x, (float)pixel.y }, 25u);
		//g_app->RenderText(letters, coords, 25u);

		current_view.rotate(-180.f);
		g_app->_wnd.setView(current_view);
	}*/
}

void PowerUp::Tick(sf::Time delta_time)
{

}

void PowerUp::HandleCollision(Object * obj)
{
	if (HasExpired())
		return;

	Log::Debug("PowerUp::HandleCollision(ObjectID({}))", enum_name(obj->_id));

	using enum ObjectID;
	switch (obj->_id)
	{
	case Ship:
		// power up
		Log::Debug("PowerUp collided with Ship!");
		Expire();
		break;
	case Sprite:
	case Object:
		break;
	case Obstacle:
	case Projectile:
	case PowerUp:
		return;
	case None:
		break;
	default:
		break;
	}

	Object::HandleCollision(obj);
}
