#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
em++ src/game.cpp -std=c++17 -O2 --no-entry -sMODULARIZE=1 -sEXPORT_ES6=1 -sEXPORT_NAME=createPocketCore -sENVIRONMENT=web,node -sALLOW_MEMORY_GROWTH=1 '-sEXPORTED_FUNCTIONS=["_pp_get","_pp_pet","_pp_species","_pp_name","_pp_message","_pp_tick","_pp_action","_pp_save","_pp_load"]' '-sEXPORTED_RUNTIME_METHODS=["cwrap"]' -o dist/core.js
node scripts/make-service-worker.mjs
