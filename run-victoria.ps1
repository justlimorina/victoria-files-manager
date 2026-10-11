# Victoria Launcher for PowerShell
$Root = $PSScriptRoot
$env:PATH = "$Root\build\libnautilus-extension;$Root\build\subprojects\gnome-autoar\gnome-autoar;C:\msys64\ucrt64\bin;C:\msys64\usr\bin;$env:PATH"
$env:GSETTINGS_SCHEMA_DIR = "$Root\build\schemas"
$env:GDK_BACKEND = "win32"

& "$Root\build\src\victoria.exe" $args
