# HƯỚNG DẪN TÁC CHIẾN KỸ THUẬT CHO DỰ ÁN VICTORIA (NAUTILUS TRÊN WINDOWS)

## Mục tiêu
Dự án **Victoria** là phiên bản Windows Native của GNOME Files (Nautilus), mang trải nghiệm giao diện hiện đại GTK4 + Libadwaita về Windows mà không cần máy ảo hay WSL.

## Tư duy lập trình (Mindset)
- "Không có việc gì khó / Chỉ sợ lòng không bền / Đào núi và lấp biển / Quyết chí cũng làm nên".
- Không bao giờ đầu hàng trước lỗi liên quan đến POSIX / Unix APIs; mọi API đều có thể được shim/stub/adapt sang Win32 API.

## Hướng dẫn chuyển đổi mã nguồn (Porting Guidelines)

### 1. File Utilities (`src/nautilus-file-utilities.c`)
- **Autofs**: Trên Windows không có autofs mount point. Thay thế thân hàm `nautilus_location_is_autofs_mountpoint` bằng:
  ```c
  #ifdef G_OS_WIN32
  return FALSE;
  #endif
  ```
- **Root Directory**: Trên Windows, đường dẫn gốc là ký tự ổ đĩa (ví dụ `C:\`). Viết lại `nautilus_is_root_directory`:
  ```c
  #ifdef G_OS_WIN32
  g_autoptr (GFile) parent = g_file_get_parent (dir);
  return parent == NULL;
  #else
  ...
  #endif
  ```

### 2. Main Entry Point (`src/nautilus-main.c`)
- Bỏ kiểm tra `if (getuid () == 0)` vì Windows không có khái niệm `uid 0` root.

### 3. File Properties & Quyền (`src/nautilus-file.c`, `src/nautilus-properties.c`)
- Không gọi `<pwd.h>`, `<grp.h>`, `getpwnam`, `getgrnam`, `getgroups`.
- Điều hướng sang Windows Attributes (`FILE_ATTRIBUTE_READONLY`, `FILE_ATTRIBUTE_HIDDEN`, `FILE_ATTRIBUTE_SYSTEM`).

### 4. Volume & Mount Monitor (`src/nautilus-application.c`)
- Không dùng `gio/gunixmounts.h` trên Windows.
- Dùng `g_volume_monitor_get()` tiêu chuẩn của GIO (được hỗ trợ bởi `GWin32VolumeMonitor`).

### 5. Khởi chạy File (`src/nautilus-mime-actions.c`)
- Ưu tiên `g_app_info_launch ()` (được GLib Win32 ánh xạ sang `ShellExecuteExW`).
