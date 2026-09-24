# Release verification

## Tested locally

- C++17 engine compiled and its regression suite passed: egg/clock gating; 30-second hatching; 72-hour evolution boundary; battle XP gating; care-independent evolution; sleeping, paused and suspended timers; evolution counter resets; all three Eevee branches; invalid action rejection; deterministic battle rewards; malformed-save rejection; save round trips; **1,580 complete simulated battles** over all 79 partner families and four routes.
- Actual Emscripten WebAssembly build passed timer, hatch, sleep/pause, invalid-import and **native-to-WebAssembly save compatibility** checks.
- Windows x64 native app linked statically, created its OpenGL window, loaded all **151 sprites**, rendered and saved successfully. Native screenshot inspected, including Windows high-DPI rendering.
- Browser UI: chose a starter, set clock, hatched in real elapsed time, trained, completed a battle using type burst/guard/tackle, received XP, and imported a native save through the file picker. Used a prepared 72-hour test fixture to check Eevee choice and evolution reveal; this did not require waiting 72 real hours.
- Responsive interface visually inspected at desktop and 390-pixel phone width; 320-pixel layout checked for horizontal overflow.
- All 151 source sprite hashes, dimensions, filenames and allowed Yellow paths verified. All 151 species are reachable through playable families/evolution and were encountered in a deterministic League simulation.
- Early ten-family preview saves successfully import into the expanded Vibe Beasts format.
- Offline browser proof: stopped the local server, evolved Eevee into Vaporeon, then reloaded from the cache with saved progress intact.
- Windows installer tested in isolated local folders: Vibe Beasts shortcut, identical executable hash, 151 sprites, and the installed copy passed its native smoke test.

## Scope

The Windows renderer and browser UI are separate interfaces over the same C++ rules. The native smoke test validates rendering/resources and save startup; it is not a complete manual playthrough of every Windows button. Phone checks use browser viewport emulation, not a physical Android/iPhone. Safari installation and OS download/reputation prompts were not independently tested. macOS/Linux native runtime behavior is not claimed verified.

The game intentionally uses local single-player saves without cloud synchronization, multiplayer, anti-cheat, official Pokémon battle fidelity or official game mechanics.

CI configuration is in [build.yml](.github/workflows/build.yml). See the repository's Actions tab for the current run rather than treating this document as a live CI status indicator.
