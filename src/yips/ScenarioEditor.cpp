#include "pch.h"
#include "ScenarioEditor.h"
#include "BallObstacle.h"
#include "Scenario.h"
#include "App.h"
#include "Scenes.h"
#include "Input.h"

ScenarioEditor::ScenarioEditor() : Scene(SceneID::ScenarioEditor)
{
	ShiftObstacle(true);
	g_app->_wnd.setView(g_app->_wnd.getDefaultView());
}

ScenarioEditor::~ScenarioEditor() = default;

void ScenarioEditor::ShiftObstacle([[maybe_unused]] const bool next)
{
	_current_obstacle = make_unique<BallObstacle>(Vector2f{ 50.f, 50.f });
}

bool ScenarioEditor::HandleEvents(const sf::Event & event)
{
	using Key = sf::Keyboard::Key;

	if (const auto key = event.getIf<sf::Event::KeyReleased>())
	{
		switch (key->code)
		{
		case Key::Up:
			_move_up = false;
			break;
		case Key::Down:
			_move_down = false;
			break;
		case Key::F1:
			_top_menu_opened = !_top_menu_opened;
			break;
		default:
			break;
		}
	}
	else if (const auto key = event.getIf<sf::Event::KeyPressed>())
	{
		// Keys typed into a text box are the box's
		if (ImGui::GetIO().WantTextInput)
			return false;

		switch (key->code)
		{
		case Key::Right:
			ShiftObstacle(true);
			break;
		case Key::Left:
			ShiftObstacle(false);
			break;
		case Key::Up:
			_move_up = true;
			break;
		case Key::Down:
			_move_down = true;
			break;
		case Key::F3:
			if (_scenario)
				_scenario->_debug_overlay = !_scenario->_debug_overlay;
			break;
		case Key::P:
			if (key->control || input::IsCtrl())
				PlayScenario();
			break;
		default:
			break;
		}
	}
	else if (const auto mouse = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mouse->button != sf::Mouse::Button::Left || !_scenario)
			return false;

		// The menu and its popups take their own clicks
		if (ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel))
			return false;

		if (_top_menu_opened && _top_menu_rect.contains(mouse->position))
			return false;

		ClickAt(mouse->position);
		return true;
	}

	return false;
}

void ScenarioEditor::ClickAt(const sf::Vector2i pixel)
{
	if (!_scenario)
		return;

	const Vector2f world = g_app->_wnd.mapPixelToCoords(pixel, _scenario->_view);

	if (_tool == ToolType::Position)
	{
		auto obj = _scenario->AddObject(make_unique<BallObstacle>(world));
		obj->_initial_pos = world;
		_remake_stack._list.push(EditorAction{ EditorActionID::AddObject });
	}
	else if (_tool == ToolType::Selection)
	{
		for (auto & object : _scenario->_objects)
		{
			if (object->IsSelectable() && object->GetCollisionRect().contains(world))
				object->_selected = !object->_selected;
		}
	}
}

sf::Vector2i ScenarioEditor::ToPixel(const Vector2f world) const
{
	return _scenario ? g_app->_wnd.mapCoordsToPixel(world, _scenario->_view) : sf::Vector2i{};
}

void ScenarioEditor::SelectAll(const bool selected)
{
	if (!_scenario)
		return;

	for (auto & object : _scenario->_objects)
	{
		if (object->IsSelectable())
			object->_selected = selected;
	}
}

void ScenarioEditor::NewScenario(const ChunkIdx chunks)
{
	_scenario = make_unique<Scenario>(chunks);
	_status = format("New scenario, {} chunks", _scenario->_chunk_cnt);
}

