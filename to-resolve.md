# to-resolve.md (yips)

## Web build and kazin.dev (2026-11-06; continue.md)

Manual checks:

- **A real browser with a GPU** (Edge or Chrome, then Firefox and Safari): open
  https://kazin.dev/projects/yips/, play (arrows, Shift, A, Q W E R S D F, O / L), then press E
  for the editor: New, place obstacles, Save As, Open, Ctrl+P. Why: only headless Edge
  (software WebGL) was tested; Safari's WebGL2 and keys differ most.
- **The browser's own shortcuts** in the editor: Ctrl+S, Ctrl+O and Ctrl+P (save page,
  open file, print). Check that the game gets them and the browser doesn't. Why: SDL
  cancels the keys it takes on the canvas, but a browser may keep some for itself (Ctrl+E
  is why E alone opens the editor).
- **The clipboard with other programs:** copy a name in another app and Ctrl+V it into
  Save As; copy from Save As and paste elsewhere. Why: only copy / paste inside the page
  was tested (SDL3's clipboard on the browser's asynchronous clipboard API).
- **Fullscreen and Scaled Window:** clicks on the editor still land where the pointer is.
- **From outside the home network** (mobile data): the page loads. Why: the tests ran on
  the LAN (dnsmasq resolves kazin.dev to the box).

Open questions:

- **Saved scenarios in a browser** live in memory (MEMFS) until the page reloads. Keep them
  with IDBFS (the browser's storage), add a download / upload, or leave it as is?
- **No sound** in yips today. If sound is added, SFML's audio module isn't built for the
  web (kazilib `web/README.md`): sounds would have to stay off under `__EMSCRIPTEN__` or the
  module be ported.
- **README:** the one README (2026-10-18, `16fba16`) was written in the kazilib move, not
  6+ months ago, so the web sections went into it directly, with no "Original README"
  section. Keep it that way?
