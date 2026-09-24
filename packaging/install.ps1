param([string]$InstallRoot=(Join-Path $env:LOCALAPPDATA 'PocketPartner\app'), [string]$ShortcutDirectory=([Environment]::GetFolderPath('Desktop')), [switch]$NoLaunch)
$ErrorActionPreference='Stop'
$destination=$InstallRoot
New-Item -ItemType Directory -Force $destination | Out-Null
foreach($name in @('PocketPartner.exe','assets','Play.cmd','README.md','CREDITS.md','LICENSE','RAYLIB-LICENSE.txt')){
 Copy-Item -LiteralPath (Join-Path $PSScriptRoot $name) -Destination $destination -Recurse -Force
}
$shell=New-Object -ComObject WScript.Shell
$shortcut=$shell.CreateShortcut((Join-Path $ShortcutDirectory 'Pocket Partner.lnk'))
$shortcut.TargetPath=Join-Path $destination 'PocketPartner.exe'
$shortcut.WorkingDirectory=$destination
$shortcut.Save()
if(-not $NoLaunch){Start-Process -FilePath (Join-Path $destination 'PocketPartner.exe') -WorkingDirectory $destination -WindowStyle Normal}
