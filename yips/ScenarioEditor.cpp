#include "pch.h"
#include "ScenarioEditor.h"
#include "Obstacle1.h"
#include "Scenario.h"
#include "Background.h"
#include "Chunk.h"
#include "App.h"

ScenarioEditor::ScenarioEditor()
{
	ShiftObstacle(true);

}

void ScenarioEditor::ShiftObstacle(bool next)
{
	array<sf::Color, 2> kColors = { sf::Color::White, sf::Color::Blue };
	static u8 sIdx = 0;
	_current_obstacle = make_unique<Obstacle1>(50.f, 50.f);
	_current_obstacle->_color = kColors[sIdx++];
	if (sIdx >= kColors.size())
		sIdx = 0;
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
		/*if(event.mouseButton.button == sf::Mouse::Left)
		{
			if(!_scenario)
				return false;

			float x = sf::Mouse::getPosition(g_app->_wnd).x; 
			float y = sf::Mouse::getPosition(g_app->_wnd).y;

			auto chunk_idx = (kChunkSizeY + _scenario->_camera_y - y) / kChunkSizeY;
			
			auto obj = _scenario->_chunks[floor(chunk_idx)]->AddObject(make_unique<Obstacle1>());
			
			y = kChunkSizeY - ((chunk_idx * kChunkSizeY) - (kChunkSizeY * floor(chunk_idx)));
			
			PrintDebug("Clicked on chunk_idx({}) _scenario->_camera_y({}), event.mouseButton.y({}) chunk_pos_y({})", 
				floor(chunk_idx), _scenario->_camera_y, event.mouseButton.y, y);
			
			obj->SetInitialPosition(x, y);
		}*/
		break;
	}
	return false;
}

void ScenarioEditor::Render(sf::RenderWindow & wnd, sf::Time delta_time)
{
	if(_scenario)
		_scenario->Render(wnd, delta_time);


	auto pos = sf::Mouse::getPosition(wnd);
	_current_obstacle->SetPosition(pos.x, pos.y);
	//_current_obstacle->Render(wnd, delta_time);


	ImGui::Begin("Map editor");
	{
		static char buffer_chunks[64];
		ImGui::InputText("Map chunks", buffer_chunks, IM_ARRAYSIZE(buffer_chunks));
		
		if(ImGui::Button("Create Scenario"))
		{
			_scenario = make_unique<Scenario>((ChunkIdx)atoi(buffer_chunks));
			Print("Scenario created with {} chunks", _scenario->_chunks.size());
		}

		static char buffer_name[64];
		ImGui::InputText("Map name", buffer_name, IM_ARRAYSIZE(buffer_name));

		if(ImGui::Button("Load Scenario"))
		{
			LoadScenario(buffer_name);
		}

		ImGui::SameLine();
		if(ImGui::Button("Save Scenario"))
		{
			SaveScenario(buffer_name);
		}
	}
	ImGui::End();
}

void ScenarioEditor::Tick(sf::Time delta_time)
{
	if(_scenario)
		_scenario->Tick(delta_time);

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
	json["chunk_cnt"] = _scenario->_chunks.size();

	auto & jchunks = json["chunks"];

	size_t chunk_idx = 0;
	for(auto & chunk : _scenario->_chunks)
	{
		auto & jchunk = jchunks[chunk_idx];
		jchunk["idx"] = chunk_idx;
		
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
			jobject.emplace("x", object->_initial_pos.x);
			jobject.emplace("y", object->_initial_pos.y);
			jobject.emplace("type", enum_name(object->_id));
		}
	}

	out << json.dump(4);
}

void ScenarioEditor::LoadScenario(string file_name)
{
	_scenario = make_unique<Scenario>(LoadJson("scenario\\"s + file_name));
}