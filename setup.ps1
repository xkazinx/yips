# The libraries yips builds against, x86 (Win32):
#   kazilib  src\dep\kazilib  (submodule: imgui + imgui-SFML, magic_enum, nlohmann json,
#            rang, std names, startup flags, exception reports, Testerino)
#   SFML     src\dep\SFML-3.1.0  (a junction to D:\code\kazilib\SFML-3.1.0 when that
#            shared SDK is there, as the other projects use; else the official
#            vc17 32-bit release, downloaded)
# Every build uses the 64-bit-hosted tools (host=x64) and the v145 toolset.
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot
$dep = Join-Path $root 'src\dep'

# Only an empty submodule is checked out: a kazilib being worked on keeps its commit
if (-not (Test-Path (Join-Path $dep 'kazilib\kazilib.vcxproj'))) {
    git -C $root submodule update --init src/dep/kazilib
    if ($LASTEXITCODE) { throw 'submodule update failed' }
}

# SFML 3.1.0
$sfml = Join-Path $dep 'SFML-3.1.0'
if (-not (Test-Path (Join-Path $sfml 'include\SFML\Graphics.hpp'))) {
    $shared = 'D:\code\kazilib\SFML-3.1.0'
    if (Test-Path (Join-Path $shared 'include\SFML\Graphics.hpp')) {
        New-Item -ItemType Junction -Path $sfml -Target $shared | Out-Null
        Write-Host "SFML: linked to $shared"
    } else {
        $zip = Join-Path $env:TEMP 'SFML-3.1.0-windows-vc17-32-bit.zip'
        Write-Host 'SFML: downloading 3.1.0 (vc17 32-bit)'
        Invoke-WebRequest 'https://github.com/SFML/SFML/releases/download/3.1.0/SFML-3.1.0-windows-vc17-32-bit.zip' -OutFile $zip
        Expand-Archive $zip -DestinationPath $dep -Force
        Remove-Item $zip
    }
}

Write-Host 'Dependencies ready'
