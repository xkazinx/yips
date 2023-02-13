#pragma once

class App;
class Scenes;
class Random;

namespace Global
{
	extern unique_ptr<Scenes> g_scene;
	
	constexpr u16 kChunkSizeX = 1024;
	constexpr u16 kChunkSizeY = 768;

	extern u16 g_res_x, g_res_y;

	extern void Initialize();
	extern void Deinitialize();

	extern App * g_app;
	extern unique_ptr<Random> g_rand;

	constexpr char kDataFolder[] = "..\\data\\";

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

		None = u8(-1)
	};

	enum class ProjectileID
	{
		Ball,
	};

	enum class ObstacleType
	{
		Ball,
	};

	using ChunkIdx = u16;

	inline ChunkIdx CalcChunkIdx(float y)
	{
		return static_cast<ChunkIdx>(floor(abs(y / kChunkSizeY)));
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

using namespace Global;