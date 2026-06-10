# Builds src/main.ino into wokwi/build/ so the Wokwi simulator can run it.
# Usage:  .\build.ps1
$ErrorActionPreference = 'Stop'

$env:Path += ';C:\Program Files\Arduino CLI'

$root = $PSScriptRoot
$tmp  = Join-Path $env:TEMP 'breathmill_sketch'

New-Item -ItemType Directory -Force -Path $tmp | Out-Null
Copy-Item (Join-Path $root 'src\main.ino') (Join-Path $tmp 'breathmill_sketch.ino') -Force

arduino-cli compile `
  --fqbn esp32:esp32:esp32 `
  --output-dir (Join-Path $root 'wokwi\build') `
  $tmp
