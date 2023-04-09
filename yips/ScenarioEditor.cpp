#include "pch.h"
#include "ScenarioEditor.h"
#include "BallObstacle.h"
#include "Scenario.h"
#include "Background.h"
#include "App.h"
#include "BallProjectile.h"
#include "Scenes.h"
#include "ScenarioScene.h"

ScenarioEditor::ScenarioEditor() : Scene(SceneID::ScenarioEditor)
{
	ShiftObstacle(true);
}

void ScenarioEditor::ShiftObstacle([[maybe_unused]] bool next)
{
	_current_obstacle = make_unique<BallObstacle>(Vector2f{ 50.f, 50.f });
}

bool ScenarioEditor::HandleEvents(sf::Event& event)
{
	switch(event.type)
	{
	case sf::Event::EventType::KeyReleased:
		switch(event.key.code)
		{
		case sf::Keyboard::Up:
			_move_up = false;
			break;

		case sf::Keyboard::Down:
			_move_down = false;
			break;

		case sf::Keyboard::F1:
			_top_menu_opened = !_top_menu_opened;
			break;
		}
		break;

	case sf::Event::EventType::KeyPressed:
		switch(event.key.code)
		{
		case sf::Keyboard::Right:
			ShiftObstacle(true);
			break;

		case sf::Keyboard::Left:
			ShiftObstacle(false);
			break;

		case sf::Keyboard::Up:
			_move_up = true;
			break;

		case sf::Keyboard::Down:
			_move_down = true;
			break;

		case sf::Keyboard::P:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
				PlayScenario();
			break;
		}
		break;

	case sf::Event::EventType::MouseButtonPressed:
		if(event.mouseButton.button == sf::Mouse::Left)
		{
			if(!_scenario)
				return false;

			// TODO: Replace by something else
			if (ImGui::IsPopupOpen("NewScenarioModal", ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel) ||
				ImGui::IsPopupOpen("OpenScenarioModal", ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel) || 
				ImGui::IsPopupOpen("SaveScenarioModal", ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel))
			{
				return false;
			}

			auto mp = sf::Mouse::getPosition(g_app->_wnd);
			auto mc = g_app->_wnd.mapPixelToCoords(sf::Mouse::getPosition(g_app->_wnd));
			auto event_mp = sf::Vector2i{ event.mouseButton.x, event.mouseButton.y};
			auto event_mc = g_app->_wnd.mapPixelToCoords(event_mp);

			if(_top_menu_rect.contains(mp))
			{
				Log::Debug("Top menu rect contains mouse!");
				return false;
			}

			if(_tool == ToolType::Position)
			{
				PrintDebug("Clicked event.mouseButton: {}/{} | event.mouseButton: {}/{}", 
						event.mouseButton.x, event.mouseButton.y, event_mc.x, event_mc.y);
			
				auto obj = _scenario->AddObject(make_unique<BallObstacle>(event_mc));

				obj->_initial_pos = event_mc;

				{
					_remake_stack._list.push(EditorAction{ EditorActionID::AddObject });
					auto last_inserted_element = _remake_stack._list.top();
				}
			}
			else if(_tool == ToolType::Selection)
			{
				for(auto & object : _scenario->_objects)
				{
					if(!object->IsSelectable())
						continue;

					auto rect = object->GetCollisionRect();
					Log::Debug("rect: {} {} {} {}, mp: {} {}, mc: {} {}",
						rect.left, rect.top, rect.width, rect.height,
						event_mp.x, event_mp.y, 
						event_mc.x, event_mc.y
					);

					//rect.width += rect.left;
					//rect.height += rect.top;

					if(rect.contains(mc))
					{
						Log::Debug("Selected!");
						object->_selected = !object->_selected;
					}
				}
			}
		}
		break;
	}
	return false;
}

