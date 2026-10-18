# 🛠️ Assembly Guide — Hướng Dẫn Gia Công & Lắp Ráp Cơ Điện

Tài liệu này hướng dẫn chi tiết quy trình in 3D các chi tiết cơ khí, hàn mạch điện tử, gia công cơ cấu gá đặt và thứ tự lắp ráp hoàn thiện trạm giám sát **Solar-Smart-IOT**.

---

## 1. Danh Mục Bộ Phận Cơ Khí In 3D (3D Printed Parts)

Tất cả các tệp mô hình 3D nằm trong thư mục [`hardware/mechanical/3d_models/`](../hardware/mechanical/3d_models/):

| Tệp STL / 3MF | Tên Bộ Phận | Kích Thước (DxRxC mm) | Số Lớp Lưới | Vật Liệu Đề Xuất | Vị Trí Lắp Đặt |
|:---|:---|:---:|:---:|:---:|:---|
| `Base_Plate_De_Mach.stl` | **Đế Gá Bo Mạch Chính** | 154.30 x 94.20 x 6.00 mm | 124 | PETG / PLA+ | Đặt đáy khung, đỡ PCB, LCD và cọc vít |
| `Sensor_Housing.stl` | **Hộp Cảm Biến Ngoài Trời** | 60.00 x 65.00 x 22.00 mm | 1,400 | PETG / ABS | Gắn trên khung tấm pin PV |
| `Sensor_Lid.stl` | **Nắp Hộp Cảm Biến** | 39.80 x 44.80 x 19.80 mm | 248 | PETG mờ / trong | Đậy bảo vệ cụm BH1750 |
| `LCD_Standoff_Pillars.stl` | **Trụ Đỡ Màn Hình LCD (x4)** | 6.90 x 6.90 x 12.30 mm | 2,102 | PETG / PLA | 4 góc nâng cao màn hình LCD1602 |
| `Button_Cap_NUTz.stl` | **Nắp Nút Nhấn (x4)** | 4.90 x 4.90 x 5.00 mm | 750 | PLA / Resin | Chụp lên đầu 4 nút nhấn tact switch |

> [!TIP]
> **Khuyến nghị cài đặt phần mềm cắt lớp (Slicer Settings):**
> * **Độ dày lớp in (Layer Height):** `0.20 mm` (cân bằng giữa độ mịn và thời gian in).
> * **Độ điền đầy (Infill Density):** `25% - 35%` theo dạng mẫu **Gyroid** để đảm bảo độ cứng vững cơ học và kháng biến dạng nhiệt ngoài trời.
> * **Số đường viền thành (Perimeters / Wall loops):** `3 - 4 lớp`.
> * **Nhiệt độ in PETG:** Đầu phun `235°C - 240°C`, bàn nhiệt `75°C - 80°C`. Không bật quạt làm mát tối đa (giữ ở 30-50%) để tăng độ liên kết giữa các lớp.

---

## 2. Hình Ảnh Mô Hình Sau Khi Lắp Ráp Hoàn Thiện

| Bo Mạch Điều Khiển Trung Tâm | Trạm Khi Đang Vận Hành & Hiển Thị LCD | Trạm Đo Kết Nối Cảm Biến Ngoài Trời |
|:---:|:---:|:---:|
| ![Bo mạch hoàn chỉnh](images/pcb_assembled.jpg) | ![Hoạt động hiển thị](images/system_in_operation.jpg) | ![Kết nối cảm biến](images/system_with_sensors.jpg) |

---

## 3. Quy Trình Lắp Ráp Từng Bước (Step-by-Step)

### Bước 1: Chuẩn bị & Gia Công Bo Mạch Điện Tử (Protoboard)
1. Sử dụng bo mạch đục lỗ kích thước tiêu chuẩn `9x15 cm`.
2. Hàn 2 hàng header cái (female header) cắm ESP32 DevKit V1 để có thể tháo lắp linh hoạt khi cần nạp lại code hoặc thay thế.
3. Hàn cọc domino vít KF301-2P cho các cổng vào/ra công suất:
   - `SOLAR_IN` (Cổng vào tấm pin mặt trời).
   - `BAT_IN_OUT` (Cổng kết nối pin Lithium 3S).
   - `LOAD_OUT` (Cổng cấp điện cho tải DC).
4. Hàn 4 nút nhấn nhả (Tact switch 6x6mm hoặc 12x12mm) tại vị trí thuận tiện thao tác, một chân nối vào các chân `GPIO 17, 5, 18, 19`, chân đối diện nối chung về đường mass `GND`.
5. Hàn điện trở kéo $4.7\text{ k}\Omega$ giữa đường nguồn $3.3\text{V}$ và chân tín hiệu `GPIO 23` dành cho cảm biến nhiệt độ DS18B20.
6. Hàn các đường bus I2C (SDA, SCL), nguồn 5V, nguồn 3.3V và GND chung.

