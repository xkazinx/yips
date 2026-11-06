# yips for a web browser (WebAssembly, Emscripten). Windows builds yips from
# yips\yips.vcxproj (build.ps1); this file is the web one, run by build_web.ps1 against
# kazilib's SFML web build (kazilib web/build_deps.ps1):
#   emcmake cmake -S src -B <build> -G Ninja -DSFML_DIR=... -DSFML_STATIC_LIBRARIES=ON
# Output: index.html / .js / .wasm / .data (deploy\data's font, pictures and scenarios
# under /data) - kazin.dev/projects/yips.
cmake_minimum_required(VERSION 3.21)
project(yips_web CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(YIPS_SRC ${CMAKE_CURRENT_SOURCE_DIR})
set(YIPS_DATA ${YIPS_SRC}/../deploy/data)
set(SFML_STATIC_LIBRARIES ON)
find_package(SFML 3 REQUIRED COMPONENTS Graphics)
include(${YIPS_SRC}/dep/kazilib/web/kazilib_web.cmake)

file(GLOB YIPS_SOURCES CONFIGURE_DEPENDS ${YIPS_SRC}/yips/*.cpp)
list(FILTER YIPS_SOURCES EXCLUDE REGEX "pch\\.cpp$")

add_executable(yips ${YIPS_SOURCES})
kazilib_web_target(yips)
kazilib_web_imgui(yips)
target_include_directories(yips PRIVATE ${YIPS_SRC}/yips)
target_compile_definitions(yips PRIVATE NOMINMAX)
target_precompile_headers(yips PRIVATE ${YIPS_SRC}/yips/pch.h)
# kazilib's imgui sources build without yips' header
set_source_files_properties(${KAZILIB_WEB_IMGUI_SOURCES} ${KAZILIB_WEB_SOURCES} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
target_link_libraries(yips PRIVATE SFML::Graphics)
# Only what yips reads (deploy\data\sprites and the .psd sources stay out)
target_link_options(yips PRIVATE
	"--preload-file=${YIPS_DATA}/CascadiaCode.ttf@/data/CascadiaCode.ttf"
	"--preload-file=${YIPS_DATA}/game_title.png@/data/game_title.png"
	"--preload-file=${YIPS_DATA}/ship_1.png@/data/ship_1.png"
	"--preload-file=${YIPS_DATA}/scenario/t1@/data/scenario/t1"
	"--preload-file=${YIPS_DATA}/scenario/t2@/data/scenario/t2"
	"--preload-file=${YIPS_DATA}/scenario/t3@/data/scenario/t3")
