# Trình Quản Lý Tập Tin Victoria (Victoria Files Manager)

[![Platform](https://img.shields.io/badge/Platform-Windows%2010%20%7C%2011-0078D6?logo=windows&logoColor=white)](https://microsoft.com/windows)
[![UI Toolkit](https://img.shields.io/badge/UI-GTK4%20%2B%20Libadwaita-3584E4?logo=gnome&logoColor=white)](https://gnome.org)
[![Build](https://img.shields.io/badge/Build-MSYS2%20UCRT64%20%7C%20Meson%20%7C%20Ninja-blue)](https://msys2.org)
[![License](https://img.shields.io/badge/License-GPL--3.0--or--later-green.svg)](LICENSE)

**Tiếng Việt** | [**🇬🇧 English**](README.md)

---

**Victoria** là bản port nguyên bản (native port) của **GNOME Files (Nautilus 47)** dành cho Windows, mang trải nghiệm duyệt tập tin hiện đại, thanh lịch của môi trường GNOME lên Windows 10 và Windows 11 mà không cần máy ảo, WSL, hay X11/Wayland server.

> *"Không có việc gì khó / Chỉ sợ lòng không bền / Đào núi và lấp biển / Quyết chí cũng làm nên"* — Chủ tịch Hồ Chí Minh.

---

## ✨ Tính Năng Nổi Bật

- 🖥️ **Ứng dụng Windows nguyên bản (`victoria.exe`)**: Biên dịch và chạy trực tiếp trên Windows x86_64, tận dụng Win32 GDK backend với khả năng tăng tốc phần cứng DirectX / WGL.
- 🎨 **Giao diện GTK4 & Libadwaita**: Phong cách thiết kế GNOME hiện đại, mượt mà, hỗ trợ tự động nhận diện giao diện Sáng/Tối (Light/Dark mode) theo Windows Registry (`AppsUseLightTheme`).
- 💾 **Nhận diện đầy đủ Ổ đĩa & Phân vùng Windows**: Tự động liệt kê các ký tự ổ đĩa (`C:\`, `D:\`,...) cùng tên phân vùng trên thanh bên (sidebar) qua `GetLogicalDriveStringsW` và `GWin32VolumeMonitor`.
- 🗑️ **Tích hợp Thùng rác Windows (Recycle Bin)**: Đồng bộ trực tiếp với Thùng rác của Windows (`SHQueryRecycleBinW`, `SHEmptyRecycleBinW`, `g_file_trash`), cập nhật biểu tượng rác đầy/trống tức thì và hỗ trợ dọn sạch thùng rác từ menu ngữ cảnh.
- ⚡ **Khởi chạy ứng dụng qua Windows Shell**: Khởi chạy tập tin bằng API `ShellExecuteExW` chuẩn mực của Windows, tương thích 100% với Microsoft Office (Word, PowerPoint, Excel), Adobe Creative Cloud, các trình phát đa phương tiện và shell extension của Windows.
- 🔍 **Tìm kiếm đệ quy độc lập**: Sử dụng bộ máy tìm kiếm `NautilusSearchEngineSimple` gọn nhẹ, tốc độ cao mà không phụ thuộc vào các dịch vụ nền Linux.
- 🪶 **Không cần dịch vụ nền Linux**: Quản lý đơn phiên (single instance) qua Win32 Named Pipes, hoàn toàn không phụ thuộc D-Bus session bus hay GVfs daemon.

---

## 🚀 Hướng Dẫn Cài Đặt & Biên Dịch

### Yêu Cầu Môi Trường

Bạn cần cài đặt môi trường [MSYS2](https://www.msys2.org/) (môi trường **UCRT64**).

Mở terminal **MSYS2 UCRT64** và cài đặt các gói phụ thuộc:

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

### Biên Dịch Victoria

Mở PowerShell tại thư mục gốc dự án:

```powershell
# Nạp công cụ MSYS2 vào PATH (nếu chạy từ PowerShell)
$env:PATH = "C:\msys64\ucrt64\bin;C:\msys64\usr\bin;$env:PATH"

# Cấu hình thư mục build
meson setup build --buildtype=release -Dtests=none -Ddocs=false

# Tiến hành biên dịch
ninja -C build
```

### Khởi Chạy Victoria

Bạn có thể khởi chạy Victoria nhanh chóng bằng các script có sẵn:

**Bằng PowerShell:**
```powershell
.\run-victoria.ps1
```

**Bằng Command Prompt (CMD / Batch):**
```cmd
run-victoria.bat
```

Hoặc chạy trực tiếp file thực thi với biến môi trường phù hợp:
```powershell
$env:PATH = "D:\Projects\victoria\build\libnautilus-extension;D:\Projects\victoria\build\subprojects\gnome-autoar\gnome-autoar;C:\msys64\ucrt64\bin;C:\msys64\usr\bin;$env:PATH"
$env:GSETTINGS_SCHEMA_DIR = "D:\Projects\victoria\build\schemas"
$env:GDK_BACKEND = "win32"
.\build\src\victoria.exe
```

---

## 🏛️ So Sánh Kiến Trúc & Giải Pháp Cầu Nối

| Thành Phần | Linux (Nautilus Gốc) | Windows (Victoria) |
|---|---|---|
| **File thực thi** | `nautilus` (ELF) | `victoria.exe` (PE32+ / Win32) |
| **Application ID** | `org.gnome.Nautilus` | `org.victoria.Victoria` |
| **GSettings Schema** | `org.gnome.nautilus` | `org.victoria` |
| **Ổ đĩa & Phân vùng** | UDisks2 / GVfs | `GetLogicalDriveStringsW` + `GWin32VolumeMonitor` |
| **Thùng rác** | `trash:///` (GVfs) | Win32 Recycle Bin (`SHEmptyRecycleBinW`, `SHQueryRecycleBinW`) |
| **Khởi chạy ứng dụng** | `GDesktopAppInfo` + `.desktop` | `ShellExecuteExW` + Windows Registry (`HKCR`) |
| **IPC / Đơn phiên** | D-Bus Session Bus | Win32 Named Pipes (`GApplication`) |
| **Đồng bộ Theme** | `org.gnome.desktop.interface` | Windows Registry (`AppsUseLightTheme`) |

---

## 📜 Giấy Phép & Nguồn Gốc

Victoria được phát triển dựa trên [GNOME Files (Nautilus)](https://gitlab.gnome.org/GNOME/nautilus) bởi GNOME Foundation và cộng đồng đóng góp.

Phát hành theo giấy phép **GNU General Public License v3.0 hoặc mới hơn** (GPL-3.0-or-later).
