# yips
A vertical space shooter (inspired by B-Wings) with a scenario editor, on SFML, ImGui and kazilib.

## Libraries

| Library | Version | Where |
|---------|---------|-------|
| kazilib | master (submodule) | `src/dep/kazilib`: imgui 1.84 + imgui-SFML (SFML 3), magic_enum, nlohmann json 3.12, rang, `stdnames.h`, `under.h`, `initflags.h` (launch flags), `exception_report.h`, Testerino (`testerino/tcore.h`, `tt.js`); built as its `ml` version through `kazilib.vcxproj` |
| SFML | 3.1.0 x86 | `src/dep/SFML-3.1.0`: a junction to `D:\code\kazilib\SFML-3.1.0`, or the official vc17 32-bit release |

## Building

Visual Studio 2026 (v145 toolset).

```
git clone --recurse-submodules https://github.com/xkazinx/yips.git
powershell -ExecutionPolicy Bypass -File build.ps1
```

`build.ps1` runs `setup.ps1` (kazilib submodule, SFML) and builds Release, x86, with the
64-bit-hosted compiler and linker, through kazilib's FASTBuild runner
(`src\dep\kazilib\fastbuild\`, configured by `src\fastbuild.json`: the `.bff` is generated
from the `.vcxproj` files, distributed to the workers that answer; MSBuild when FBuild is
missing) to `deploy\yips.exe`, with SFML's DLLs beside it. Its arguments go to the runner:
`kazilib` / `yips` (both by default), `-Configuration Debug`, `-Regen`, `-Clean`, `-Local`.
`src\yips.sln` opens the same projects in Visual Studio.

## Playing

Run `deploy\yips.exe` (it reads `deploy\data\` beside it, from any working folder).

- **Start:** Enter plays, Ctrl+E opens the scenario editor, Esc exits (from any scene).
- **Scenario:** the arrows move the ship (Shift doubles the speed); Q W E R A S D F use the
  abilities while they have ammo (A fires, 99 shots to start), Ctrl + the key spends an
  energy on its level; O / L add a waving / straight obstacle; F3 the numbers overlay.
  Four obstacles of a type destroyed in a row drop a power up: Energy, Velocity,
  Invincible or ammo (3) for an ability.
- **Editor:** New / Open / Save / Save As / Play Scenario (Ctrl+P plays the scenario being
  edited, saved or not); the Position tool places obstacles where you click, Selection
  toggles them and the Selection tasks align or separate the selected ones (by
  Separation pixels); Up / Down scroll (Shift faster), F1 the menu, F3 the overlay.
  Scenarios are JSON files in `deploy\data\scenario\`.

Launch flags (kazilib `initflags.h`, also from an `init.json` beside the exe):
`title=<text>`, `background` (opens behind the window in use), `testerino=<slot>`,
`catchexceptions` (an exception writes `deploy\exceptions\exception_<pid>.txt` / `.dmp` and
ends the program; without it the report is written and the JIT debugger still gets it).

## Testing (Testerino)

The window is driven through kazilib's test bridge (`src\dep\kazilib\testerino\README.md`):
`testerino=<slot>`, only in a source checkout (`src\yips.sln`). The routines start yips behind
your windows, press keys, click, check the answers and take screenshots:

```
node src\dep\kazilib\testerino\tt.js run src\tests\testerino\play.tt
node src\dep\kazilib\testerino\tt.js run src\tests\testerino\editor.tt
```

Reports in `testerino\reports\`, screenshots in `testerino\client_<slot>\shots\`.
Verbs: `state`, `scene`, `key <k|ctrl+k> [times]`, `hold <key> <ms>`, `down` / `up <key>`,
`click x y`, `obstacle <ahead_px> [wave|linear|still]`, `powerup <PowerUpID> [ahead_px]`,
`editor new|open|save|tool|place|select|task|play|menu`, `shot`, `close`, `quit`; conditions
for `waitfor`: `scene=`, `obstacles`, `power_ups`, `projectiles`, `destroyed`,
`power_ups_taken` (`=`, `>`, `<`). `help` lists them.
