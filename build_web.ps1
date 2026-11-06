# yips for a web browser (WebAssembly): src\CMakeLists.txt with Emscripten, against
# kazilib's SFML web build. Output src\build\web\yips-build\index.html / .js / .wasm /
# .data (serve that folder, or deploy_ubuntu.ps1 sends it to kazin.dev/projects/yips).
#   powershell -ExecutionPolicy Bypass -File build_web.ps1 [-Deps] [-Debug] [-Jobs N]
# SFML: the shared prebuilt web build in D:\code\tools\kazilib-web (-WebDeps <folder>),
#       built there by src\dep\kazilib\web\build_deps.ps1 when missing
# -Deps   rebuild SFML / SFGUI for the web first (after a kazilib web patch change)
# -Debug  function names in stack traces and Emscripten's memory checks
# -Jobs   parallel compiles (default: every logical CPU but one)
param([switch]$Deps, [switch]$Debug, [int]$Jobs = 0, [string]$WebDeps = 'D:\code\tools\kazilib-web')
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot
$emsdk = 'D:\code\tools\emsdk'
$sfmlDir = Join-Path $WebDeps 'sfml-web\lib\cmake\SFML'

if ($Deps -or -not (Test-Path (Join-Path $sfmlDir 'SFMLConfig.cmake'))) {
	& (Join-Path $root 'src\dep\kazilib\web\build_deps.ps1') -Out $WebDeps -Emsdk $emsdk
	if ($LASTEXITCODE) { throw 'web dependencies failed' }
}

$env:EMSDK_QUIET = 1
. (Join-Path $emsdk 'emsdk_env.ps1') | Out-Null
$ninja = (Get-Command ninja -ErrorAction SilentlyContinue)
$ninja = if ($ninja) { $ninja.Source } else {
	Get-ChildItem 'C:\Program Files\Microsoft Visual Studio' -Recurse -Filter ninja.exe -ErrorAction SilentlyContinue | Select-Object -First 1 -ExpandProperty FullName
}
if (-not $ninja) { throw 'ninja.exe not found (PATH or Visual Studio)' }

$build = Join-Path $root 'src\build\web\yips-build'
$linkFlags = if ($Debug) { '--profiling-funcs -sASSERTIONS=2 -sSAFE_HEAP=1' } else { '' }
emcmake cmake -S (Join-Path $root 'src') -B $build -G Ninja "-DCMAKE_MAKE_PROGRAM=$ninja" -DCMAKE_BUILD_TYPE=Release `
	-DSFML_STATIC_LIBRARIES=ON "-DSFML_DIR=$($sfmlDir -replace '\\', '/')" "-DCMAKE_EXE_LINKER_FLAGS=$linkFlags" | Out-Null
if ($LASTEXITCODE) { throw 'configure failed' }
if ($Jobs -le 0) { $Jobs = [Math]::Max(1, [Environment]::ProcessorCount - 1) }
cmake --build $build -j $Jobs
if ($LASTEXITCODE) { throw 'web build failed' }
Write-Host "Web build: $build"
