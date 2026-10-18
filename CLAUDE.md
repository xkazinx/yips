# CLAUDE.md

yips follows `D:\code\CLAUDE.md` ("upd", commit messages, shared code) and kazilib's rules
(`src\dep\kazilib\CLAUDE.md`: names `_member` / `PascalCase`, globals `g_` in `namespace global`,
`Under` / magic_enum, stdnames without `std::`, `v += x`, loops standing apart, const parameters,
Release only). README.md has the build, the controls and the tests.

- **Build:** `powershell -ExecutionPolicy Bypass -File build.ps1` (Release|Win32, v145, C++latest,
  `/MD`, 64-bit-hosted tools producing x86) after every prompt that changes code, through
  kazilib's FASTBuild runner (`src\fastbuild.json`; the `.vcxproj` files are the one place
  settings live, the `.bff` is generated from them); it stops this checkout's `yips.exe` first. `setup.ps1` checks the kazilib submodule out only when it is empty,
  so a kazilib commit being worked on is kept.
- **kazilib first:** imgui, imgui-SFML, magic_enum, json, rang, launch flags, exception reports
  and Testerino are kazilib's; yips has no copy of any. Something yips needs that another project
  could use goes into kazilib (`lib/shared`), built in its other consumers before it is pushed.
- **SFML 3:** events through `pollEvent()`'s `optional` and `getIf<>`; a `sf::Sprite` / `sf::Text`
  needs its texture / font when made (an `optional` built after the load); rects are
  `position` / `size`; angles `sf::degrees`.
- **Scenes** change between frames (`Scenes::Set` / `Play`, applied by `ApplyPending`), never from
  inside a scene's own call. Objects added while a scenario loops over its objects go through
  `Scenario::Spawn`.
- **Keys held** are read through `input::IsDown` (focus-aware, and the keys a test holds), never
  `sf::Keyboard::isKeyPressed` directly.
- **Testing:** "testerino" runs `src\tests\testerino\*.tt` (`node src\dep\kazilib\testerino\tt.js run
  <file>`, in the background, outside the command sandbox), 0 failed; a new feature gets its
  verbs in `src\yips\Testerino.cpp` and its scenarios in a routine.
