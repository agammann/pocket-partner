# Rebuild Pocket Partner

For ready-to-play downloads, see [README.md](README.md). Rebuilding requires a compiler; playing a prebuilt release does not.

## Versions

- C++17 compiler; the Windows release is built with LLVM MinGW UCRT x64 `20260908`.
- raylib **5.5**, static Windows MinGW library.
- Emscripten **4.0.15** for WebAssembly.
- Node.js 22 or later for serving, offline-cache generation and WebAssembly tests.
- CMake 3.20+ for the optional cross-platform build.

Dependencies are downloaded on first setup. Once downloaded, builds can run without network access. Generated web artifacts are committed in `dist/` so the repository is directly playable through a local web server without a compiler.

## Windows native build

Download the [raylib 5.5 MinGW package](https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_win64_mingw-w64.zip) and a C++ compiler such as [LLVM MinGW](https://github.com/mstorsjo/llvm-mingw/releases/tag/20260908). Extract them, then run from the repository root:

```powershell
.\scripts\build-windows.ps1 -Compiler 'C:\path\to\llvm-mingw\bin\clang++.exe' -Raylib 'C:\path\to\raylib-5.5_win64_mingw-w64'
.\build\windows\PocketPartner.exe
```

The build script compiles and runs the C++ tests first, then creates `build/windows/` with the executable, all sprites, player documentation, and Play/Install launchers. It statically links the runtime and raylib.

```powershell
.\build\windows\PocketPartner.exe --smoke-test
.\build\windows\PocketPartner.exe --snapshot build/native-preview.png
```

Smoke mode uses a separate save in `build/smoke.save`, loads all sprites, renders eight frames and exits. It never uses the player’s normal save. Run these commands from the repository root; the renderer also finds sprites under `dist/assets/`.

## Browser build

Install and activate [Emscripten 4.0.15](https://emscripten.org/docs/getting_started/downloads.html), then enter its configured shell (`emsdk_env.bat` on Windows). Run:

```powershell
.\scripts\build-web.ps1
node tests/web-core.mjs
node scripts/serve.mjs
```

`tests/web-core.mjs` expects `build/test-transfer.save`, written by the native core tests. It loads that save into the actual WebAssembly module to verify interoperability.

Open http://localhost:4173. Do not double-click `dist/index.html`: browsers require HTTP/HTTPS for WebAssembly loading and service workers. The built web bundle is entirely local; it has no external runtime asset dependencies.

On Linux/macOS, use `scripts/build-web.sh` from an Emscripten-enabled shell. `node scripts/make-service-worker.mjs` must run after any web asset change; it hashes all cached files to generate a fresh offline cache version.

## CMake

```sh
cmake -S . -B build/cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build/cmake --config Release
ctest --test-dir build/cmake --output-on-failure
```

CMake downloads raylib 5.5 on its first native configure. Linux needs raylib’s X11/OpenGL development dependencies. Run the resulting executable from the repository root, or copy `dist/assets/` beside it. Native macOS/Linux builds are supported by this source layout but have not been runtime-tested for this release.

## Publishing

The public source repository is https://github.com/agammann/pocket-partner. The browser version targets ChatGPT Sites using `dist/` as its static directory. For your own deployment, replace the Site identity in `.openai/hosting.json` with your own; do not reuse this project's identity. Any static HTTPS host can serve `dist/` with `.wasm` as `application/wasm`.

GitHub Actions rebuilds and tests the C++ core, creates a Linux desktop build, rebuilds WebAssembly and checks the shared save fixture. Windows release packages are built separately using the script above. See [VERIFIED.md](VERIFIED.md) for the precise tested scope.
