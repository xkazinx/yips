#include "pch.h"
#include "ScenarioScene.h"
#include "Background.h"
#include "ObjectShip1.h"
#include "Scenario.h"
#include "BallProjectile.h"
#include "Sprite.h"
#include "BallObstacle.h"
#include "Random.h"
#include "App.h"

ScenarioScene::ScenarioScene() : Scene(SceneID::Scenario)
{
	constexpr auto kChunkCount = 16;
   _scenario = make_unique<Scenario>(kChunkCount);

   Initialize();
}

void ScenarioScene::Initialize()
{
	_scenario->AddObject<Object>(make_unique<ObjectShip1>());
	_ship = (ObjectShip1*)_scenario->_objects.back().get();


	_ability_levels.fill(0);
	_ability_ammo.fill(0);

	_total_energies = 1;
	ChargeAbilityLevel(sf::Keyboard::A);

	ChargeAbilityAmmo(PowerUpID::Ammo_Ability5, 99u);
}

bool ScenarioScene::HandleEvents(sf::Event& event)
{
   switch(event.type)
   {
   case sf::Event::KeyPressed:
   {
      auto is_ctrl = []() { return sf::Keyboard::isKeyPressed(sf::Keyboard::LControl); };
      switch(event.key.code)
      {
      case sf::Keyboard::O:
      {
         auto obs = _scenario->AddObject<BallObstacle>(make_unique<BallObstacle>(Vector2f
            {
            (float)g_rand->Generate<u16>(0u, kViewSizeX), 
            _scenario->_view.getCenter().y + kViewSizeY / 2.f
            }
         ));
         
         obs->_velocity_x = 10.f;
         obs->_velocity_y = 0.f;//(float)g_rand->Generate<u16>(0.f, 1.f);
         obs->_move_type = MoveType::Wave;
         break;
      }

		case sf::Keyboard::L:
      {
			auto obs = _scenario->AddObject<BallObstacle>(make_unique<BallObstacle>(Vector2f
				{
				(float)g_rand->Generate<u16>(0u, kViewSizeX),
				_scenario->_view.getCenter().y + kViewSizeY / 2.f
				}
			));

			obs->_velocity_x = 10.f;
			obs->_velocity_y = 0.f;//(float)g_rand->Generate<u16>(0.f, 1.f);
			obs->_move_type = MoveType::Linear;
			break;
      }

      case sf::Keyboard::Q:
		case sf::Keyboard::W:
		case sf::Keyboard::E:
		case sf::Keyboard::R:
		case sf::Keyboard::A:
      case sf::Keyboard::S:
		case sf::Keyboard::D:
		case sf::Keyboard::F:
         if(is_ctrl())
            ChargeAbilityLevel(event.key.code);
         else
            UseAbility(event.key.code);
			return true;
      }
      break;
   }
   }

   return false;
}

bool ScenarioScene::UseAbility(sf::Keyboard::Key key)
{
	auto ability = KeyToAbility(key);
	if(_ability_ammo[(szt)ability] == 0)
		return false;

	--_ability_ammo[(szt)ability];

   switch(ability)
   {
	case AbilityID::Ability1:
		
		return true;

	case AbilityID::Ability2:

		return true;

	case AbilityID::Ability3:

		return true;

	case AbilityID::Ability4:
		
		return true;

	case AbilityID::Ability5:
	{
		float scale = 0.15f;
		float proj_radius = 10.f;
		_scenario->_projectiles.push_back(make_unique<BallProjectile>(Vector2f
			{
			_ship->_ship_sprite->_spr.getPosition().x - ((_ship->_ship_sprite->_spr.getTexture()->getSize().x * scale) / 2) - proj_radius,
			_ship->_ship_sprite->_spr.getPosition().y
			}
		));
		return true;
	}

	case AbilityID::Ability6:

		return true;

	case AbilityID::Ability7:

		return true;

	case AbilityID::Ability8:

		return true;
	}

	return false;
}

void ScenarioScene::ChargeAbilityLevel(sf::Keyboard::Key key)
{
	if(_total_energies == 0)
		return;

	++_ability_levels[(szt)KeyToAbility(key)];
	--_total_energies; 
}

void ScenarioScene::ChargeAbilityAmmo(PowerUpID power_up, u16 ammo)
{
	auto ability_id = PowerUpToAbility(power_up);
	if(ability_id == AbilityID::None)
		return;

	_ability_ammo[(szt)ability_id] += ammo;
}

AbilityID ScenarioScene::KeyToAbility(sf::Keyboard::Key key)
{
	using Key = sf::Keyboard::Key;
	switch (key)
	{
	case Key::Q:
		return AbilityID::Ability1;

	case Key::W:
		return AbilityID::Ability2;

	case Key::E:
		return AbilityID::Ability3;

	case Key::R:
		return AbilityID::Ability4;

	case Key::A:
		return AbilityID::Ability5;

	case Key::S:
		return AbilityID::Ability6;

	case Key::D:
		return AbilityID::Ability7;

	case Key::F:
		return AbilityID::Ability8;

	default:
		break;
	}

	return AbilityID::None;
}

AbilityID ScenarioScene::PowerUpToAbility(PowerUpID id)
{
	switch (id)
	{
		using enum PowerUpID;
	case Ammo_Ability1:
		return AbilityID::Ability1;

	case Ammo_Ability2:
		return AbilityID::Ability2;

	case Ammo_Ability3:
		return AbilityID::Ability3;

	case Ammo_Ability4:
		return AbilityID::Ability4;

	case Ammo_Ability5:
		return AbilityID::Ability5;

	case Ammo_Ability6:
		return AbilityID::Ability6;

	case Ammo_Ability7:
		return AbilityID::Ability7;

	case Ammo_Ability8:
		return AbilityID::Ability8;

	default:
		break;
	}

	return AbilityID::None;
}

void ScenarioScene::Render(sf::RenderWindow & wnd, sf::Time delta_time)
{
   Object::Render(wnd, delta_time);

   _scenario->Render(wnd, delta_time);

   g_app->_wnd.setView(g_app->_wnd.getDefaultView());
	g_app->RenderText(format("E: {}", _total_energies), { 0.f, g_res_y - 30.f }, 25);

	constexpr auto kShapeSize = 35u;
   vector<string> keys = { "Q", "W", "E", "R", "A", "S", "D", "F" };
   for(szt i = 0u; i < keys.size(); ++i)
	{
		auto total_width = keys.size() * kShapeSize;

      static sf::RectangleShape shape;
      shape.setFillColor({ 255/2, 0, 0, 255/2 });
      shape.setSize({ kShapeSize, kShapeSize });
      shape.setPosition((g_res_x / 2.f - total_width / 2.f) + (i * kShapeSize), g_res_y - (kShapeSize * 2.f));
      shape.setOutlineColor(sf::Color::Green);
      shape.setOutlineThickness(1.f);

      wnd.draw(shape);

		auto pos = shape.getPosition();
		//pos += Vector2f(kShapeSize, kShapeSize);
      g_app->RenderText(keys[i], pos, 30);

		g_app->RenderText(std::to_string(_ability_ammo[i]),
			{ pos.x + (30.f / 2.f), pos.y }, 30 / 2);

		g_app->RenderText(std::to_string(_ability_levels[i]),
			{ pos.x + (30.f / 2.f), pos.y + 15.f }, 30 / 2);
   }
}

void ScenarioScene::Tick(sf::Time delta_time)
{
   _scenario->Tick(delta_time);

   {
      float velocity = 50.f;
      _scenario->Move(0, velocity * delta_time.asSeconds());
   }

   Object::Tick(delta_time);
}