void ScenarioEditor::RunSelectionTask(const SelectionTask task, const float separation)
{
	auto objects = GetSelectedObjects();
	if (objects.empty())
	{
		_status = "Nothing selected";
		return;
	}

	const bool horizontal = task == SelectionTask::AlignHorizontally || task == SelectionTask::SeparateHorizontally;
	ranges::sort(objects, [horizontal](const auto * left, const auto * right) {
		return horizontal ? left->GetCollisionRect().position.x < right->GetCollisionRect().position.x : left->GetCollisionRect().position.y < right->GetCollisionRect().position.y;
	});

	const auto first = objects[0]->GetCollisionRect();
	float next = horizontal ? first.position.x : first.position.y;

	for (auto * object : objects)
	{
		const auto rect = object->GetCollisionRect();

		switch (task)
		{
		case SelectionTask::SeparateHorizontally:
			object->SetPosition(next, rect.position.y);
			next += rect.size.x + separation;
			break;
		case SelectionTask::SeparateVertically:
			object->SetPosition(rect.position.x, next);
			next += rect.size.y + separation;
			break;
		case SelectionTask::AlignVertically:
			object->SetPosition(first.position.x, rect.position.y);
			break;
		case SelectionTask::AlignHorizontally:
			object->SetPosition(rect.position.x, first.position.y);
			break;
		}

		// Saved where it now stands
		object->_initial_pos = object->GetPosition();
	}

	_remake_stack._list.push(EditorAction{ horizontal ? EditorActionID::AlignHorizontally : EditorActionID::AlignVertically });
	_status = format("{}: {} objects", enum_name(task), objects.size());
}

void ScenarioEditor::Render(sf::RenderWindow & wnd, const sf::Time delta_time)
{
	if (_scenario)
	{
		_scenario->Render(wnd, delta_time);

		if (_tool == ToolType::Position && wnd.hasFocus())
		{
			const auto mpos = wnd.mapPixelToCoords(sf::Mouse::getPosition(wnd), _scenario->_view);
			_current_obstacle->SetPosition(mpos.x, mpos.y);
			_current_obstacle->Render(wnd, delta_time);
		}
	}

	if (_top_menu_opened)
		DrawMenu();
}

