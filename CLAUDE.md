# CLAUDE.md

Follows kazilib's rules (`src\dep\kazilib\CLAUDE.md`). README.md: build, controls, tests.

- **Build:** `powershell -ExecutionPolicy Bypass -File build.ps1` (v145, C++latest, `/MD`; kazilib FASTBuild on `src\fastbuild.json`); stops this checkout's `yips.exe` first. `setup.ps1` checks kazilib out only when empty.
- **Web:** `build_web.ps1` (Emscripten, `src\CMakeLists.txt` = the web build only, kazilib `web\`: `kazilib_web_target` + `kazilib_web_imgui`, shared SFML `D:\code\tools\kazilib-web`) → `src\build\web\yips-build\`; data preloaded under `/data`. Differences under `#ifdef __EMSCRIPTEN__` (no frame sleep, Esc back to Start, E opens the editor), Windows-only under `#ifdef _WIN32` (Testerino bridge, HWND, `timeGetTime` via `NowMs()`, exe folder). Test `src\tests\web\run_web_test.ps1` (`-Url` live), 0 failed; editor status lines print `Editor: <status>` for it.
- **Deploy:** `deploy_ubuntu.ps1` → box `kazin-music`: `/opt/yips/web`, Caddy `kazin.dev/projects/yips` (`deploy\ubuntu\`, static, no service).
- **kazilib first:** imgui, imgui-SFML, magic_enum, json, rang, launch flags, exception reports, Testerino are kazilib's, no copies here; reusable code goes to kazilib `lib/shared`, built in its other consumers before push.
- **SFML 3:** events via `pollEvent()` `optional` + `getIf<>`; `sf::Sprite` / `sf::Text` built after their texture / font load (`optional`); rects `position` / `size`; `sf::degrees`.
- **Scenes** change between frames (`Scenes::Set` / `Play`, `ApplyPending`), never inside a scene's call; objects added mid-loop via `Scenario::Spawn`.
- **Keys held:** `input::IsDown`, never `sf::Keyboard::isKeyPressed`.
- **Tests:** `src\tests\testerino\*.tt` in the background, outside the command sandbox, 0 failed; new features get verbs in `src\yips\Testerino.cpp` and a routine.
