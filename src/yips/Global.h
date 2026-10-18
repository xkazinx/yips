#pragma once

class App;
class Scenes;
class Random;

namespace global
{
	extern unique_ptr<Scenes> g_scene;
	extern unique_ptr<Random> g_rand;
	extern App * g_app;

	constexpr u16 kViewSizeX = 1024;
	constexpr u16 kViewSizeY = 768;

	extern u16 g_res_x, g_res_y;

	void Initialize();
	void Deinitialize();

	// Next to the exe (deploy\data\): the working directory is not trusted, so a
	// shortcut or a test started from anywhere finds the files
	const Path & DataFolder();
	inline Path ScenarioFolder() { return DataFolder() / "scenario"; }

	enum class SceneID : u8
	{
		Start,
		Scenario,
		ScenarioEditor,

		None = u8(-1)
	};

	enum class ObjectID : u8
	{
		Ship,
		Sprite,
		Chunk,
		Object, // Scenario Object
		Obstacle,
		Projectile,
		PowerUp,

		None = u8(-1)
	};

	enum class ProjectileID : u8
	{
		Ball,
	};

	enum class ObstacleType : u8
	{
		Ball,

		None = u8(-1)
	};

	enum class PowerUpID : u8
	{
		Energy = 0,
		Velocity,
		Invincible,
		Ammo_Ability1,
		Ammo_Ability2,
		Ammo_Ability3,
		Ammo_Ability4,
		Ammo_Ability5,
		Ammo_Ability6,
		Ammo_Ability7,
		Ammo_Ability8,

		None = u8(-1)
	};

	enum class AbilityID : u8
	{
		Ability1,
		Ability2,
		Ability3,
		Ability4,
		Ability5,
		Ability6,
		Ability7,
		Ability8,

		None = u8(-1)
	};

	// The abilities' count, without None
	constexpr szt kAbilityCount = Under(AbilityID::Ability8) + 1;

	enum class ItemID : u8
	{
		Item1,
		Item2,
		Item3,
		Item4,
		Item5,
		Item6,
		Item7,
		Item8,

		None = u8(-1)
	};

	enum class InventorySlotID : u8
	{
		Item1,
		Item2,
		Item3,
		Item4,
		Item5,
		Item6,
		Item7,
		Item8,

		None = u8(-1)
	};

	using ChunkIdx = u16;

	inline ChunkIdx CalcChunkIdx(const float y)
	{
		return static_cast<ChunkIdx>(std::floor(std::abs(y / kViewSizeY)));
	}

	// A JSON file; empty (null) when it is missing or does not parse
	inline Json LoadJson(const Path & path)
	{
		std::ifstream in(path);
		if (!in.is_open())
		{
			Log::Error("Couldn't load file {}", path.string());
			return {};
		}

		Json json = Json::parse(in, nullptr, false);
		if (json.is_discarded())
		{
			Log::Error("{} is not valid JSON", path.string());
			return {};
		}

		return json;
	}
}

namespace scenario
{
	static constexpr auto kNameSize = 64u;
}

using namespace global;
