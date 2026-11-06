# continue.md (yips, master)

## Repos

| Repo | Commit | State |
|---|---|---|
| yips | master: the showcase retake with stars (this file's) on `9241a80` (the web / deploy commit) | pushed |
| kazilib (`src/dep/kazilib`) | master `ad10d04` (points drawn in a browser: the star field) on `a28bf1b` (ImGui in a browser) | pushed |

The kazilib pointer moved from `10d9db9` to master. kazilib master needed two fixes in yips:
`ranges::` is kazilib's now (`containers.h`), so pch.h's alias went, and Testerino uses
`tcore::Endpoint::_dir` (was `Dir()`).

## Last work (2026-11-06)

- **Web build:** yips (game and editor) compiled to WebAssembly: `build_web.ps1`,
  `src\CMakeLists.txt`. Data preloaded under `/data`; the editor saves to MEMFS.
- **ImGui in a browser (kazilib):** `imgui-SFML.cpp` draws through SFML under
  `__EMSCRIPTEN__` (one `RenderTarget::draw` per command, clip rect = view viewport,
  `ImTextureID` = `sf::Texture*`). Also `kazilib_web_imgui(<target>)`. A key pressed and
  released within one frame now reaches ImGui (in every build).
- **browser.js:** Ctrl / Shift / Alt on a key step, `hold`, `wheel`, key presses held 50 ms
  (merged with another agent's `down` / `up` from the same hour, a216c7d).
- **Browser-only behaviour:** Esc goes back to Start (not while typing), E alone opens the
  editor (Ctrl+E is the browser's search), and the Start screen says so.
- **Testerino:** verb `editor delete tt_<name>`; `editor.tt` deletes its scenario first
  (a second run used to fail 3 checks on the file the first run left).
- **Deployed:** https://kazin.dev/projects/yips/ (`deploy_ubuntu.ps1`, static files in
  `/opt/yips/web`, Caddy snippet `kazin.dev.d/yips.caddy`).
- **Star field in a browser:** `Background.cpp`'s `sf::PrimitiveType::Points` stars were 0 px
  on WebGL (the GL ES 1 emulation never set `gl_PointSize`); fixed in kazilib `ad10d04`
  by the coordinator, rebuilt and deployed.
- **Showcase (retaken with stars):** `docs\showcase\1_start` .. `5_open.png` (live page,
  `src\tests\web\showcase.json`), `docs\screenshot.png` (= 3_battle), kazin.dev photos
  `docs\showcase\kazin\yips.jpg` (= 2_play), `yips-1` .. `yips-4.jpg` (1024x768). The
  portfolio repo is not edited.

## Tested

| Test | Result |
|---|---|
| Testerino `play.tt` (native) | 108 passed, 0 failed |
| Testerino `editor.tt` (native, run twice in a row) | 86 passed, 0 failed (both) |
| `src\tests\web\run_web_test.ps1` (local build, headless Edge) | passed |
| same, `-Url https://kazin.dev/projects/yips/` | passed (again after `ad10d04`'s deploy) |

Known gaps: only Edge headless (SwiftShader) tested. No Firefox / Safari, no phone, no real
GPU, no system clipboard to or from other apps (the clipboard was only checked within the page).

## Next steps

- `to-resolve.md`: the manual checks and open questions.
- Saving scenarios across reloads (IDBFS) or as a download / upload.

## The user's prompts (this work)

1. "now I want you standarize the work and do the same with the githubs of yip, yips, tileship, tiled, nft, use various agents if well scoped"
2. "all the process we've done with tictactoe"
3. "I don't see the yip background"
