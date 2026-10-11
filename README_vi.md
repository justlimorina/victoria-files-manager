# Victoria Files Manager

[![Nền tảng](https://img.shields.io/badge/nền%20tảng-Windows%2010%20%7C%2011-0078D6?logo=windows&logoColor=white)](https://www.microsoft.com/windows)
[![Giao diện](https://img.shields.io/badge/giao%20diện-GTK4%20%2B%20Libadwaita-3584E4?logo=gnome&logoColor=white)](https://www.gtk.org/)
[![Build](https://img.shields.io/badge/build-MSYS2%20UCRT64%20%7C%20Meson%20%7C%20Ninja-4C8BF5)](https://www.msys2.org/)
[![Giấy phép](https://img.shields.io/badge/giấy%20phép-GPL--3.0--or--later-2E7D32)](LICENSE)

[English](README.md) · [Kho mã GitHub](https://github.com/justlimorina/victoria-files-manager)

Victoria là bản port hướng đến Windows của [GNOME Files (Nautilus)](https://gitlab.gnome.org/GNOME/nautilus). Dự án giữ lại trải nghiệm quản lý tệp bằng GTK4 và Libadwaita, đồng thời thay các phần tích hợp Linux quan trọng bằng cơ chế native của Windows. Victoria nhắm đến Windows 10/11 64-bit và tạo ra tệp thực thi `victoria.exe`.

## Điểm nổi bật

- Giao diện GTK4 và Libadwaita; nhận diện chế độ sáng/tối theo thiết lập Windows.
- Duyệt thư mục bằng chế độ lưới hoặc danh sách; hỗ trợ tab, sidebar, bookmark và tìm kiếm.
- Hiển thị ổ đĩa, phân vùng và volume Windows trong sidebar, bao gồm các ổ theo ký tự như `C:` và `D:`.
- Mở tệp bằng liên kết ứng dụng và Windows Shell gốc.
- Tích hợp Recycle Bin: đưa tệp vào thùng rác, theo dõi trạng thái đầy/trống, dọn sạch, và mở cửa sổ Recycle Bin của Victoria để xem, khôi phục hoặc xóa vĩnh viễn mục đã chọn.
- Hoạt động đơn phiên qua `GApplication` trên Windows; không cần WSL, X11 server hay D-Bus session daemon.

## Trạng thái dự án

Victoria đang được phát triển như một bản port cho Windows, không phải bản phát hành chính thức của GNOME. Mã nguồn vẫn kế thừa phần lớn từ Nautilus, vì vậy một số tính năng thiên về Linux có thể chưa hoàn chỉnh hoặc hoạt động khác trên Windows. Khi báo lỗi, vui lòng ghi rõ phiên bản Windows, lệnh build và các bước tái hiện trong [Issue tracker](https://github.com/justlimorina/victoria-files-manager/issues).

## Build từ mã nguồn

### Yêu cầu

- Windows 10 hoặc Windows 11, bản 64-bit
- [MSYS2](https://www.msys2.org/) với môi trường **UCRT64**
- Git để clone mã nguồn

Trước hết cập nhật MSYS2. Sau đó mở terminal **MSYS2 UCRT64** và cài các gói phát triển:

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

Clone và build dự án:

```bash
git clone https://github.com/justlimorina/victoria-files-manager.git
cd victoria-files-manager
meson setup build --buildtype=release -Dtests=none
meson compile -C build
```

Tệp thực thi được tạo tại `build/src/victoria.exe`.

### Build từ PowerShell

Nếu làm việc từ PowerShell thay vì terminal MSYS2 UCRT64, dùng lệnh sau:

```powershell
$env:MSYSTEM = 'UCRT64'
$env:CHERE_INVOKING = 'yes'
& C:\msys64\usr\bin\bash.exe -lc 'cd /d/Projects/victoria && ninja -C build'
```

Thay `/d/Projects/victoria` bằng đường dẫn checkout của bạn. Hãy đóng Victoria trước khi build lại: Windows sẽ khóa `victoria.exe` đang chạy nên linker không thể ghi đè tệp đó.

### Cấu hình lại hoặc build sạch

```bash
meson setup build --reconfigure -Dtests=none
meson compile -C build
```

Để tạo lại hoàn toàn thư mục build:

```bash
meson setup build --wipe --buildtype=release -Dtests=none
meson compile -C build
```

## Chạy Victoria

Các script đi kèm đã thiết lập DLL và GSettings cần thiết cho bản build trong cây mã nguồn:

```powershell
.\run-victoria.ps1
```

```cmd
run-victoria.bat
```

Có thể truyền thư mục cần mở ở cuối lệnh, ví dụ:

```powershell
.\run-victoria.ps1 D:\Projects
```

## Tích hợp Windows

| Hạng mục | Cách Victoria triển khai |
| --- | --- |
| File thực thi | `victoria.exe` trên Win32 GTK backend |
| Application ID | `org.victoria.Victoria` |
| Ổ đĩa và volume | `GWin32VolumeMonitor` và Win32 drive APIs |
| Liên kết ứng dụng | Windows Shell và file association đã đăng ký |
| Recycle Bin | Windows Shell, `SHQueryRecycleBinW`, `SHEmptyRecycleBinW` và thao tác thùng rác native |
| Giao diện sáng/tối | Registry `AppsUseLightTheme` của Windows |
| Đơn phiên | Windows IPC của `GApplication` |

## Đóng góp

Hãy fork repository, tạo một nhánh có phạm vi rõ ràng rồi gửi pull request. Với thay đổi dành riêng cho Windows, cần dùng platform guard phù hợp, giữ attribution của mã nguồn upstream, và mô tả cách đã kiểm tra build hoặc runtime trong pull request.

## Ghi công và giấy phép

Victoria được phát triển dựa trên GNOME Files (Nautilus), sản phẩm của GNOME Foundation và cộng đồng đóng góp. Phần thay đổi dành cho Victoria được duy trì tại [repository Victoria](https://github.com/justlimorina/victoria-files-manager).

Dự án được phát hành theo [GNU General Public License, phiên bản 3 hoặc mới hơn](LICENSE).
