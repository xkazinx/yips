#include "pch.h"
#include "Scenario.h"
#include "Background.h"
#include "App.h"
#include "BallObstacle.h"
#include "Projectile.h"

Scenario::Scenario(const ChunkIdx chunk_cnt)
{
	Initialize(chunk_cnt);
}

Scenario::Scenario(const Json & json)
{
	Load(json);
}

Scenario::~Scenario() = default;

void Scenario::Initialize(const ChunkIdx chunk_cnt)
{
	_chunk_cnt = std::max<ChunkIdx>(chunk_cnt, 1);
	_view = sf::View(sf::FloatRect({ 0.f, 0.f }, { (float)kViewSizeX, (float)kViewSizeY }));
	// Turned from the start: the editor maps clicks through it before the first frame
	_view.setRotation(sf::degrees(180.f));
	g_app->_wnd.setView(_view);

	AddObject(make_unique<StarsBackground>(0.f, float(kViewSizeY * _chunk_cnt)));
}

void Scenario::Spawn(unique_ptr<Object> && object)
{
	_spawned += std::move(object);
}

void Scenario::Tick(const sf::Time delta_time)
{
	for (auto & projectile : _projectiles)
		projectile->Tick(delta_time);

	Object::Tick(delta_time);

	// Projectiles against the objects
	for (auto & projectile : _projectiles)
	{
		for (auto & object : _objects)
		{
			if (projectile->HasExpired())
				break;

			if (object->HasExpired() || !projectile->IsCollision(object.get()))
				continue;

			projectile->HandleCollision(object.get());
			object->HandleCollision(projectile.get());
		}
	}

	// The objects against each other, each pair once
	for (szt i = 0; i < _objects.size(); ++i)
	{
		for (szt j = i + 1; j < _objects.size(); ++j)
		{
			Object * const a = _objects[i].get();
			Object * const b = _objects[j].get();
			if (a->HasExpired() || b->HasExpired() || !a->IsCollision(b))
				continue;

			a->HandleCollision(b);
			b->HandleCollision(a);
		}
	}

	const float view_y = _view.getCenter().y;

	for (auto & projectile : _projectiles)
	{
		if (std::abs(projectile->GetPosition().y - view_y) > kProjectileReach)
			projectile->Expire();
	}

	std::erase_if(_projectiles, [](const auto & p) { return p->HasExpired(); });
	std::erase_if(_objects, [](const auto & o) { return o->HasExpired(); });

	for (auto & object : _spawned)
		_objects += std::move(object);

	_spawned.clear();
}

void Scenario::Render(sf::RenderTarget & wnd, const sf::Time delta_time)
{
	auto & window = g_app->_wnd;

	if (_debug_overlay)
	{
		window.setView(window.getDefaultView());

		const auto mp = sf::Mouse::getPosition(window);
		const auto mc = window.mapPixelToCoords(mp);
		constexpr auto kTextSize = 15u;
		Vector2f text_pos = { mc.x, mc.y + 20 };

		const auto view_center = _view.getCenter();
		const auto view_viewport = _view.getViewport();
		const string lines[] = {
			format("view_center: {}/{}", view_center.x, view_center.y),
			format("_view_viewport: l:{} t:{} r:{} b:{}", view_viewport.position.x, view_viewport.position.y, view_viewport.size.x, view_viewport.size.y),
			format("view_rotation: {}", _view.getRotation().asDegrees()),
			format("mouse_coord: {}/{}", mc.x, mc.y),
			format("mouse_pos: {}/{}", mp.x, mp.y),
		};

		for (const auto & line : lines)
		{
			g_app->RenderText(line, text_pos, kTextSize);
			text_pos.y += kTextSize;
		}
	}

	_view.setRotation(sf::degrees(180.f));
	window.setView(_view);

	Object::Render(wnd, delta_time);

	for (auto & projectile : _projectiles)
		projectile->Render(wnd, delta_time);
}

void Scenario::Move(const float x, const float y)
{
	_view.move({ x, y });
}

szt Scenario::CountObjects(const ObjectID id) const
{
	return (szt)ranges::count_if(_objects, [id](const auto & o) { return o->_id == id && !o->HasExpired(); });
}

void Scenario::Load(const Json & json)
{
	if (!json.is_object())
	{
		Log::Error("Scenario: not a scenario file, an empty one in its place");
		Initialize(kCount);
		return;
	}

	_name = json.value("scenario_name", string());
	Initialize(json.value("chunk_cnt", (ChunkIdx)kCount));

	const auto objects = json.find("objects");
	if (objects == json.end() || !objects->is_array())
		return;

	for (const auto & jobject : *objects)
	{
		// TODO: Read more than Obstacles in the future
		const auto type = enum_cast<ObstacleType>(jobject.value("type", string()));
		if (!type || *type == ObstacleType::None)
		{
			Log::Error("Scenario {}: unknown object type {}", _name, jobject.value("type", string()));
			continue;
		}

		const Vector2f pos{ jobject.value("x", 0.f), jobject.value("y", 0.f) };

		switch (*type)
		{
		case ObstacleType::Ball:
			AddObject(make_unique<BallObstacle>(pos))->_initial_pos = pos;
			break;
		default:
			break;
		}
	}
}

Json Scenario::ToJson() const
{
	Json json;
	json["scenario_name"] = _name;
	// TODO: Replace by size?
	json["chunk_cnt"] = _chunk_cnt;
	json["objects"] = Json::array();

	for (const auto & object : _objects)
	{
		if (object->_id != ObjectID::Obstacle)
			continue;

		if (object->_initial_pos.x == -1.f && object->_initial_pos.y == -1.f)
			continue;

		const auto * obstacle = static_cast<const Obstacle *>(object.get());
		json["objects"].push_back({
			{ "type", enum_name(obstacle->_type) },
			{ "x", object->_initial_pos.x },
			{ "y", object->_initial_pos.y },
		});
	}

	return json;
}
