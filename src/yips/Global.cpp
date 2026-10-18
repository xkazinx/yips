#include "pch.h"
#include "Global.h"
#include "Scenes.h"
#include "App.h"
#include "Random.h"

namespace global
{
	unique_ptr<Scenes> g_scene;
	unique_ptr<Random> g_rand;

	App * g_app = nullptr;

	u16 g_res_x = kViewSizeX, g_res_y = kViewSizeY;

	void Initialize()
	{
		g_rand = make_unique<Random>();
		g_scene = make_unique<Scenes>();
	}

	void Deinitialize()
	{
		// The scenes first: their objects hold textures the window's context draws
		g_scene.reset();
		g_rand.reset();
	}

	const Path & DataFolder()
	{
		static const Path kPath = [] {
			char exe[MAX_PATH] = {};
			GetModuleFileNameA(nullptr, exe, MAX_PATH);
			return Path(exe).parent_path() / "data";
		}();
		return kPath;
	}
}
