#include "pch.h"
#include "ScenarioEditor.h"
#include "BallObstacle.h"
#include "Scenario.h"
#include "Background.h"
#include "App.h"
#include "BallProjectile.h"

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
		}
		break;

	case sf::Event::EventType::MouseButtonPressed:
		if(event.mouseButton.button == sf::Mouse::Left)
		{
			if(!_scenario)
				return false;

			auto mp = sf::Mouse::getPosition(g_app->_wnd);

			// auto chunk_idx = CalcChunkIdx(y);
			auto chunk_idx = (kChunkSizeY + _scenario->_camera_y - mp.y) / kChunkSizeY;
			
			//y = kChunkSizeY - ((chunk_idx * kChunkSizeY) - (kChunkSizeY * floor(chunk_idx)));

			PrintDebug("Clicked on chunk_idx({}) _scenario->_camera_y({}), event.mouseButton.y({}) chunk_pos_y({}) x({}) y({})", 
				floor(chunk_idx), _scenario->_camera_y, event.mouseButton.y, mp.y, mp.x, mp.y);
			
			auto mp2 = g_app->_wnd.mapPixelToCoords(sf::Mouse::getPosition(g_app->_wnd));
			auto obj = _scenario->AddObject(make_unique<BallObstacle>(Vector2f{ (float)mp2.x, (float)mp2.y }));

			mp2.y += kChunkSizeY;
			// Deduce final coord to place at
			obj->_initial_pos = { (float)mp2.x, (float)mp2.y };
		}
		break;
	}
	return false;
}

void ScenarioEditor::Render(sf::RenderWindow & wnd, sf::Time delta_time)
{
	if(_scenario)
		_scenario->Render(wnd, delta_time);
	
	auto mpos = wnd.mapPixelToCoords(sf::Mouse::getPosition(g_app->_wnd));
	_current_obstacle->SetPosition(mpos.x, mpos.y);
	_current_obstacle->Render(wnd, delta_time);

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
		_scenario->Move(0, -velocity * delta_time.asSeconds());
	}
	else if(_move_down)
	{
		_scenario->Move(0, velocity * delta_time.asSeconds());
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

	/*
	json["chunk_cnt"] = _scenario->_chunk_cnt;

	auto & jchunks = json["chunks"];
	
	size_t chunk_idx = 0;
	for(auto & chunk : _scenario->_chunks)
	{
		auto & jchunk = jchunks[chunk->_idx];
		jchunk["idx"] = chunk->_idx;
		
		PrintDebug("ChunkIdx({}) ObjectsCount({})", chunk_idx++, chunk->_objects.size());

		size_t object_idx = 0;
		for(auto & object : chunk->_objects)
		{
			PrintDebug("ObjectID({})", enum_name(object->_id));

			if(object->_id == ObjectID::Chunk || 
				object->_id == ObjectID::None)
			{
				continue;
			}

			auto & jobject = jchunk["objects"][object_idx++];
			if(object->_initial_pos.x != -1.f && object->_initial_pos.y != -1.f)
			{
				jobject.emplace("x", object->_initial_pos.x);
				jobject.emplace("y", object->_initial_pos.y);
			}
			jobject.emplace("type", enum_name(object->_id));
			switch(object->_id)
			{
			case ObjectID::Obstacle:
			{
				auto * obstacle = dynamic_cast<Obstacle*>(object.get());
				assert(obstacle);

				jobject.emplace("obstacle_type", enum_name(obstacle->_type));
				break;
			}
			}
		}
	}
	*/

	out << json.dump(4);
}

void ScenarioEditor::LoadScenario(string file_name)
{
	_scenario = make_unique<Scenario>(LoadJson("scenario\\"s + file_name));
}