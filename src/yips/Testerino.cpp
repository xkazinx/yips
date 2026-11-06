#include "pch.h"
#include "Testerino.h"

#ifndef _WIN32
// A browser build has no bridge (its files, its process): the calls do nothing
namespace testerino
{
	void Start() {}
	void Pump() {}
	void AfterDraw(sf::RenderWindow &) {}
	void Stop() {}
	bool Active() { return false; }
}
#else
#include "App.h"
#include "Scenes.h"
#include "ScenarioScene.h"
#include "ScenarioEditor.h"
#include "Scenario.h"
#include "ObjectShip1.h"
#include "BallObstacle.h"
#include "BallProjectile.h"
#include "PowerUp.h"
#include "Projectile.h"
#include "Input.h"
#include <testerino/tcore.h>
#include <iostream>

namespace testerino
{
	namespace
	{
		using tcore::JArr;
		using tcore::JObj;

		tcore::Endpoint g_ep;
		tcore::Runner g_run(g_ep);
		bool g_active = false;
		SceneID g_scene_seen = SceneID::None;

		struct Shot
		{
			string _path;
			tcore::Job * _job = nullptr;
		};
		vector<Shot> g_shots;

		// ---- What is on screen ---------------------------------------------------

		Scenario * CurrentScenario()
		{
			if (auto s = g_scene->Get<ScenarioScene>())
				return s->_scenario.get();
			if (auto e = g_scene->Get<ScenarioEditor>())
				return e->_scenario.get();
			return nullptr;
		}

		JArr Numbers(const auto & list)
		{
			JArr a;

			for (const auto v : list)
				a.Int((long long)v);

			return a;
		}

		JObj State()
		{
			JObj o;
			o.Str("scene", string(enum_name(g_scene->Current()))).Bool("focus", g_app->_wnd.hasFocus());

			if (auto sc = CurrentScenario())
			{
				o.Str("scenario", sc->_name).Int("chunks", sc->_chunk_cnt).Num("view_y", sc->_view.getCenter().y);
				o.Int("obstacles", (long long)sc->CountObjects(ObjectID::Obstacle)).Int("power_ups", (long long)sc->CountObjects(ObjectID::PowerUp));
				o.Int("projectiles", (long long)sc->_projectiles.size()).Bool("debug_overlay", sc->_debug_overlay);
			}

			if (auto s = g_scene->Get<ScenarioScene>())
			{
				const Vector2f ship = s->_ship->GetPosition();
				const sf::Vector2i px = g_app->_wnd.mapCoordsToPixel(ship, s->_scenario->_view);
				o.Num("ship_x", ship.x).Num("ship_y", ship.y).Int("ship_px", px.x).Int("ship_py", px.y);
				o.Int("energies", s->_total_energies).Int("shots", s->_shots).Int("destroyed", s->_destroyed_obstacles).Int("power_ups_taken", s->_power_ups_taken);
				o.Raw("ammo", Numbers(s->_ability_ammo).Done()).Raw("levels", Numbers(s->_ability_levels).Done());
				JArr taken;

				for (const auto p : s->_ship->_power_ups)
					taken.Str(string(enum_name(p)));

				o.Raw("ship_power_ups", taken.Done());
			}

			if (auto e = g_scene->Get<ScenarioEditor>())
			{
				o.Str("tool", string(enum_name(e->_tool))).Int("selected", (long long)e->GetSelectedObjects().size());
				o.Str("status", e->_status).Bool("menu", e->_top_menu_opened);
				o.Int("menu_x", e->_top_menu_rect.position.x).Int("menu_y", e->_top_menu_rect.position.y).Int("menu_w", e->_top_menu_rect.size.x).Int("menu_h", e->_top_menu_rect.size.y);
			}

			return o;
		}

		// ---- Input, through App::ProcessEvent as real input goes -------------------

