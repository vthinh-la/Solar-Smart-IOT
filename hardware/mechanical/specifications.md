# 📐 Mechanical Specifications & 3D Fabrication — Bản Vẽ & Kích Thước Chế Tạo Cơ Khí

Tài liệu này cung cấp bảng kích thước hình học chính xác được trích xuất trực tiếp từ các file lưới tam giác 3D (STL/3MF) và file CAD tham số gốc Autodesk Fusion 360 của hệ thống **Solar-Smart-IOT**.

---

## 1. Bảng Thông Số Kích Thước Hình Học Chi Tiết (STL Geometric Data)

| Tên Bộ Phận / Tệp File | Kích Thước Bao Phủ Ngoài (X × Y × Z mm) | Số Lượng Lưới Tam Giác | Dung Sai Khuyến Nghị | Khối Lượng Ước Tính (Infill 25%) | Mô Tả & Chức Năng |
|:---|:---:|:---:|:---:|:---:|:---|
| **`Base_Plate_De_Mach.stl`** | **154.30 × 94.20 × 6.00 mm** | 124 tam giác | $\pm 0.2\text{ mm}$ | ~45 gram | Đế gá đỡ toàn bộ bo mạch điều khiển trung tâm, module Mini360, các cọc vít domino và cụm 4 phím bấm. |
| **`Sensor_Housing.stl`** | **60.00 × 65.00 × 22.00 mm** | 1,400 tam giác | $\pm 0.15\text{ mm}$ | ~35 gram | Hộp gá lắp cảm biến cường độ ánh sáng BH1750 và lối dẫn dây cảm biến nhiệt độ DS18B20 gắn cố định trên khung tấm pin. |
| **`Sensor_Lid.stl`** | **39.80 × 44.80 × 19.80 mm** | 248 tam giác | $\pm 0.15\text{ mm}$ | ~15 gram | Nắp bảo vệ chống bụi và nước hắt cho hộp cảm biến ngoài trời, có rãnh thoát quang học cho BH1750. |
| **`LCD_Standoff_Pillars.stl`** | **6.90 × 6.90 × 12.30 mm** | 2,102 tam giác | $\pm 0.1\text{ mm}$ | ~5 gram (bộ 4 trụ) | Trụ đệm ren M3 nâng cao màn hình LCD1602 tạo khoảng cách thoáng khí tản nhiệt cho bo mạch bên dưới. |
| **`Button_Cap_NUTz.stl`** | **4.90 × 4.90 × 5.00 mm** | 750 tam giác | $\pm 0.05\text{ mm}$ | ~2 gram (bộ 4 nút) | Nắp chụp đầu nút nhấn nhả (Tact switch), tăng diện tích tiếp xúc ngón tay khi thao tác bấm menu. |

---

## 2. File Nguồn Thiết Kế CAD Tham Số (Parametric CAD Source)

* **Tệp:** [`hardware/mechanical/cad_source/Sensor_Housing_Parametric.f3d`](cad_source/Sensor_Housing_Parametric.f3d)
* **Phần mềm tương thích:** Autodesk Fusion 360 (hoặc Autodesk Inventor).
* **Tính năng:** Toàn bộ lịch sử thiết kế (Timeline), phác thảo (Sketches), ràng buộc kích thước (Constraints) và thông số tham số hóa (User Parameters) được lưu trữ đầy đủ, cho phép người dùng tùy chỉnh kích thước ngàm kẹp theo các loại khung nhôm tấm pin mặt trời khác nhau.

---

## 3. Quy Chuẩn & Khuyến Nghị In 3D (3D Printing Best Practices)

Do thiết bị được bố trí vận hành ngoài trời dưới ánh nắng trực tiếp, nhiệt độ vỏ hộp có thể lên tới $55^\circ\text{C} - 65^\circ\text{C}$:

* **Lựa chọn vật liệu:**
  * ✅ **Khuyến nghị số 1: Nhựa PETG (Polyethylene Terephthalate Glycol):** Kháng tia cực tím (UV resistance), nhiệt độ biến dạng nhiệt (HDT) $\approx 75^\circ\text{C}$, không bị giòn gãy dưới thời tiết mưa nắng.
  * ✅ **Lựa chọn số 2: Nhựa ABS / ASA:** Rất bền với môi trường ngoài trời, cần máy in có buồng kín để tránh cong vênh.
  * ❌ **Không khuyến nghị: Nhựa PLA tiêu chuẩn:** PLA bị mềm và biến dạng ở nhiệt độ trên $50^\circ\text{C}$ khi phơi nắng trực tiếp.
* **Hướng đặt chi tiết trên bàn in (Print Orientation):**
  * `Base_Plate_De_Mach`: Đặt mặt phẳng lớn nhất tiếp xúc trực tiếp mặt bàn nhiệt. Không cần bật Support.
  * `Sensor_Housing`: Đặt mặt đáy tiếp xúc bàn in, bật Support dạng Tree (Cây) cho các lỗ gá ốc bên hông.
  * `Sensor_Lid`: In bằng nhựa trong suốt hoặc nhựa mờ (Translucent/Clear) để ánh sáng tự nhiên truyền qua cảm biến BH1750 tốt nhất.
