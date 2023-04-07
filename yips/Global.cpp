#include "pch.h"
#include "Global.h"
#include "Scenes.h"
#include "StartScene.h"
#include "App.h"
#include "Random.h"

namespace Global
{
	unique_ptr<Scenes> g_scene;
	unique_ptr<Random> g_rand;

	App * g_app;

	u16 g_res_x = kViewSizeX, g_res_y = kViewSizeY;

	void Initialize()
	{
		g_scene = make_unique<Scenes>();
		g_rand = make_unique<Random>();
	}

	void Deinitialize()
	{
		// ordered deinitialization
	}
}