		bool KeyCode(const string & name, sf::Keyboard::Key & code)
		{
			const string n = tcore::Lower(name);
			if (n.size() == 1 && n[0] >= 'a' && n[0] <= 'z')
			{
				code = sf::Keyboard::Key(Under(sf::Keyboard::Key::A) + (n[0] - 'a'));
				return true;
			}

			static const std::map<string, sf::Keyboard::Key> kKeys = {
				{ "enter", sf::Keyboard::Key::Enter }, { "escape", sf::Keyboard::Key::Escape }, { "space", sf::Keyboard::Key::Space },
				{ "left", sf::Keyboard::Key::Left }, { "right", sf::Keyboard::Key::Right }, { "up", sf::Keyboard::Key::Up }, { "down", sf::Keyboard::Key::Down },
				{ "shift", sf::Keyboard::Key::LShift }, { "ctrl", sf::Keyboard::Key::LControl }, { "f1", sf::Keyboard::Key::F1 }, { "f3", sf::Keyboard::Key::F3 },
			};
			const auto it = kKeys.find(n);
			if (it == kKeys.end())
				return false;
			code = it->second;
			return true;
		}

		// "ctrl+p": the key with Control held, as the event and isKeyPressed both see it
		bool ParseKey(const string & text, sf::Keyboard::Key & code, bool & ctrl)
		{
			string name = tcore::Lower(text);
			ctrl = name.starts_with("ctrl+");
			if (ctrl)
				name = name.substr(5);
			return KeyCode(name, code);
		}

		void Key(const sf::Keyboard::Key code, const bool ctrl, const bool down)
		{
			if (ctrl)
				input::Hold(sf::Keyboard::Key::LControl, down);

			if (down)
			{
				sf::Event::KeyPressed k{};
				k.code = code;
				k.scancode = sf::Keyboard::delocalize(code);
				k.control = ctrl;
				input::Hold(code, true);
				g_app->ProcessEvent(k);
			}
			else
			{
				sf::Event::KeyReleased k{};
				k.code = code;
				k.scancode = sf::Keyboard::delocalize(code);
				k.control = ctrl;
				input::Hold(code, false);
				g_app->ProcessEvent(k);
			}
		}

		void Click(tcore::Job & j, const sf::Vector2i at, JObj answer)
		{
			j.Add([at] { g_app->ProcessEvent(sf::Event::MouseMoved{ at }); return true; });
			j.Add([at] { g_app->ProcessEvent(sf::Event::MouseButtonPressed{ sf::Mouse::Button::Left, at }); return true; });
			j.Add([at] { g_app->ProcessEvent(sf::Event::MouseButtonReleased{ sf::Mouse::Button::Left, at }); return true; });
			j.Add([&j, answer] { j.Ok(answer); return true; });
		}

		// A step that ends the job with the state once the scene asked for is running
		void AnswerInScene(tcore::Job & j, const SceneID id)
		{
			j.Add([&j, id] {
				if (g_scene->Current() != id)
					return false;
				j.Ok(State());
				return true;
			});
		}

		bool Compare(const double have, const char op, const double want)
		{
			switch (op)
			{
			case '>': return have > want;
			case '<': return have < want;
			default: return have == want;
			}
		}

		// ---- Verbs and conditions ----------------------------------------------------

