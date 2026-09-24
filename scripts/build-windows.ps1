param([string]$Compiler='clang++',[Parameter(Mandatory=$true)][string]$Raylib)
$ErrorActionPreference='Stop'
$Raylib=(Resolve-Path $Raylib).Path
Push-Location (Join-Path $PSScriptRoot '..')
try {
 New-Item -ItemType Directory -Force build,build/windows | Out-Null
 & $Compiler -std=c++17 -O2 -Wall -Wextra src/game.cpp tests/core-tests.cpp -o build/core-tests.exe -static
 if($LASTEXITCODE -ne 0){throw 'Core test compilation failed'}
 & .\build\core-tests.exe
 if($LASTEXITCODE -ne 0){throw 'Core tests failed'}
 & $Compiler -std=c++17 -O2 -Wall -Wextra src/game.cpp src/desktop.cpp "-I$Raylib/include" "$Raylib/lib/libraylib.a" -lopengl32 -lgdi32 -lwinmm -static -o build/windows/VibeBeasts.exe
 if($LASTEXITCODE -ne 0){throw 'Desktop build failed'}
 Copy-Item -Recurse -Force dist/assets build/windows/
 Copy-Item assets/RAYLIB-LICENSE.txt,dist/icon-192.png build/windows/
 Copy-Item packaging/Play.cmd,packaging/Install.cmd,packaging/install.ps1,packaging/README.md,CREDITS.md,LICENSE build/windows/
 Write-Output 'Built build/windows/VibeBeasts.exe'
}finally{Pop-Location}
