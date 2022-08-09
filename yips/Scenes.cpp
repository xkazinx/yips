#include "pch.h"
#include "Scenes.h"
#include "StartScene.h"
#include "ScenarioScene.h"
#include "ScenarioEditor.h"
#include "Obstacle1.h"
#include "Background.h"
#include "Scenario.h"
#include "Chunk.h"
#include "ObjectShip1.h"

Scenes::Scenes()
{
	_ptr = make_unique<StartScene>();
}

bool Scenes::HandleEvents(sf::Event & event)
{
	return _ptr->HandleEvents(event);
}

void Scenes::Render(sf::RenderWindow & wnd, sf::Time delta_time)
{
	_ptr->Render(wnd, delta_time);
}

void Scenes::Tick(sf::Time delta_time)
{
	_ptr->Tick(delta_time);
}

void Scenes::Set(SceneID id)
{
	switch(id)
	{
	case SceneID::Start:
		_ptr = make_unique<StartScene>();
		break;
	case SceneID::Scenario:
		_ptr = make_unique<ScenarioScene>();
		break;
	case SceneID::ScenarioEditor:
		_ptr = make_unique<ScenarioEditor>();
		break;
	}
}