		void AddEditorVerb()
		{
			g_run.Add("editor", "editor <new [chunks] | open <name> | save <name> | delete tt_<name> | tool <Position|Selection> | place <x> <y> | select <all|none> | task <SelectionTask> [separation] | play | menu <on|off>>",
				"the scenario editor's actions, the ones its menu runs (place: a click on that world point with the current tool)", [](const tcore::Cmd & c, tcore::Job & j) {
				auto e = g_scene->Get<ScenarioEditor>();
				if (!e)
					return j.Fail("not in the editor (scene ScenarioEditor)");

				const string what = tcore::Lower(c.Arg(0));
				if (what == "new")
				{
					int chunks = 32;
					c.ArgInt(1, chunks);
					e->NewScenario((ChunkIdx)std::clamp(chunks, 1, 999));
				}
				else if (what == "open")
				{
					if (!e->LoadScenario(c.Arg(1)))
						return j.Fail(e->_status);
				}
				else if (what == "save")
				{
					if (!e->_scenario)
						return j.Fail("no scenario");
					if (!e->CanSaveAs(c.Arg(1)))
						return j.Fail("cannot save as '" + c.Arg(1) + "' (taken or not a file name)");
					if (!e->SaveScenario(c.Arg(1)))
						return j.Fail(e->_status);
				}
				else if (what == "delete")
				{
					// Only a routine's own scenarios (tt_*): a run starts without the last run's
					const string name = c.Arg(1);
					if (!name.starts_with("tt_") || name.find_first_of("/\\:") != string::npos)
						return j.Fail("editor delete tt_<name> (a routine's scenario)");
					std::error_code ec;
					std::filesystem::remove(ScenarioFolder() / name, ec);
				}
				else if (what == "tool")
				{
					const auto tool = enum_cast<ScenarioEditor::ToolType>(c.Arg(1), magic_enum::case_insensitive);
					if (!tool)
						return j.Fail("unknown tool " + c.Arg(1));
					e->_tool = *tool;
				}
				else if (what == "place")
				{
					int x = 0, y = 0;
					if (!e->_scenario || !c.ArgInt(1, x) || !c.ArgInt(2, y))
						return j.Fail("editor place <x> <y> (a scenario open)");
					e->ClickAt(e->ToPixel({ (float)x, (float)y }));
				}
				else if (what == "select")
				{
					e->SelectAll(tcore::Lower(c.Arg(1)) != "none");
				}
				else if (what == "task")
				{
					const auto task = enum_cast<ScenarioEditor::SelectionTask>(c.Arg(1), magic_enum::case_insensitive);
					if (!task)
						return j.Fail("unknown task " + c.Arg(1));
					int sep = 15;
					c.ArgInt(2, sep);
					e->RunSelectionTask(*task, (float)sep);
				}
				else if (what == "play")
				{
					if (!e->_scenario)
						return j.Fail("no scenario");
					e->PlayScenario();
					return AnswerInScene(j, SceneID::Scenario);
				}
				else if (what == "menu")
				{
					e->_top_menu_opened = tcore::Lower(c.Arg(1)) != "off";
				}
				else
				{
					return j.Fail("unknown editor action " + c.Arg(0));
				}

				j.Frame();
				j.Add([&j] { j.Ok(State()); return true; });
			});
		}

