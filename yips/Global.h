#pragma once

class App;
class Scenes;
class Random;

namespace Global
{
	extern unique_ptr<Scenes> g_scene;
	
	constexpr u16 kViewSizeX = 1024;
	constexpr u16 kViewSizeY = 768;

	extern u16 g_res_x, g_res_y;

	extern void Initialize();
	extern void Deinitialize();

	extern App * g_app;
	extern unique_ptr<Random> g_rand;

	constexpr char kDataFolder[] = ".\\data\\";
	constexpr char kScenarioFolder[] = "scenario\\";

	enum class SceneID 
	{
		Start,
		Scenario,
		ScenarioEditor,

		None = u8(-1)
	};

	enum class ObjectID
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

	enum class ProjectileID
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

	inline ChunkIdx CalcChunkIdx(float y)
	{
		return static_cast<ChunkIdx>(floor(abs(y / kViewSizeY)));
	}

	inline Json LoadJson(string file_name)
	{
		Json json;

		std::ifstream in(kDataFolder + file_name);
		if(!in.is_open())
		{
			//PrintError("Couldn't load file {}", file_name);
			return {};
		}

		in >> json;
		return json;
	}
}

namespace scenario
{
	static constexpr auto kNameSize = 64u;
}

namespace projectile
{

}

using namespace Global;