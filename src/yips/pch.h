#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

#include <SFML/Graphics.hpp>
using sf::Vector2f;

// kazilib: std names, enums, startup flags, imgui (+ imgui-SFML for SFML 3), json, rang
#include <stdnames.h>
#include <magic_enum.h>
#include <under.h>
#include <initflags.h>
#include <exception_report.h>
using lib::Under;
using lib::EnumCStr;
using magic_enum::enum_name;
using magic_enum::enum_cast;
using magic_enum::enum_count;
using magic_enum::enum_values;
using magic_enum::enum_index;

#include <json.hpp>
using Json = nlohmann::json;

#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

#include <cstdint>
using i8 = std::int8_t;
using u8 = std::uint8_t;
using i16 = std::int16_t;
using u16 = std::uint16_t;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using uint = unsigned int;
using szt = std::size_t;

#include <fstream>
using std::ofstream;
using std::ifstream;

#include <format>
using std::format;

#include <filesystem>
using Path = std::filesystem::path;

#include <ranges>
// ranges:: is kazilib's (containers.h: namespace ranges { using namespace std::ranges; })
namespace views = std::ranges::views;

#include <stack>
#include <cmath>
#include <cassert>

using namespace std::string_literals;

#include "Log.h"
#include "Misc.h"
#include "Global.h"
