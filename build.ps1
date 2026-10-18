# Builds kazilib and yips (deploy\yips.exe): Release, x86, with the 64-bit-hosted
# tools, through kazilib's FASTBuild runner (src\dep\kazilib\fastbuild\build.ps1,
# src\fastbuild.json), distributed to the workers that answer; MSBuild when there is
# no FBuild. Runs setup.ps1 first for any library still missing. Arguments go to the
# runner: aliases (kazilib, yips; all by default), -Configuration Debug (only when
# asked), -Regen, -Clean, -Local.
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot

& (Join-Path $root 'setup.ps1')

& (Join-Path $root 'src\dep\kazilib\fastbuild\build.ps1') -Config (Join-Path $root 'src\fastbuild.json') @args
exit $LASTEXITCODE
