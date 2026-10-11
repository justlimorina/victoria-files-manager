# Victoria Files Manager

[![Platform](https://img.shields.io/badge/Platform-Windows%2010%20%7C%2011-0078D6?logo=windows&logoColor=white)](https://microsoft.com/windows)
[![UI Toolkit](https://img.shields.io/badge/UI-GTK4%20%2B%20Libadwaita-3584E4?logo=gnome&logoColor=white)](https://gnome.org)
[![Build](https://img.shields.io/badge/Build-MSYS2%20UCRT64%20%7C%20Meson%20%7C%20Ninja-blue)](https://msys2.org)
[![License](https://img.shields.io/badge/License-GPL--3.0--or--later-green.svg)](LICENSE)

[**🇻🇳 Tiếng Việt**](README_vi.md) | **English**

---

**Victoria** is a native Windows port of **GNOME Files (Nautilus 47)**, bringing GNOME's elegant, modern file management experience directly to Windows 10 and 11 without requiring WSL, virtualization, or X11/Wayland servers.

> *"Không có việc gì khó / Chỉ sợ lòng không bền / Đào núi và lấp biển / Quyết chí cũng làm nên"* — Chủ tịch Hồ Chí Minh.

---

## ✨ Features

- 🖥️ **Native Windows Application (`victoria.exe`)**: Runs directly on Windows (x86_64) using the native Win32 GDK backend with hardware acceleration.
- 🎨 **GTK4 & Libadwaita Interface**: Clean, modern GNOME UI with adaptive views, smooth animations, and automatic Light/Dark mode detection via the Windows Registry (`AppsUseLightTheme`).
- 💾 **Drive Letters & Partitions**: Native support for Windows drive letters (`C:\`, `D:\`, etc.) and volumes in the sidebar via `GWin32VolumeMonitor` and `GetLogicalDriveStringsW`.
- 🗑️ **Windows Recycle Bin Integration**: Seamlessly mapped to the Windows Recycle Bin (`SHQueryRecycleBinW`, `SHEmptyRecycleBinW`, `g_file_trash`) with live full/empty status icons and context menu actions.
- ⚡ **Native Shell File Launching**: Integrated with Windows `ShellExecuteExW` for 100% compatibility when opening files with default applications—fully compatible with Microsoft Office (Word, PowerPoint, Excel), Adobe Creative Cloud, media players, and Windows Shell extensions.
- 🔍 **Standalone Recursive Search**: Powered by `NautilusSearchEngineSimple` for fast directory searches without requiring background Linux daemons.
- 🪶 **Zero Linux Daemons Required**: Uses Win32 Named Pipes for single-instance IPC; no D-Bus session bus or GVfs daemon needed.

---

## 🚀 Getting Started

### Prerequisites

You need [MSYS2](https://www.msys2.org/) with the **UCRT64** environment.

Open the **MSYS2 UCRT64** terminal and install the required dependencies:

```bash
pacman -S --needed \
  mingw-w64-ucrt-x86_64-gcc \
  mingw-w64-ucrt-x86_64-meson \
  mingw-w64-ucrt-x86_64-ninja \
  mingw-w64-ucrt-x86_64-pkgconf \
  mingw-w64-ucrt-x86_64-gtk4 \
  mingw-w64-ucrt-x86_64-libadwaita \
  mingw-w64-ucrt-x86_64-glib2 \
  mingw-w64-ucrt-x86_64-libxml2 \
  mingw-w64-ucrt-x86_64-gexiv2 \
  mingw-w64-ucrt-x86_64-libportal \
  mingw-w64-ucrt-x86_64-libportal-gtk4
```

### Building Victoria

From the project root directory in PowerShell or MSYS2:

```powershell
# Set up toolchain in PATH (if running from PowerShell)
$env:PATH = "C:\msys64\ucrt64\bin;C:\msys64\usr\bin;$env:PATH"

# Configure the build directory
meson setup build --buildtype=release -Dtests=none -Ddocs=false

# Compile
ninja -C build
```

### Running Victoria

You can launch Victoria using the provided launcher scripts:

**Using PowerShell:**
```powershell
.\run-victoria.ps1
```

**Using Command Prompt / Batch:**
```cmd
run-victoria.bat
```

Or run the compiled executable directly with runtime environment set:
```powershell
$env:PATH = "D:\Projects\victoria\build\libnautilus-extension;D:\Projects\victoria\build\subprojects\gnome-autoar\gnome-autoar;C:\msys64\ucrt64\bin;C:\msys64\usr\bin;$env:PATH"
$env:GSETTINGS_SCHEMA_DIR = "D:\Projects\victoria\build\schemas"
$env:GDK_BACKEND = "win32"
.\build\src\victoria.exe
```

---

## 🏛️ Architecture & Porting Strategy

| Component | Linux (Upstream Nautilus) | Windows (Victoria) |
|---|---|---|
| **Executable** | `nautilus` (ELF) | `victoria.exe` (PE32+ / Win32) |
| **Application ID** | `org.gnome.Nautilus` | `org.victoria.Victoria` |
| **GSettings Schema** | `org.gnome.nautilus` | `org.victoria` |
| **Drives & Volumes** | UDisks2 / GVfs | `GetLogicalDriveStringsW` + `GWin32VolumeMonitor` |
| **Recycle Bin** | `trash:///` (GVfs) | Win32 Recycle Bin (`SHEmptyRecycleBinW`, `SHQueryRecycleBinW`) |
| **File Launching** | `GDesktopAppInfo` + `.desktop` | `ShellExecuteExW` + Windows Registry (`HKCR`) |
| **IPC / Single Instance** | D-Bus Session Bus | Win32 Named Pipes (`GApplication`) |
| **Theme Sync** | `org.gnome.desktop.interface` | Windows Registry (`AppsUseLightTheme`) |

---

## 📜 Upstream & Credits

Victoria is based on [GNOME Files (Nautilus)](https://gitlab.gnome.org/GNOME/nautilus) developed by the GNOME Foundation and contributors.

Licensed under the **GNU General Public License v3.0 or later** (GPL-3.0-or-later).
