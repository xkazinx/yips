#include "pch.h"
#include "Scenes.h"
#include "StartScene.h"
#include "ScenarioScene.h"
#include "ScenarioEditor.h"

Scenes::Scenes() : Scene(SceneID::None)
{
	_ptr = make_unique<StartScene>();
	Print("Scene: {}", enum_name(SceneID::Start));
}

bool Scenes::HandleEvents(const sf::Event & event)
{
	return _ptr->HandleEvents(event);
}

void Scenes::Render(sf::RenderWindow & wnd, const sf::Time delta_time)
{
	_ptr->Render(wnd, delta_time);
}

void Scenes::Tick(const sf::Time delta_time)
{
	_ptr->Tick(delta_time);
}

void Scenes::Set(const SceneID id)
{
	_pending = id;
	_pending_scenario.reset();
}

void Scenes::Play(Json scenario)
{
	_pending = SceneID::Scenario;
	_pending_scenario = std::move(scenario);
}

void Scenes::ApplyPending()
{
	if (_pending == SceneID::None)
		return;

	const SceneID id = _pending;
	_pending = SceneID::None;
	// The old scene goes first: its scenario owns the view the new one sets
	_ptr.reset();

	switch (id)
	{
	case SceneID::Start:
		_ptr = make_unique<StartScene>();
		break;
	case SceneID::Scenario:
		_ptr = _pending_scenario ? make_unique<ScenarioScene>(*_pending_scenario) : make_unique<ScenarioScene>();
		break;
	case SceneID::ScenarioEditor:
		_ptr = make_unique<ScenarioEditor>();
		break;
	default:
		_ptr = make_unique<StartScene>();
		break;
	}

	_pending_scenario.reset();
	Print("Scene: {}", enum_name(id));
}