void ScenarioEditor::DrawMenu()
{
	ImGui::SetNextWindowPos({ 10.f, 10.f }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Scenario Editor", &_top_menu_opened, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::MenuItem("New"))
		ImGui::OpenPopup("NewScenarioModal");

	if (ImGui::MenuItem("Open", "Ctrl+O"))
	{
		_scenario_names = Misc::FetchFileNamesFromDirectory(ScenarioFolder());
		ImGui::OpenPopup("OpenScenarioModal");
	}

	if (ImGui::MenuItem("Save", "Ctrl+S"))
	{
		if (!_scenario)
			_status = "There's no scenario to save yet!";
		else if (_scenario->_name.empty())
			ImGui::OpenPopup("SaveScenarioModal");
		else
			SaveScenario(_scenario->_name);
	}

	if (ImGui::MenuItem("Save As.."))
	{
		if (!_scenario)
		{
			_status = "There's no scenario to save yet!";
		}
		else
		{
			snprintf(_new_scenario_name, sizeof(_new_scenario_name), "%s", _scenario->_name.c_str());	// truncated to fit (portable strncpy_s _TRUNCATE)
			ImGui::OpenPopup("SaveScenarioModal");
		}
	}

	if (_scenario && ImGui::MenuItem("Play Scenario", "Ctrl+P"))
		PlayScenario();

	if (_scenario && ImGui::TreeNode("Tools"))
	{
		for (const auto tool : enum_values<ToolType>())
		{
			if (ImGui::Selectable(EnumCStr(tool), _tool == tool))
				_tool = tool;
		}

		ImGui::TreePop();
	}

	if (_scenario && ImGui::TreeNode("Selection"))
	{
		for (const auto task : enum_values<SelectionTask>())
		{
			if (ImGui::Button(EnumCStr(task)))
				RunSelectionTask(task, (float)atoi(_separation));
		}

		ImGui::InputText("Separation", _separation, IM_ARRAYSIZE(_separation), ImGuiInputTextFlags_CharsDecimal);
		ImGui::TreePop();
	}

	if (_scenario)
		ImGui::Text("%s: %u chunks, %u obstacles", _scenario->_name.empty() ? "(unsaved)" : _scenario->_name.c_str(), (uint)_scenario->_chunk_cnt, (uint)_scenario->CountObjects(ObjectID::Obstacle));

	if (!_status.empty())
		ImGui::TextUnformatted(_status.c_str());

	_top_menu_rect = { { (int)ImGui::GetWindowPos().x, (int)ImGui::GetWindowPos().y }, { (int)ImGui::GetWindowSize().x, (int)ImGui::GetWindowSize().y } };

	// The popups are opened from inside this window: they live in its ID stack
	DrawPopups();
	ImGui::End();
}

void ScenarioEditor::DrawPopups()
{
	if (ImGui::BeginPopupModal("NewScenarioModal", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("Chunks", _new_scenario_chunks, IM_ARRAYSIZE(_new_scenario_chunks), ImGuiInputTextFlags_CharsDecimal);

		if (ImGui::Button("Create Scenario"))
		{
			NewScenario((ChunkIdx)std::clamp(atoi(_new_scenario_chunks), 1, 999));
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("OpenScenarioModal", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::BeginListBox("Scenarios"))
		{
			for (const string & item_name : _scenario_names)
			{
				if (ImGui::Selectable(item_name.c_str(), false))
				{
					ImGui::CloseCurrentPopup();
					LoadScenario(item_name);
				}
			}

			ImGui::EndListBox();
		}

		if (_scenario_names.empty())
			ImGui::TextUnformatted("No scenarios saved yet");

		if (ImGui::Button("Cancel"))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("SaveScenarioModal", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("Scenario name", _new_scenario_name, IM_ARRAYSIZE(_new_scenario_name));

		if (ImGui::Button("Save Scenario"))
		{
			if (CanSaveAs(_new_scenario_name))
			{
				SaveScenario(_new_scenario_name);
				ImGui::CloseCurrentPopup();
			}
			else
			{
				_status = format("The scenario {} already exists! Try another...", _new_scenario_name);
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
			ImGui::CloseCurrentPopup();

		if (!_status.empty())
			ImGui::TextUnformatted(_status.c_str());

		ImGui::EndPopup();
	}
}

void ScenarioEditor::Tick(const sf::Time delta_time)
{
	if (_status != _status_printed)
	{
		_status_printed = _status;
		Print("Editor: {}", _status);
	}

	// So that the obstacles don't move, the scenario does not tick here
	if (!_scenario)
		return;

	const float velocity = input::IsShift() ? 128.f : 64.f;

	if (_move_up)
		_scenario->Move(0, velocity * delta_time.asSeconds());
	else if (_move_down)
		_scenario->Move(0, -velocity * delta_time.asSeconds());
}

bool ScenarioEditor::CanSaveAs(const string & name) const
{
	// A file name without folders, never an existing scenario other than this one
	if (name.empty() || name.find_first_of("\\/:*?\"<>|") != string::npos || name == "." || name == "..")
		return false;

	if (_scenario && name == _scenario->_name)
		return true;

	const auto names = Misc::FetchFileNamesFromDirectory(ScenarioFolder());
	return ranges::find(names, name) == names.end();
}

bool ScenarioEditor::SaveScenario(const string & file_name)
{
	if (!_scenario)
		return false;

	std::error_code ec;
	std::filesystem::create_directories(ScenarioFolder(), ec);

	const Path path = ScenarioFolder() / file_name;
	ofstream out(path);
	if (!out.is_open())
	{
		_status = format("Couldn't save scenario {}", file_name);
		PrintError("{}", _status);
		return false;
	}

	_scenario->_name = file_name;
	out << _scenario->ToJson().dump(4);
	_status = format("Saved {} ({} obstacles)", file_name, _scenario->CountObjects(ObjectID::Obstacle));
	Log::Debug("{}", _status);
	return true;
}

bool ScenarioEditor::LoadScenario(const string & file_name)
{
	const Json json = LoadJson(ScenarioFolder() / file_name);
	if (json.is_null())
	{
		_status = format("Couldn't open {}", file_name);
		return false;
	}

	_scenario = make_unique<Scenario>(json);
	// A file renamed by hand plays under its file's name
	_scenario->_name = file_name;
	_status = format("Opened {} ({} obstacles)", file_name, _scenario->CountObjects(ObjectID::Obstacle));
	return true;
}

bool ScenarioEditor::IsAnyObjectSelected() const
{
	if (!_scenario)
		return false;

	return ranges::any_of(_scenario->_objects, [](const auto & o) { return o->IsSelectable() && o->_selected; });
}

vector<Object *> ScenarioEditor::GetSelectedObjects()
{
	vector<Object *> objects;
	if (!_scenario)
		return objects;

	for (auto & object : _scenario->_objects)
	{
		if (object->IsSelectable() && object->_selected)
			objects += object.get();
	}

	return objects;
}

void ScenarioEditor::PlayScenario()
{
	if (!_scenario)
	{
		_status = "There's no scenario to play yet!";
		return;
	}

	g_scene->Play(_scenario->ToJson());
}
