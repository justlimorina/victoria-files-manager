@echo off
set "ROOT=%~dp0"
set "PATH=%ROOT%build\libnautilus-extension;%ROOT%build\subprojects\gnome-autoar\gnome-autoar;C:\msys64\ucrt64\bin;C:\msys64\usr\bin;%PATH%"
set "GSETTINGS_SCHEMA_DIR=%ROOT%build\schemas"
set "GDK_BACKEND=win32"

"%ROOT%build\src\victoria.exe" %*
