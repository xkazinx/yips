#pragma once
#include "Scene.h"

class Obstacle1;
class Scenario;
class ScenarioEditor : public Scene
{
public:
	ScenarioEditor();

	void ShiftObstacle(bool next);

	bool HandleEvents(sf::Event & event) override;
	void Render(sf::RenderWindow & wnd, sf::Time delta_time) override;
	void Tick(sf::Time delta_time) override;

	unique_ptr<Scenario> _scenario;
	void SaveScenario(string file_name);
	void LoadScenario(string file_name);

	bool _move_up = false;
	bool _move_down = false;

	unique_ptr<Obstacle1> _current_obstacle;
};

