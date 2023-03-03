#pragma once

#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#else
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-system.lib")
#endif

#pragma comment(lib, "opengl32.lib")

#include <SFML/Graphics.hpp>
using sf::Vector2f;

#include <memory>
using std::unique_ptr;
using std::make_unique;

#include <type_traits>

using i8 = signed short;
using u8 = unsigned short;
using i16 = signed short;
using u16 = unsigned short;
using i32 = signed long;
using u32 = unsigned long;
using uint = unsigned int;
using szt = std::size_t;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <array>
using std::array;

#include <functional>

#include <fstream>
using std::ofstream;
using std::ifstream;

#include <Windows.h>

#include <format>
using std::format;

#include <filesystem>
using Path = std::filesystem::path;

using std::forward;
#include "Log.h"

using std::underlying_type_t;
#include "magic_enum.h"
using namespace magic_enum;

#include <nlohmann/json.hpp>
using Json = nlohmann::json;

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

#include <string_view>
using namespace std::string_literals;

#include <ranges>
namespace ranges = std::ranges;
namespace views = std::ranges::views;

#include <stack>

#include "Misc.h"

#include "Global.h"