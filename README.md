# Victoria Files Manager

[![Platform](https://img.shields.io/badge/platform-Windows%2010%20%7C%2011-0078D6?logo=windows&logoColor=white)](https://www.microsoft.com/windows)
[![Toolkit](https://img.shields.io/badge/toolkit-GTK4%20%2B%20Libadwaita-3584E4?logo=gnome&logoColor=white)](https://www.gtk.org/)
[![Build](https://img.shields.io/badge/build-MSYS2%20UCRT64%20%7C%20Meson%20%7C%20Ninja-4C8BF5)](https://www.msys2.org/)
[![License](https://img.shields.io/badge/license-GPL--3.0--or--later-2E7D32)](LICENSE)

[Tiếng Việt](README_vi.md) · [GitHub repository](https://github.com/justlimorina/victoria-files-manager)

Victoria is a Windows-focused port of [GNOME Files (Nautilus)](https://gitlab.gnome.org/GNOME/nautilus). It keeps the GTK4 and Libadwaita file-manager experience while replacing key Linux integrations with Windows-native equivalents. It targets 64-bit Windows 10 and Windows 11 and builds as `victoria.exe`.

## Highlights

- GTK4 and Libadwaita interface with Windows light/dark-theme detection.
- Browse folders in grid and list views; use tabs, the sidebar, bookmarks, and search.
- Windows drives and volumes in the sidebar, including drive-letter roots and mounted volumes.
- Native file launching through Windows file associations and Shell APIs.
- Windows Recycle Bin support: move items to the bin, track its empty/full state, empty it, and open a Victoria-owned Recycle Bin window to list, restore, or permanently delete items.
- Single-instance behaviour through `GApplication` on Windows; no WSL, X11 server, or D-Bus session daemon is required.

## Project status

Victoria is an active Windows port, not an official GNOME release. It still inherits a large Nautilus codebase, so some upstream Linux-oriented features may be incomplete or behave differently on Windows. Please report reproducible issues with the Windows version, build command, and steps to reproduce in the [issue tracker](https://github.com/justlimorina/victoria-files-manager/issues).

## Build from source

### Requirements

- Windows 10 or Windows 11, 64-bit
- [MSYS2](https://www.msys2.org/) using the **UCRT64** environment
- Git (to clone the repository)

First update MSYS2, then open the **MSYS2 UCRT64** shell and install the development packages:

```bash
pacman -Syu
pacman -S --needed \
  mingw-w64-ucrt-x86_64-gcc \
  mingw-w64-ucrt-x86_64-meson \
  mingw-w64-ucrt-x86_64-ninja \
  mingw-w64-ucrt-x86_64-pkgconf \
  mingw-w64-ucrt-x86_64-gtk4 \
  mingw-w64-ucrt-x86_64-libadwaita \
  mingw-w64-ucrt-x86_64-glib2 \
  mingw-w64-ucrt-x86_64-libxml2 \
  mingw-w64-ucrt-x86_64-libarchive \
  mingw-w64-ucrt-x86_64-gexiv2 \
  mingw-w64-ucrt-x86_64-gstreamer \
  mingw-w64-ucrt-x86_64-libportal \
  mingw-w64-ucrt-x86_64-libportal-gtk4
```

Clone and build:

```bash
git clone https://github.com/justlimorina/victoria-files-manager.git
cd victoria-files-manager
meson setup build --buildtype=release -Dtests=none
meson compile -C build
```

The executable is created at `build/src/victoria.exe`.

### Build from PowerShell

Use this when working from PowerShell rather than the MSYS2 UCRT64 shell:

```powershell
$env:MSYSTEM = 'UCRT64'
$env:CHERE_INVOKING = 'yes'
& C:\msys64\usr\bin\bash.exe -lc 'cd /d/Projects/victoria && ninja -C build'
```

Replace `/d/Projects/victoria` with your checkout path. Close Victoria before rebuilding; Windows locks a running `victoria.exe` and the linker cannot overwrite it.

### Reconfigure or clean a build

```bash
meson setup build --reconfigure -Dtests=none
meson compile -C build
```

For a completely fresh build directory:

```bash
meson setup build --wipe --buildtype=release -Dtests=none
meson compile -C build
```

## Run

The included launchers set the DLL and GSettings paths needed by an in-tree build:

```powershell
.\run-victoria.ps1
```

```cmd
run-victoria.bat
```

To pass a location, append it to either command, for example:

```powershell
.\run-victoria.ps1 D:\Projects
```

## Windows integration

| Area | Victoria implementation |
| --- | --- |
| Executable | `victoria.exe` on the Win32 GTK backend |
| Application ID | `org.victoria.Victoria` |
| Drives and volumes | `GWin32VolumeMonitor` and Win32 drive APIs |
| File associations | Windows Shell and registered file associations |
| Recycle Bin | Windows Shell, `SHQueryRecycleBinW`, `SHEmptyRecycleBinW`, and native trash operations |
| Theme | Windows `AppsUseLightTheme` registry setting |
| Single instance | `GApplication` Windows IPC |

## Contributing

Fork the repository, create a focused branch, and submit a pull request. Please keep Windows-specific changes behind the appropriate platform guards, preserve upstream attribution, and include build or runtime verification in the pull-request description.

## Credits and license

Victoria is derived from GNOME Files (Nautilus), developed by the GNOME Foundation and its contributors. Victoria-specific work is maintained in the [Victoria repository](https://github.com/justlimorina/victoria-files-manager).

The project is licensed under the [GNU General Public License, version 3 or later](LICENSE).
