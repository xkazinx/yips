#include "pch.h"
#include "App.h"
#include "Scenes.h"
#include "Testerino.h"
#include "Input.h"

#include <chrono>

void App::Tick()
{
	g_app = this;

	if (!_font.openFromFile(DataFolder() / "CascadiaCode.ttf"))
		Log::Error("Couldn't load {}", (DataFolder() / "CascadiaCode.ttf").string());
	_text.emplace(_font);

	// title=<text>: told apart from other windows; background: opens behind the window
	// that has focus and leaves it the focus (tests running while the PC is used)
	const string extra = lib::init::Value("title");
	const string title = "Yips" + (extra.empty() ? string() : " - " + extra);
#ifdef _WIN32
	const bool background = lib::init::Flag("background", false);
	const HWND before = background ? GetForegroundWindow() : nullptr;
#endif

	_wnd.create(sf::VideoMode({ g_res_x, g_res_y }), title, sf::Style::Titlebar | sf::Style::Close);
	_wnd.setVerticalSyncEnabled(true);

#ifdef _WIN32
	if (background)
	{
		SetWindowPos((HWND)_wnd.getNativeHandle(), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		if (before)
			SetForegroundWindow(before);
	}
#endif

	// imgui.ini beside the exe, not in whatever folder it was started from
	static const string kImguiIni = (DataFolder().parent_path() / "imgui.ini").string();
	ImGui::SFML::Init(_wnd);
	ImGui::GetIO().IniFilename = kImguiIni.c_str();

	global::Initialize();
	testerino::Start();

	sf::Clock delta_clock;

	while (!_exit && _wnd.isOpen())
	{
		g_scene->ApplyPending();
		HandleEvents();
		if (_exit || !_wnd.isOpen())
			break;

		testerino::Pump();

		const auto delta_time = delta_clock.restart();

		ImGui::SFML::Update(_wnd, delta_time);

		g_scene->Tick(delta_time);

		Render(delta_time);

		_clock.restart();

#ifndef __EMSCRIPTEN__
		// A browser: one frame per pass, display() already waited for the animation frame
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
#endif
	}

	testerino::Stop();
	global::Deinitialize();
	ImGui::SFML::Shutdown();
	_text.reset();
}

void App::HandleEvents()
{
	while (const optional<sf::Event> event = _wnd.pollEvent())
	{
		ProcessEvent(*event);
		if (_exit)
			return;
	}
}

void App::ProcessEvent(const sf::Event & event)
{
	if (event.is<sf::Event::Closed>())
	{
		_exit = true;
		return;
	}

	if (const auto key = event.getIf<sf::Event::KeyPressed>(); key && key->code == sf::Keyboard::Key::Escape)
	{
#ifdef __EMSCRIPTEN__
		// A browser page has no exit: Esc goes back to the Start scene (not while
		// typing into one of the editor's boxes)
		if (g_scene->Current() != SceneID::Start && !ImGui::GetIO().WantTextInput)
			g_scene->Set(SceneID::Start);
#else
		_exit = true;
#endif
		return;
	}

	// Focus lost: the keys a hand held are up as far as the game knows
	if (event.is<sf::Event::FocusLost>())
		input::ReleaseAll();

	ImGui::SFML::ProcessEvent(event);
	g_scene->HandleEvents(event);
}

void App::Render(const sf::Time delta_time)
{
	_wnd.clear();

	g_scene->Render(_wnd, delta_time);

	ImGui::SFML::Render(_wnd);

	testerino::AfterDraw(_wnd);
	_wnd.display();
}

void App::RenderText(const string & text, const Vector2f pos, const uint size, const sf::Color color, const function<void(sf::Text &)> & pre_draw_fnc)
{
	if (!_text)
		return;

	sf::Text & t = *_text;
	t.setPosition(pos);
	t.setCharacterSize(size);
	t.setFillColor(color);
	t.setString(sf::String::fromUtf8(text.begin(), text.end()));
	t.setOrigin({ 0.f, 0.f });
	t.setRotation(sf::degrees(0.f));

	if (pre_draw_fnc)
		pre_draw_fnc(t);

	_wnd.draw(t);
}
