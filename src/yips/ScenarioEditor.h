#pragma once
#include "Scene.h"

enum class EditorActionID : u8
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
	std::stack<EditorAction> _list;
};

class BallObstacle;
class Scenario;
class Object;
class ScenarioEditor : public Scene
{
public:
	ScenarioEditor();
	~ScenarioEditor() override;

	void ShiftObstacle(const bool next);

	bool HandleEvents(const sf::Event & event) override;
	void Render(sf::RenderWindow & wnd, const sf::Time delta_time) override;
	void Tick(const sf::Time delta_time) override;

	enum class ToolType : u8
	{
		Position,
		Selection,

		Default = Position,
	};

	enum class SelectionTask : u8
	{
		AlignVertically,		// Put at the same line than first selected
		AlignHorizontally,		// Same but Horizontal
		SeparateVertically,	// Separate between selected by given pixels
		SeparateHorizontally,	// Same but Horizontal
	};

	// The editor's actions, from its menu and from Testerino alike
	void NewScenario(const ChunkIdx chunks);
	bool SaveScenario(const string & file_name);
	bool LoadScenario(const string & file_name);
	// A click on the window (pixels): places an obstacle or (de)selects one, by the tool
	void ClickAt(const sf::Vector2i pixel);
	void SelectAll(const bool selected);
	void RunSelectionTask(const SelectionTask task, const float separation);
	// The scenario being edited, saved or not, in the Scenario scene
	void PlayScenario();
	// The window spot (pixels) of a world point of the scenario
	sf::Vector2i ToPixel(const Vector2f world) const;

	bool IsAnyObjectSelected() const;
	vector<Object *> GetSelectedObjects();
	bool CanSaveAs(const string & name) const;

	unique_ptr<Scenario> _scenario;

	bool _move_up = false;
	bool _move_down = false;

	unique_ptr<BallObstacle> _current_obstacle;

	vector<string> _scenario_names;

	char _new_scenario_name[scenario::kNameSize] = {};
	char _new_scenario_chunks[4] = { "32" };
	char _separation[4] = { "15" };
	string _status;	// the last action's result, shown in the menu

public: // Section: Top menu
	bool _top_menu_opened = true;
	sf::IntRect _top_menu_rect;

	ToolType _tool = ToolType::Default;

	RemakeStack _remake_stack;

private:
	void DrawMenu();
	void DrawPopups();
};