void ScenarioEditor::Render(sf::RenderWindow & wnd, sf::Time delta_time)
{
	if(_scenario)
	{
		_scenario->Render(wnd, delta_time);
	
		auto mpos = wnd.mapPixelToCoords(sf::Mouse::getPosition(g_app->_wnd));
		if(_tool == ToolType::Position)
		{
			_current_obstacle->SetPosition(mpos.x, mpos.y);
			_current_obstacle->Render(wnd, delta_time);
		}
		else if(_tool == ToolType::Selection)
		{
		
		}
	}

	/**
	 * UI
	 */
	if(_top_menu_opened)
	{
		if (ImGui::MenuItem("New")) 
		{	
			ImGui::OpenPopup("NewScenarioModal");
		}
		if (ImGui::MenuItem("Open", "Ctrl+O")) 
		{
			// read files from "scenario" folder
			_scenario_names = Misc::FetchFileNamesFromDirectory(kDataFolder + "scenario\\"s);
			//_scenario_names.push_back("1_name");
			//_scenario_names.push_back("2_name");
			//_scenario_names.push_back("3_name");

			ImGui::OpenPopup("OpenScenarioModal");
		}

		if (ImGui::MenuItem("Save", "Ctrl+S")) 
		{
			if(!_scenario)
			{
				Log::Error("There's no scenario to save yet!");
			}
			else
			{
				if(_scenario->_name.empty())
				{
					ImGui::OpenPopup("SaveScenarioModal");
				}
				else
				{
					SaveScenario(_scenario->_name);
				}
			}
		}

		if (ImGui::MenuItem("Save As..")) 
		{
			if(!_scenario)
			{
				Log::Error("There's no scenario to save yet!");
			}
			else
			{
				if(!_scenario->_name.empty())
				{
					strcpy_s(_new_scenario_name, _scenario->_name.c_str());
				}

				ImGui::OpenPopup("SaveScenarioModal");
			}
		}

		if (_scenario && !_scenario->_name.empty() && ImGui::MenuItem("Play Scenario"))
		{
			auto name = _scenario->_name;
			g_scene->Set(SceneID::Scenario);
			g_scene->Get<ScenarioScene>()->_scenario = make_unique<Scenario>(LoadJson(kDataFolder + string(kScenarioFolder) + name));
			g_scene->Get<ScenarioScene>()->Initialize();
			return;
		}

		if (_scenario && ImGui::TreeNode("Tools"))
		{
			for (auto tool : enum_values<ToolType>())
			{
				auto sv = enum_name(tool);
				string data{ sv.data(), sv.size() };
				//data = data.substr(0, 1);

				if (ImGui::Selectable(data.c_str(), _tool == tool))
				{
					_tool = tool;
				}
			}
			ImGui::TreePop();
		}

		if (_scenario && ImGui::TreeNode("Selection"))
		{
			static char sAlignmentSeparation[3] = { "15" };
			const auto separator = atoi(sAlignmentSeparation);

			for (auto task : enum_values<SelectionTask>())
			{
				auto sv = enum_name(task);
				string data{ sv.data(), sv.size() };
				//data = data.substr(0, 1);

				if (ImGui::Button(data.c_str()) && IsAnyObjectSelected())
				{
					auto objects = GetSelectedObjects();
					if(!objects.empty())
					{
						ranges::sort(objects, 
							[&](const auto * left, const auto * right)
							{
								return left->GetCollisionRect().top < right->GetCollisionRect().top;
							});

						switch(task)
						{
						case SelectionTask::SeparateHorizontally:
						{
							auto rect = objects[0]->GetCollisionRect();
							float start_pos = rect.left;
							for (szt i = 0; i < objects.size(); ++i)
							{
								auto rect = objects[i]->GetCollisionRect();
								objects[i]->SetPosition(start_pos + (rect.width * i) + (separator * i), rect.top);
							}

							_remake_stack._list.push(EditorAction{ EditorActionID::AlignHorizontally });
							break;
						}

						case SelectionTask::SeparateVertically:
						{
							auto rect = objects[0]->GetCollisionRect();
							float start_pos = rect.top;
							for(szt i = 0; i < objects.size(); ++i)
							{
								rect = objects[i]->GetCollisionRect();
								objects[i]->SetPosition(rect.left, start_pos + (rect.height * i) + (separator * i));
							}
							
							_remake_stack._list.push(EditorAction{ EditorActionID::AlignVertically });
							break;
						}

						case SelectionTask::AlignVertically:
						{
							auto rect = objects[0]->GetCollisionRect();
							float start_pos = rect.left;
							for (szt i = 0; i < objects.size(); ++i)
							{
								auto rect = objects[i]->GetCollisionRect();
								objects[i]->SetPosition(start_pos, rect.top);
							}

							_remake_stack._list.push(EditorAction{ EditorActionID::AlignVertically });
							break;
						}

						case SelectionTask::AlignHorizontally:
						{
							auto rect = objects[0]->GetCollisionRect();
							float start_pos = rect.top;
							for (szt i = 0; i < objects.size(); ++i)
							{
								auto rect = objects[i]->GetCollisionRect();
								objects[i]->SetPosition(rect.left, start_pos);
							}
							Log::Debug("Objects selected: {}", objects.size());
							_remake_stack._list.push(EditorAction{ EditorActionID::AlignHorizontally });
							break;
						}

					}
					}
				}
			}

			ImGui::InputText("Separation", sAlignmentSeparation, IM_ARRAYSIZE(sAlignmentSeparation));

			ImGui::TreePop();
		}

		_top_menu_rect = {
			(int)ImGui::GetWindowPos().x, (int)ImGui::GetWindowPos().y,
			(int)ImGui::GetWindowSize().x, (int)ImGui::GetWindowSize().y
		};
	}

	if (ImGui::BeginPopupModal("NewScenarioModal", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static char sNewScenarioChunksCount[3] = { "32" };

		ImGui::InputText("Chunks", sNewScenarioChunksCount, IM_ARRAYSIZE(sNewScenarioChunksCount));

		if (ImGui::Button("Create Scenario"))
		{
			// check if there are existent scenarios with this name on save
			_scenario = make_unique<Scenario>((ChunkIdx)atoi(sNewScenarioChunksCount));
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("OpenScenarioModal", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::ListBoxHeader("Scenarios");
		for (szt i = 0u; i < _scenario_names.size(); ++i)
		{
			string& item_name = _scenario_names[i];
			if (ImGui::Selectable(item_name.c_str(), false))
			{
				ImGui::CloseCurrentPopup();

				LoadScenario(item_name.c_str());
			}
		}
		ImGui::ListBoxFooter();

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("SaveScenarioModal", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("Scenario name", _new_scenario_name, IM_ARRAYSIZE(_new_scenario_name));

		if (ImGui::Button("Save Scenario"))
		{
			_scenario_names = Misc::FetchFileNamesFromDirectory(kDataFolder + "scenario"s);
			if(std::ranges::find(_scenario_names, _new_scenario_name) == _scenario_names.end())
			{
				SaveScenario(_new_scenario_name);
				ImGui::CloseCurrentPopup();
			}
			else
			{
				Log::Debug("The scenario {} already exists! Try another...", string(_new_scenario_name));
			}
		}

		ImGui::EndPopup();
	}
}

void ScenarioEditor::Tick(sf::Time delta_time)
{
	// So that the obstacles don't move
	//if(_scenario)
		//_scenario->Tick(delta_time);

	float velocity = 64.f;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		velocity *= 2.f;
	}

	if(_move_up)
	{
		_scenario->Move(0, velocity * delta_time.asSeconds());
	}
	else if(_move_down)
	{
		_scenario->Move(0, -velocity * delta_time.asSeconds());
	}
}

void ScenarioEditor::SaveScenario(string file_name)
{
	Json json;

	ofstream out(kDataFolder + "scenario\\"s + file_name);
	if(!out.is_open())
	{
		PrintError("Couldn't save map {}", file_name);
		return;
	}

	std::filesystem::path path(file_name);
	
	json["scenario_name"] = path.filename().string();

	// TODO: Replace by size?
	json["chunk_cnt"] = _scenario->_chunk_cnt;

	Log::Debug("Saving scenario with objects.size(): {}", _scenario->_objects.size());

	json["objects"] = Json::array();

	for (auto& object : _scenario->_objects)
	{
		if (object->_id != ObjectID::Obstacle)
			continue;

		auto& jobjects = json["objects"];

		auto jobject = Json::object();
		if (object->_initial_pos.x != -1.f && object->_initial_pos.y != -1.f)
		{
			//jobjects.emplace("type", enum_name(object->_id));

			jobject.emplace("x", object->_initial_pos.x);
			jobject.emplace("y", object->_initial_pos.y);

			Log::Debug("Saved scenario object at {}/{}", object->_initial_pos.x, object->_initial_pos.y);

			switch (object->_id)
			{
			case ObjectID::Obstacle:
			{
				auto* obstacle = dynamic_cast<Obstacle*>(object.get());
				assert(obstacle);

				jobject.emplace("type", enum_name(obstacle->_type));
				break;
			}
			}

			jobjects.push_back(jobject);
		}
	}

	out << json.dump(4);
}

void ScenarioEditor::LoadScenario(string file_name)
{
	_scenario = make_unique<Scenario>(LoadJson(kDataFolder + "scenario\\"s + file_name));
}

bool ScenarioEditor::IsAnyObjectSelected() const
{
	assert(_scenario);

	for (auto& object : _scenario->_objects)
	{
		if (!object->IsSelectable())
			continue;

		return object->_selected;
	}

	return true;
}

vector<Object*> ScenarioEditor::GetSelectedObjects()
{
	assert(_scenario);
	
	vector<Object*> objects;

	for (auto& object : _scenario->_objects)
	{
		if (!object->IsSelectable() || !object->_selected)
			continue;

		objects.push_back(object.get());
	}

	return objects;
}

void ScenarioEditor::PlayScenario()
{
	// Method without saving
	if(true){
		unique_ptr<Scenario> scenario_to_play = std::move(_scenario);

		//auto name = _scenario->_name;
		//SaveScenario(name);

		g_scene->Set(SceneID::Scenario);
		ScenarioScene * scene = g_scene->Get<ScenarioScene>();
	
		scene->_scenario = std::move(scenario_to_play);
	}

	// Method via saving
	/*{
		if(_scenario->_name.empty())
		{
			ImGui::OpenPopup("SaveScenarioModal");
			return;
		}

		SaveScenario(_scenario->_name);
		
		auto file_name = _scenario->_name;

		g_scene->Set(SceneID::Scenario);
		ScenarioScene * scene = g_scene->Get<ScenarioScene>();
		scene->_scenario = make_unique<Scenario>(LoadJson(kDataFolder + "scenario\\"s + file_name));
	}*/
}