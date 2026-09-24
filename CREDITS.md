# Credits

Game design, implementation and publication: **agammann**.

## Pokémon artwork

Pokémon Yellow sprites archived by **The RBY Sprites Project**, maintained by **Plague von Karma**:

https://github.com/ShiraTheMogul/rby-sprites-project

Pinned source revision: `07db37f903e8697b791e017d5adae0cd66e3d3a7`.

Source paths: `yellow/gen1/56x56/NNN.png`, with Jynx from `124-GB.png` renamed to `124.png`. The 151 original transparent PNGs are included without pixel changes. The upstream README permits use with repository attribution. The source manifest records the original URL, Git blob hash, SHA256, dimensions and transparency for each file.

**Asset policy: Pokémon Yellow only.** Betamons, prototype designs, SpaceWorld sets, glitch Pokémon, and all other games' sprite sets are excluded. `node tests/assets.mjs` checks the allowed source folder, roster, file list, dimensions and every SHA256 hash.

Pokémon names, characters, sprites and associated intellectual property belong to their respective rights holders, including Nintendo, Creatures and GAME FREAK. This project is not affiliated with or endorsed by them. Upstream archive permission does not resolve underlying franchise rights. The source-code license explicitly excludes third-party art, names and trademarks.

## Libraries

- **raylib 5.5** by Ramon Santamaria and contributors — zlib/libpng license. Its license is included in `assets/RAYLIB-LICENSE.txt` and Windows packages.
- **Emscripten 4.0.15** — MIT/University of Illinois NCSA licenses. Used to compile the C++ core and generate its JavaScript runtime. See https://github.com/emscripten-core/emscripten/blob/4.0.15/LICENSE.

## Inspiration

Tamaweb and Digital Monster Web informed the virtual-pet concept. Pokémon games informed familiar evolution families and turn-based battle ideas. No code, interface artwork, music or sound from those games was copied. Evolution timing and battle rules here are custom.

## Species data

Original-151 names, family relationships and primary types were derived from [PokeAPI](https://github.com/PokeAPI/pokeapi), revision `2b9ea590fcc62a1bce2534835e9d5d1b234a2976`. Later-generation relatives and types are excluded. Pidgey uses Flying as its single battle type. The checked-in roster and provenance are in `assets/roster.json`.
