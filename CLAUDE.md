# CLAUDE.md

Follows kazilib's rules (`src\dep\kazilib\CLAUDE.md`). README.md: build, controls, tests.

- **Build:** `powershell -ExecutionPolicy Bypass -File build.ps1` (v145, C++latest, `/MD`; kazilib FASTBuild on `src\fastbuild.json`); stops this checkout's `yips.exe` first. `setup.ps1` checks kazilib out only when empty.
- **kazilib first:** imgui, imgui-SFML, magic_enum, json, rang, launch flags, exception reports, Testerino are kazilib's, no copies here; reusable code goes to kazilib `lib/shared`, built in its other consumers before push.
- **SFML 3:** events via `pollEvent()` `optional` + `getIf<>`; `sf::Sprite` / `sf::Text` built after their texture / font load (`optional`); rects `position` / `size`; `sf::degrees`.
- **Scenes** change between frames (`Scenes::Set` / `Play`, `ApplyPending`), never inside a scene's call; objects added mid-loop via `Scenario::Spawn`.
- **Keys held:** `input::IsDown`, never `sf::Keyboard::isKeyPressed`.
- **Tests:** `src\tests\testerino\*.tt` in the background, outside the command sandbox, 0 failed; new features get verbs in `src\yips\Testerino.cpp` and a routine.
