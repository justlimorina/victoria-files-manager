# VICTORIA PROJECT: GNOME FILES (NAUTILUS) FOR WINDOWS

> *"Không có việc gì khó / Chỉ sợ lòng không bền / Đào núi và lấp biển / Quyết chí cũng làm nên"* — Chủ tịch Hồ Chí Minh.

---

## 1. UNBREAKABLE RULES FOR AI (CORE RULES)

1. **Never complain or back down:**
   - Strictly forbidden to use excuses such as "Nautilus is too tightly coupled to Linux/GNOME", "too complex", or "impossible to achieve".
   - Strictly forbidden to advise the user to abandon the project in favor of Windows File Explorer or alternative software.
   - When encountering build, compilation, or runtime errors, the AI's sole task is: **Analyze root cause $\rightarrow$ Write patches, bridges, or adapters $\rightarrow$ Drive the implementation to completion.**

2. **Project Identity & Rebranding:**
   - Project Name: **Victoria**
   - Executable Name: `victoria.exe`
   - Application ID: `org.victoria.Victoria`
   - GSettings Schema ID: `org.victoria` (replacing `org.gnome.nautilus`)
   - User-facing Display Name: **Victoria** (or *Victoria Files*)

---

## 2. ARCHITECTURE & TECHNICAL INFRASTRUCTURE

- **Target Operating System:** Windows 10 / Windows 11 (x86_64).
- **Toolchain Environment:** MSYS2 UCRT64 (`C:\msys64\ucrt64\bin`).
- **Compiler & Build System:** GCC (UCRT), Meson (`meson setup`), Ninja (`ninja`).
- **Graphical Toolkit:**
  - **GTK4** (`GDK_BACKEND=win32`) + **Libadwaita 1.x**.
  - Rendered via Win32 GDK backend (hardware-accelerated DirectX / WGL).
  - Automatic Light/Dark theme detection via Windows Registry (`AppsUseLightTheme`).

---

## 3. DESIGN PRINCIPLES & WINDOWS - LINUX PARITY

1. **Paths & Drive Letters:**
   - Linux has a single unified root `/`. Windows has multiple roots via drive letters (`C:\`, `D:\`) and UNC network paths (`\\server\share`).
   - Root directory check (`is_root_directory`): On Windows, a directory is a root when `g_file_get_parent(dir) == NULL` or is a drive root (`[A-Za-z]:\`).
   - Integrated `GWin32VolumeMonitor` and `GetLogicalDriveStringsW` to display drives and partitions in the sidebar.

2. **File Permissions & Attributes:**
   - Neutralize POSIX-specific calls that do not exist on Win32: `<pwd.h>`, `<grp.h>`, `chmod`, `chown`, `getpwnam`, `getgrnam`.
   - Map file attributes to standard Windows attributes: **Read-only**, **Hidden**, **System**, **Archive** via `GetFileAttributesW` / `SetFileAttributesW`.

3. **Recycle Bin:**
   - Independent of GVfs daemon `trash:///`.
   - Use native Win32 Recycle Bin mechanisms: `g_file_trash()` (backed by `SHFileOperationW` / `IFileOperation`), `SHQueryRecycleBinW`, and `SHEmptyRecycleBinW`.

4. **Default Application Launching (File Associations):**
   - Replace Linux `.desktop` files and `GDesktopAppInfo` with native Windows Shell integration: `GWin32AppInfo` paired with Windows Registry (`HKCR`) and `ShellExecuteExW` to ensure full compatibility with Microsoft Office, Adobe, and Windows shell extensions without URL-encoding bugs.

5. **Search Engine:**
   - Prefer `NautilusSearchEngineSimple` (recursive directory search) as a lightweight, robust, and standalone search solution.

6. **Single Instance & IPC:**
   - `GApplication` on Windows natively communicates over Win32 Named Pipes in the absence of a D-Bus session bus. Never require or enforce installing a D-Bus daemon.
