#pragma once
#include "Scene.h"
#include "Object.h"

class ObjectShip1;
class Scenario;

class ScenarioScene : public Scene, public Object
{
public:
	ScenarioScene();

	bool HandleEvents(sf::Event & event) override;
	void Render(sf::RenderWindow & wnd, sf::Time delta_time) override;
	void Tick(sf::Time delta_time) override;

	unique_ptr<ObjectShip1> _ship;

	unique_ptr<Scenario> _scenario;
};

