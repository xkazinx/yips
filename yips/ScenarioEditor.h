#pragma once
#include "Scene.h"

enum class EditorActionID
{
	AddObject,
	AlignVertically,
	AlignHorizontally,
};

class EditorAction 
{
public:
	EditorActionID _id;
};

class RemakeStack
{
public:
	struct Action
	{
		EditorAction _id;
		//std::vector<unique_ptr<Object>> _objects;
	};

	std::stack<EditorAction> _list;
};

class BallObstacle;
class Scenario;
class Object;
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

	unique_ptr<BallObstacle> _current_obstacle;

	vector<string> _scenario_names;

	// #move
	char _new_scenario_name[scenario::kNameSize] = {};

public: // Section: Top menu
	bool _top_menu_opened = true;
	sf::Rect<int> _top_menu_rect;

	enum class ToolType
	{
		Position,
		Selection,

		Default = Position,
	};

	enum class SelectionTask
	{
		AlignVertically, // Put at the same line than first selected
		AlignHorizontally, // Same but Horizontal
		SeparateVertically, // Separate between selected by given pixels 
		SeparateHorizontally, // Same but Horizontal
		//CenterHorizontally,
		//CenterVertically,
	};

	ToolType _tool = ToolType::Default;

	bool IsAnyObjectSelected() const;
	vector<Object*> GetSelectedObjects();

	RemakeStack _remake_stack;

	void PlayScenario();
};

