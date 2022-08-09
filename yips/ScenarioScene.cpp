#include "pch.h"
#include "ScenarioScene.h"
#include "Background.h"
#include "ObjectShip1.h"
#include "Scenario.h"
#include "Chunk.h"
#include "Projectile1.h"
#include "Sprite.h"
#include "Obstacle1.h"

ScenarioScene::ScenarioScene()
{
   _ship = make_unique<ObjectShip1>();

   _scenario = make_unique<Scenario>(kChunkSizeY * 15);

   _scenario->AddObject(make_unique<Obstacle1>(kChunkSizeX / 2, -(kChunkSizeY / 2)));
}

bool ScenarioScene::HandleEvents(sf::Event& event)
{
   switch(event.type)
   {
   case sf::Event::KeyPressed:
      switch(event.key.code)
      {
      case sf::Keyboard::A:
         _scenario->_projectiles.push_back(make_unique<Projectile1>(
            _ship->_ship_sprite->_spr.getPosition().x,
            _ship->_ship_sprite->_spr.getPosition().y
         ));
         return true;
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
