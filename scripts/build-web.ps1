param([string]$Emcc = 'em++')
$ErrorActionPreference = 'Stop'
Push-Location (Join-Path $PSScriptRoot '..')
try {
  $exports = '["_pp_get","_pp_pet","_pp_species","_pp_name","_pp_message","_pp_tick","_pp_action","_pp_save","_pp_load"]'
  & $Emcc src/game.cpp -std=c++17 -O2 --no-entry -sMODULARIZE=1 -sEXPORT_ES6=1 -sEXPORT_NAME=createPocketCore '-sENVIRONMENT=web,node' -sALLOW_MEMORY_GROWTH=1 "-sEXPORTED_FUNCTIONS=$exports" '-sEXPORTED_RUNTIME_METHODS=["cwrap"]' -o dist/core.js
  if ($LASTEXITCODE -ne 0) { throw 'WebAssembly build failed' }
  node scripts/make-service-worker.mjs
  if ($LASTEXITCODE -ne 0) { throw 'Offline manifest generation failed' }
} finally { Pop-Location }
