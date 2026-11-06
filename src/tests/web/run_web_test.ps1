# yips in a headless browser (kazilib web\tools\browser.js: Edge, WebGL2 on SwiftShader):
# the Start scene, a few seconds of play (obstacles, moving, shooting), back with Esc;
# the editor (ImGui in the browser): New with a typed chunk count, four obstacles
# placed, Save As (typing, the clipboard), five more saves, Open with the mouse wheel
# scrolling its list, Ctrl+P plays it. Screenshots in testerino\web\.
# Exit code = failed steps.
#   powershell -ExecutionPolicy Bypass -File src\tests\web\run_web_test.ps1 [-Url https://kazin.dev/projects/yips/]
# Without -Url it serves the local web build (src\build\web\yips-build, build_web.ps1).
param([string]$Url = '', [int]$Port = 8191)
$ErrorActionPreference = 'Stop'
$root = Resolve-Path (Join-Path $PSScriptRoot '..\..\..')
$shots = Join-Path $root 'testerino\web'
New-Item -ItemType Directory -Force $shots | Out-Null

$build = Join-Path $root 'src\build\web\yips-build'
$where = if ($Url) { @('--url', $Url) } else {
	if (-not (Test-Path (Join-Path $build 'index.wasm'))) { throw "no $build\index.wasm: run build_web.ps1" }
	@('--serve', $build, '--port', $Port)
}

# Screenshot names in the steps land in testerino\web\ (web_live_* for -Url)
$prefix = ($shots -replace '\\', '/') + '/' + $(if ($Url) { 'live_' } else { '' })
$steps = Join-Path $shots 'play_and_edit.json'
(Get-Content (Join-Path $PSScriptRoot 'play_and_edit.json') -Raw) -replace '"shot": "', ('"shot": "' + $prefix) |
	Set-Content -Encoding utf8 $steps

node (Join-Path $root 'src\dep\kazilib\web\tools\browser.js') @where --steps $steps
exit $LASTEXITCODE
