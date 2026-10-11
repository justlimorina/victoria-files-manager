# DỰ ÁN VICTORIA: GNOME FILES (NAUTILUS) CHO WINDOWS

> *"Không có việc gì khó / Chỉ sợ lòng không bền / Đào núi và lấp biển / Quyết chí cũng làm nên"* — Chủ tịch Hồ Chí Minh.

---

## 1. NGUYÊN TẮC BẤT DI BẤT DỊCH CHO AI (CORE RULES)

1. **Tuyệt đối không kêu khó hay bàn lùi:**
   - Cấm viện cớ "Nautilus gắn quá chặt với Linux/GNOME", "quá phức tạp", hay "không thể thực hiện".
   - Cấm đề xuất người dùng từ bỏ dự án để dùng File Explorer hoặc phần mềm khác.
   - Khi gặp lỗi biên dịch hoặc lỗi hệ thống, nhiệm vụ duy nhất của AI là: **Phân tích nguyên nhân gốc rễ $\rightarrow$ Viết bản vá (patch) hoặc cầu nối (bridge/adapter) $\rightarrow$ Tiếp tục hoàn thiện.**

2. **Định danh dự án (Identity & Rebranding):**
   - Tên dự án: **Victoria**
   - Tên file thực thi: `victoria.exe`
   - Application ID: `org.victoria.Victoria`
   - GSettings Schema ID: `org.victoria` (thay thế cho `org.gnome.nautilus`)
   - Tên hiển thị người dùng: **Victoria** (hoặc *Victoria Files*)

---

## 2. KIẾN TRÚC & HẠ TẦNG KỸ THUẬT

- **Hệ điều hành đích:** Windows 10 / Windows 11 (x86_64).
- **Môi trường Toolchain:** MSYS2 UCRT64 (`C:\msys64\ucrt64\bin`).
- **Compiler & Build System:** GCC (UCRT), Meson (`meson setup`), Ninja (`ninja`).
- **Giao diện đồ họa (UI Toolkit):**
  - **GTK4** (`GDK_BACKEND=win32`) + **Libadwaita 1.x**.
  - Render qua Win32 GDK backend (tăng tốc phần cứng DirectX / WGL).
  - Tự động nhận diện theme Sáng/Tối theo Windows Registry (`AppsUseLightTheme`).

---

## 3. CÁC QUY TẮC THIẾT KẾ & XỬ LÝ KHÁC BIỆT WINDOWS - LINUX

1. **Xử lý Đường dẫn & Ổ đĩa (Paths & Drive Letters):**
   - Linux chỉ có 1 gốc `/`. Windows có nhiều gốc theo ký tự ổ đĩa (`C:\`, `D:\`) và đường dẫn mạng UNC (`\\server\share`).
   - Mọi hàm kiểm tra gốc (`is_root_directory`): Trên Windows, một thư mục là gốc khi `g_file_get_parent(dir) == NULL` hoặc là Drive root (`[A-Za-z]:\`).
   - Tích hợp `GWin32VolumeMonitor` và `GetLogicalDriveStringsW` để hiển thị các ổ đĩa trên thanh bên (sidebar).

2. **Phân quyền & Thuộc tính Tập tin (Permissions & Attributes):**
   - Vô hiệu hoá các lời gọi POSIX không tồn tại trên Win32: `<pwd.h>`, `<grp.h>`, `chmod`, `chown`, `getpwnam`, `getgrnam`.
   - Ánh xạ sang các thuộc tính chuẩn của Windows: **Read-only**, **Hidden**, **System**, **Archive** thông qua `GetFileAttributesW` / `SetFileAttributesW`.

3. **Thùng rác (Recycle Bin):**
   - Không phụ thuộc vào daemon GVfs `trash:///`.
   - Sử dụng cơ chế thùng rác của Win32: `g_file_trash()` (gọi ngầm `SHFileOperationW` / `IFileOperation`).

4. **Khởi chạy ứng dụng mặc định (File Associations):**
   - Thay thế việc tìm file `.desktop` và `GDesktopAppInfo` bằng `GWin32AppInfo` kết hợp Windows Registry (`HKCR`) và `ShellExecuteExW`.

5. **Bộ máy tìm kiếm (Search Engine):**
   - Ưu tiên sử dụng `NautilusSearchEngineSimple` (tìm kiếm thư mục đệ quy) làm giải pháp tiêu chuẩn nhẹ nhàng, ổn định, độc lập.

6. **Đơn phiên & Tiến trình (Single Instance / IPC):**
   - `GApplication` trên Windows tự động hoạt động qua Win32 Named Pipes khi không có D-Bus session bus. Tuyệt đối không ép buộc cài đặt D-Bus daemon.