		void AddVerbs()
		{
			g_run.Add("state", "state", "the scene, the scenario (name, chunks, obstacles, power ups, projectiles), the ship (spot, energies, ammo, levels, counters) and the editor (tool, selected, status, menu rect)", [](const tcore::Cmd &, tcore::Job & j) {
				j.Ok(State());
			});

			g_run.Add("scene", "scene <Start|Scenario|ScenarioEditor>", "changes the scene (as Enter / Ctrl+E do) and answers in it", [](const tcore::Cmd & c, tcore::Job & j) {
				const auto id = enum_cast<SceneID>(c.Arg(0), magic_enum::case_insensitive);
				if (!id || *id == SceneID::None)
					return j.Fail("unknown scene " + c.Arg(0));
				g_scene->Set(*id);
				AnswerInScene(j, *id);
			});

			g_run.Add("key", "key <a..z|enter|escape|space|left|right|up|down|shift|f1|f3>[ctrl+] [times]", "a key pressed and let go (ctrl+p: with Control held)", [](const tcore::Cmd & c, tcore::Job & j) {
				sf::Keyboard::Key code{};
				bool ctrl = false;
				if (!ParseKey(c.Arg(0), code, ctrl))
					return j.Fail("unknown key " + c.Arg(0));
				int times = 1;
				c.ArgInt(1, times);

				for (int i = 0; i < std::max(1, times); i++)
				{
					j.Add([code, ctrl] { Key(code, ctrl, true); return true; });
					j.Add([code, ctrl] { Key(code, ctrl, false); return true; });
				}

				j.Add([&j] { j.Ok(State()); return true; });
			});

			g_run.Add("hold", "hold <key> <ms>", "a key held down that long (the ship's arrows, Shift), then let go", [](const tcore::Cmd & c, tcore::Job & j) {
				sf::Keyboard::Key code{};
				bool ctrl = false;
				int ms = 0;
				if (!ParseKey(c.Arg(0), code, ctrl) || !c.ArgInt(1, ms))
					return j.Fail("hold <key> <ms>");

				auto until = make_shared<DWORD>(0);
				j.Add([code, ctrl, ms, until] { Key(code, ctrl, true); *until = timeGetTime() + (DWORD)ms; return true; });
				j.Add([until] { return timeGetTime() >= *until; });
				j.Add([code, ctrl] { Key(code, ctrl, false); return true; });
				j.Add([&j] { j.Ok(State()); return true; });
				j.OnAbort([code, ctrl] { input::Hold(code, false); if (ctrl) input::Hold(sf::Keyboard::Key::LControl, false); });
			});

			auto down_up = [](const char * verb, const bool down) {
				g_run.Add(verb, string(verb) + " <key>", down ? "a key pressed and kept down (up lets it go): two keys at once, Shift with an arrow" : "lets go a key down kept", [down](const tcore::Cmd & c, tcore::Job & j) {
					sf::Keyboard::Key code{};
					bool ctrl = false;
					if (!ParseKey(c.Arg(0), code, ctrl))
						return j.Fail("unknown key " + c.Arg(0));
					Key(code, ctrl, down);
					j.Frame();
					j.Add([&j] { j.Ok(State()); return true; });
				});
			};
			down_up("down", true);
			down_up("up", false);

			g_run.Add("click", "click <x> <y>", "a left click at that window spot (move, press, release on three frames)", [](const tcore::Cmd & c, tcore::Job & j) {
				int x = 0, y = 0;
				if (!c.ArgInt(0, x) || !c.ArgInt(1, y))
					return j.Fail("click <x> <y>");
				Click(j, { x, y }, JObj().Int("x", x).Int("y", y));
			});

			g_run.Add("obstacle", "obstacle <ahead_px> [wave|linear|still]", "a ball obstacle that far ahead of the ship's nose, in line with it (still by default)", [](const tcore::Cmd & c, tcore::Job & j) {
				auto s = g_scene->Get<ScenarioScene>();
				int ahead = 0;
				if (!s || !c.ArgInt(0, ahead))
					return j.Fail("obstacle <ahead_px> (in the Scenario scene)");
				const string mode = tcore::Lower(c.Arg(1));
				const Vector2f nose = s->_ship->Muzzle(BallProjectile::kRadius);
				auto obs = s->_scenario->AddObject(make_unique<BallObstacle>(Vector2f{ nose.x - 20.f, nose.y + (float)ahead }));
				obs->_move_type = mode == "wave" ? MoveType::Wave : mode == "linear" ? MoveType::Linear : MoveType::None;
				j.Ok(State());
			});

			g_run.Add("powerup", "powerup <PowerUpID> [ahead_px]", "a power up that far ahead of the ship (0: on it, taken next frame)", [](const tcore::Cmd & c, tcore::Job & j) {
				auto s = g_scene->Get<ScenarioScene>();
				const auto id = enum_cast<PowerUpID>(c.Arg(0), magic_enum::case_insensitive);
				if (!s || !id || *id == PowerUpID::None)
					return j.Fail("powerup <Energy|Velocity|Invincible|Ammo_Ability1..8> (in the Scenario scene)");
				int ahead = 0;
				c.ArgInt(1, ahead);
				const sf::FloatRect ship = s->_ship->GetCollisionRect();
				s->_scenario->Spawn(make_unique<PowerUp>(*id, Vector2f{ ship.position.x, ship.position.y + (float)ahead }));
				j.Frame();
				j.Add([&j] { j.Ok(State()); return true; });
			});

			AddEditorVerb();

			g_run.Add("shot", "shot <name>", "a screenshot of the next frame (shots\\<name>.png)", [](const tcore::Cmd & c, tcore::Job & j) {
				if (c.Arg(0).empty())
					return j.Fail("shot <name>");
				g_shots += Shot{ g_ep.ShotPath(c.Arg(0)).string(), &j };
				j.Add([&j] { return j.answered; }); // AfterDraw answers it
				j.OnAbort([&j] { std::erase_if(g_shots, [&j](const Shot & s) { return s._job == &j; }); });
			});

			g_run.Add("close", "close", "closes the window, as its X button does (the program ends)", [](const tcore::Cmd &, tcore::Job & j) {
				j.Ok();
				g_app->ProcessEvent(sf::Event::Closed{});
			});

			g_run.Add("quit", "quit", "ends the program", [](const tcore::Cmd &, tcore::Job & j) {
				j.Ok();
				g_app->_exit = true;
			});

			g_run.AddCond("scene", "scene=<Start|Scenario|ScenarioEditor>: running", [](const string & v, char, tcore::Cond & out, string &) {
				out = [v](string & d) {
					d = string(enum_name(g_scene->Current()));
					return tcore::Lower(d) == tcore::Lower(v);
				};
				return true;
			});

			// Numbers: name=<n>, name><n>, name<<n>
			auto number_cond = [](const char * key, function<double()> get) {
				g_run.AddCond(key, string(key) + "=<n> / " + key + "><n> / " + key + "<<n>", [get](const string & v, const char op, tcore::Cond & out, string & err) {
					char * end = nullptr;
					const double want = strtod(v.c_str(), &end);
					if (end == v.c_str())
					{
						err = "not a number: " + v;
						return false;
					}
					out = [want, op, get](string & d) {
						const double have = get();
						d = std::format("{}", have);
						return Compare(have, op, want);
					};
					return true;
				});
			};
			number_cond("obstacles", [] { auto sc = CurrentScenario(); return sc ? (double)sc->CountObjects(ObjectID::Obstacle) : -1.0; });
			number_cond("power_ups", [] { auto sc = CurrentScenario(); return sc ? (double)sc->CountObjects(ObjectID::PowerUp) : -1.0; });
			number_cond("projectiles", [] { auto sc = CurrentScenario(); return sc ? (double)sc->_projectiles.size() : -1.0; });
			number_cond("destroyed", [] { auto s = g_scene->Get<ScenarioScene>(); return s ? (double)s->_destroyed_obstacles : -1.0; });
			number_cond("power_ups_taken", [] { auto s = g_scene->Get<ScenarioScene>(); return s ? (double)s->_power_ups_taken : -1.0; });

			g_run.ping_fields = [](JObj & o) { o.Str("scene", string(enum_name(g_scene->Current()))); };
		}
	}

