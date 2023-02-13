#include "pch.h"
#include "ScenarioScene.h"
#include "Background.h"
#include "ObjectShip1.h"
#include "Scenario.h"
#include "BallProjectile.h"
#include "Sprite.h"
#include "BallObstacle.h"
#include "Random.h"

ScenarioScene::ScenarioScene() : Scene(SceneID::Scenario)
{
   _ship = make_unique<ObjectShip1>();

   constexpr auto kChunkCount = 16;
   _scenario = make_unique<Scenario>(kChunkCount);

   _scenario->AddObject<Obstacle>(make_unique<BallObstacle>(Vector2f{ kChunkSizeX / 2, -(kChunkSizeY / 2 )}));
}

bool ScenarioScene::HandleEvents(sf::Event& event)
{
   switch(event.type)
   {
   case sf::Event::KeyPressed:
      switch(event.key.code)
      {
      case sf::Keyboard::A:
      {
         float scale = 0.15f;
         float proj_radius = 10.f;
         _scenario->_projectiles.push_back(make_unique<BallProjectile>(Vector2f
            {
            _ship->_ship_sprite->_spr.getPosition().x + ((_ship->_ship_sprite->_spr.getTexture()->getSize().x * scale) / 2) - proj_radius,
            _ship->_ship_sprite->_spr.getPosition().y 
            }
         ));
         return true;
      }

      case sf::Keyboard::O:
         auto obs = _scenario->AddObject<BallObstacle>(make_unique<BallObstacle>(Vector2f
            {
            (float)g_rand->Generate<u16>(0u, kChunkSizeX), 
            _scenario->_view.getCenter().y - kChunkSizeY / 2
            }
         ));
         
         obs->_velocity_x = 10.f;
         obs->_velocity_y = 0.f;//(float)g_rand->Generate<u16>(0.f, 1.f);
         obs->_move_type = MoveType::Wave;

         break;
      }
      break;
   }

   return false;
}

void ScenarioScene::Render(sf::RenderWindow & wnd, sf::Time delta_time)
{
   Object::Render(wnd, delta_time);

   _scenario->Render(wnd, delta_time);

   _ship->Render(wnd, delta_time);
}

void ScenarioScene::Tick(sf::Time delta_time)
{
   _scenario->Tick(delta_time);

   {
      float velocity = 50.f;
      _scenario->Move(0, -velocity * delta_time.asSeconds());
   }

   _ship->Tick(delta_time);

   Object::Tick(delta_time);
}
