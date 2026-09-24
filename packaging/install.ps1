param([string]$InstallRoot=(Join-Path $env:LOCALAPPDATA 'VibeBeasts\app'), [string]$ShortcutDirectory=([Environment]::GetFolderPath('Desktop')), [switch]$NoLaunch)
$ErrorActionPreference='Stop'
$destination=$InstallRoot
New-Item -ItemType Directory -Force $destination | Out-Null
foreach($name in @('VibeBeasts.exe','icon-192.png','assets','Play.cmd','README.md','CREDITS.md','LICENSE','RAYLIB-LICENSE.txt')){
 Copy-Item -LiteralPath (Join-Path $PSScriptRoot $name) -Destination $destination -Recurse -Force
}
$shell=New-Object -ComObject WScript.Shell
$shortcut=$shell.CreateShortcut((Join-Path $ShortcutDirectory 'Vibe Beasts.lnk'))
$shortcut.TargetPath=Join-Path $destination 'VibeBeasts.exe'
$shortcut.WorkingDirectory=$destination
$shortcut.Save()
if(-not $NoLaunch){Start-Process -FilePath (Join-Path $destination 'VibeBeasts.exe') -WorkingDirectory $destination -WindowStyle Normal}