	void Start()
	{
		const int slot = (int)lib::init::Number("testerino", 0);
		if (slot <= 0)
			return;

		std::error_code ec;
		if (!tcore::fs::exists(tcore::CheckoutFromExe() / "src" / "yips.sln", ec))
		{
			std::cout << "(!) testerino is only on in a source checkout (src\\yips.sln)\n";
			return;
		}

		AddVerbs();
		if (!g_ep.Open(tcore::CheckoutFromExe() / "testerino", "client_" + to_string(slot), JObj().Int("slot", slot)))
		{
			std::cout << "(!) testerino: cannot open its folder\n";
			return;
		}

		g_active = true;
		std::cout << "Testerino: listening on " << g_ep._dir.string() << "\n";
	}

	void Pump()
	{
		if (!g_active)
			return;

		g_run.Pump();

		// An event each time the scene changes, for `events` and routines
		if (g_scene->Current() != g_scene_seen)
		{
			g_scene_seen = g_scene->Current();
			g_ep.Event("scene", JObj().Str("scene", string(enum_name(g_scene_seen))));
		}
	}

	void AfterDraw(sf::RenderWindow & wnd)
	{
		if (g_shots.empty())
			return;

		static sf::Texture texture;
		const auto sz = wnd.getSize();
		if (texture.getSize() != sz && !texture.resize(sz))
			return;
		texture.update(wnd);
		const sf::Image image = texture.copyToImage();

		tcore::Rgba img;
		img.w = (int)image.getSize().x;
		img.h = (int)image.getSize().y;
		img.px.assign(image.getPixelsPtr(), image.getPixelsPtr() + (size_t)img.w * img.h * 4);

		for (auto & s : g_shots)
		{
			if (tcore::WritePng(s._path, img))
				s._job->Ok(JObj().Str("path", s._path).Int("w", img.w).Int("h", img.h));
			else
				s._job->Fail("could not write " + s._path);
		}

		g_shots.clear();
	}

	void Stop()
	{
		if (g_active)
			g_ep.Close();
		g_active = false;
	}

	bool Active()
	{
		return g_active;
	}
}
#endif // _WIN32