### Bước 2: Hiệu Chỉnh Mạch Hạ Áp Mini360 (BẮT BUỘC)
1. Cấp nguồn từ 9V - 12.6V vào hai chân `IN+` và `IN-` của module Mini360 (tuyệt đối KHÔNG cắm tải hoặc cắm ESP32 ở bước này).
2. Dùng que đo đồng hồ vạn năng DMM đặt ở thang đo DC Volts, kẹp vào hai chân `OUT+` và `OUT-`.
3. Dùng tuốc-nơ-vít hai cạnh nhỏ xoay biến trở vi chỉnh cho đến khi đồng hồ hiển thị chính xác **$5.00\text{ V}$**.
4. Cố định biến trở bằng một giọt keo nến hoặc sơn cách điện để tránh rung lắc làm sai lệch điện áp trong quá trình vận hành ngoài trời.

### Bước 3: Lắp Ráp Cơ Khí Khung Đế & Màn Hình
1. Đặt tấm đế in 3D `Base_Plate_De_Mach.stl` lên mặt bàn phẳng.
2. Bắt 4 trụ đỡ in 3D `LCD_Standoff_Pillars.stl` vào 4 lỗ định vị bằng ốc vít M3.
3. Đặt màn hình LCD1602 (đã gắn module PCF8574 ở mặt sau) lên 4 trụ đỡ, siết nhẹ ốc M3 để cố định chắc chắn.
4. Gắn bo mạch trung tâm đã hoàn thiện vào khung đế.
5. Chụp 4 nắp phím `Button_Cap_NUTz.stl` lên đầu 4 nút nhấn điều hướng.

### Bước 4: Lắp Đặt Cụm Cảm Biến Môi Trường (Sensor Pod)
1. Cố định module cảm biến ánh sáng BH1750 vào trong hộp in 3D `Sensor_Housing.stl`.
2. Đậy nắp `Sensor_Lid.stl`. Nắp có khe thoáng quang học giúp cảm biến tiếp nhận ánh sáng mặt trời tự nhiên mà không bị đọng nước.
3. Cảm biến nhiệt độ DS18B20 dạng dây chống nước được luồn qua lỗ dẫn dây phía đáy hộp.
4. Gắn toàn bộ hộp cảm biến lên cạnh khung nhôm của tấm pin năng lượng mặt trời, đảm bảo bề mặt nhận sáng của BH1750 nằm **cùng mặt phẳng và cùng góc nghiêng** với tấm pin.
5. Áp sát phần đầu kim loại của đầu dò DS18B20 vào chính giữa mặt lưng tấm pin PV. Dùng keo tản nhiệt (Thermal Paste) và dán cố định bằng băng keo chịu nhiệt Kapton tape để đảm bảo truyền nhiệt chính xác và không bị bong tróc khi tấm pin nóng lên tới 60°C.

### Bước 5: Đấu Nối Toàn Bộ Dây Dẫn Hệ Thống
1. Đấu dây từ cụm cảm biến môi trường về bo mạch chính theo ma trận chân trong [`docs/wiring.md`](wiring.md).
2. Đấu nối hai module INA226 vào đường động lực tấm pin và nhánh pin lưu trữ.
3. Kiểm tra thông mạch bằng thang đo đo thông mạch (Beep test) trên đồng hồ để chắc chắn không có chập mạch giữa `VCC` và `GND`.

---

## 4. Danh Mục Kiểm Tra An Toàn Trước Khi Đóng Điện (Pre-Power Checklist)

Trước khi bật công tắc nguồn hoặc kết nối tấm pin mặt trời ngoài trời, hãy kiểm tra lần lượt:

- [ ] Cực tính của bộ pin lưu trữ 3S đấu nối chính xác vào `BAT+` và `BAT-`.
- [ ] Điện áp ra của mạch Mini360 đã được kiểm chứng bằng DMM đạt mức $5.0\text{V} \pm 0.1\text{V}$.
- [ ] Chân `A0, A1` của hai module INA226 đã được hàn cấu hình đúng địa chỉ (`0x41` cho Solar, `0x40` cho Battery).
- [ ] Điện trở kéo $4.7\text{ k}\Omega$ trên chân DQ của DS18B20 đã được đấu nối.
- [ ] Không có bavia thiếc hàn văng chập giữa các chân GPIO liền kề của ESP32.
- [ ] Cáp I2C giữa bo mạch và màn hình LCD1602 tiếp xúc chắc chắn.
