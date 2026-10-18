# 📋 Bill of Materials (BOM) — Danh Mục Linh Kiện & Chi Phí

Tài liệu này cung cấp bảng kê chi tiết toàn bộ linh kiện điện tử, cảm biến, linh kiện cơ khí chế tạo 3D và vật tư phụ trợ được sử dụng trong hệ thống **IoT Giám Sát Hiệu Suất Pin Năng Lượng Mặt Trời (Solar-Smart-IOT)**.

---

## 1. Bảng Kê Linh Kiện Điện Tử & Cảm Biến

| STT | Tên Linh Kiện | Mã / Quy Cách Kỹ Thuật | Giao Tiếp / Chân Nối | Số Lượng | Đơn Giá Tham Khảo (VNĐ) | Thành Tiền (VNĐ) | Tình Trạng Xác Minh |
|:---:|:---|:---|:---|:---:|:---:|:---:|:---:|
| 1 | **Tấm pin năng lượng mặt trời** | 20W Polycrystalline (Voc: 22V, Isc: 1.2A, Vmp: 17.5V, Imp: 1.14A, kích thước 350x280x17mm) | Cực (+) qua INA226 #1, Cực (-) chung GND | 1 | 250.000 | 250.000 | Đã lắp ráp & đo đạc thực tế |
| 2 | **Bộ điều khiển trung tâm ESP32** | ESP32 DevKit V1 (30 chân, Dual-Core Xtensa LX6 240MHz, 4MB Flash, Wi-Fi 2.4GHz + BLE) | I2C (GPIO21/22), 1-Wire (GPIO23), Nút (GPIO17, 5, 18, 19) | 1 | 95.000 | 95.000 | Đã kiểm thử firmware FreeRTOS |
| 3 | **Cảm biến dòng & áp Solar** | Module INA226 (I2C address: `0x41`, Shunt 0.100Ω R100/R010, ADC 16-bit) | I2C (SDA: D21, SCL: D22), mắc nối tiếp nguồn Solar | 1 | 45.000 | 45.000 | Đã hiệu chuẩn & đo kiểm |
| 4 | **Cảm biến dòng & áp Battery/Load** | Module INA226 (I2C address: `0x40`, Shunt 0.100Ω R100/R010, ADC 16-bit) | I2C (SDA: D21, SCL: D22), mắc nối tiếp nhánh ắc quy & tải | 1 | 45.000 | 45.000 | Đã hiệu chuẩn & đo kiểm |
| 5 | **Cảm biến cường độ ánh sáng** | Module BH1750FVI (I2C address: `0x23`, dải 1 - 65535 lux) | I2C (SDA: D21, SCL: D22), VCC 3.3V | 1 | 35.000 | 35.000 | Đã lắp ráp hộp cảm biến ngoài |
| 6 | **Cảm biến nhiệt độ bề mặt** | Cảm biến DS18B20 dạng dây chống nước (Vỏ inox, dải -55°C đến +125°C) | 1-Wire (DQ: GPIO23, kèm trở kéo 4.7kΩ lên 3.3V) | 1 | 35.000 | 35.000 | Đã gắn mặt sau tấm pin PV |
| 7 | **Màn hình hiển thị LCD** | LCD 1602 kèm mạch chuyển đổi I2C PCF8574 (Địa chỉ I2C: `0x27`) | I2C (SDA: D21, SCL: D22), VCC 5V | 1 | 45.000 | 45.000 | Đã gắn khung đỡ 3D |
| 8 | **Mạch sạc năng lượng mặt trời** | Module sạc MPPT Solar pin Lithium (hỗ trợ MPPT cho pin 3S) | IN từ Solar, OUT cấp ắc quy / pin lưu trữ | 1 | 85.000 | 85.000 | Đã tích hợp trên bo mạch chính |
| 9 | **Khối pin lưu trữ (Ắc quy)** | Bộ pin Lithium-ion 3S 18650 (11.1V danh định, 12.6V đầy) tích hợp BMS | Cấp điện tải và qua Mini360 nuôi mạch | 1 | 150.000 | 150.000 | Đã đo đạc thực nghiệm |
| 10 | **Mạch hạ áp nguồn nuôi** | Module DC-DC Step-Down Mini360 (MP1584EN, hiệu suất 92%, hạ về 5V) | IN: Pin lưu trữ (9-12.6V), OUT: 5.0V ổn định | 1 | 15.000 | 15.000 | Đã chỉnh áp 5.0V trước khi cấp |
| 11 | **Nút nhấn điều hướng menu** | Nút nhấn nhả 4 chân (Tact switch 6x6mm hoặc 12x12mm) | Nối GPIO17, 5, 18, 19 xuống GND (INPUT_PULLUP) | 4 | 2.500 | 10.000 | Đã lắp ráp kèm nắp chụp 3D |
| 12 | **Tải tiêu thụ thử nghiệm** | Quạt tản nhiệt DC 5V cổng USB (hoặc bóng đèn thử tải DC) | Nối đầu ra tải qua INA226 #2 | 1 | 30.000 | 30.000 | Tải thử nghiệm thực tế |
| 13 | **Bo mạch gắn linh kiện (PCB)** | Bo mạch đục lỗ thử nghiệm (Protoboard 9x15cm hoặc tự thiết kế) | Hàn kết nối toàn bộ module | 1 | 25.000 | 25.000 | Đã gia công & hàn hoàn thiện |
| 14 | **Linh kiện phụ trợ** | Trở kéo 4.7kΩ, tụ lọc nguồn (100uF, 100nF), cọc domino vít KF301, dây bus | Mạch phân phối nguồn & bảo vệ chống nhiễu | 1 bộ | 30.000 | 30.000 | Đã lắp ráp hoàn thiện |
| **TỔNG CỘNG ĐIỆN TỬ** | | | | | | **800.000 VNĐ** | |

---

## 2. Bảng Kê Bộ Phận Cơ Khí Chế Tạo (In 3D FDM)

Toàn bộ các chi tiết cơ khí được thiết kế trên Autodesk Fusion 360 và in 3D bằng vật liệu PETG/PLA:

| STT | Bộ Phận Cơ Khí | File Nguồn / File STL | Kích Thước Bao (DxRxC mm) | Số Lớp Tam Giác | Vật Liệu Khuyến Nghị | Khối Lượng Ước Tính | Vai Trò |
|:---:|:---|:---|:---:|:---:|:---:|:---:|:---|
| 1 | **Đế gá mạch trung tâm** | `Base_Plate_De_Mach.stl` | 154.30 x 94.20 x 6.00 mm | 124 | PETG / PLA+ | ~45g | Đế cố định bo mạch chính, ESP32, màn hình và cọc đấu dây |
| 2 | **Hộp cảm biến ngoài trời** | `Sensor_Housing.stl` | 60.00 x 65.00 x 22.00 mm | 1,400 | PETG / ABS | ~35g | Hộp bảo vệ cảm biến ánh sáng BH1750 và đầu dò nhiệt độ gắn cạnh tấm pin |
| 3 | **Nắp đậy hộp cảm biến** | `Sensor_Lid.stl` | 39.80 x 44.80 x 19.80 mm | 248 | PETG trong / mờ | ~15g | Nắp đậy bảo vệ có khe hở ánh sáng cho BH1750 |
| 4 | **Trụ đỡ màn hình LCD** | `LCD_Standoff_Pillars.stl` | 6.90 x 6.90 x 12.30 mm | 2,102 | PETG / PLA | ~5g (4 trụ) | Trụ đệm tạo khoảng cách tản nhiệt và nâng cao LCD1602 |
| 5 | **Nắp chụp nút bấm** | `Button_Cap_NUTz.stl` | 4.90 x 4.90 x 5.00 mm | 750 | PLA / Resin | ~2g (4 nút) | Nắp chụp đầu nút nhấn tạo cảm giác bấm êm ái |
| 6 | **CAD Gốc Tham Số** | `Sensor_Housing_Parametric.f3d` | N/A | N/A | Autodesk Fusion 360 | N/A | File thiết kế tham số gốc phục vụ tùy chỉnh kích thước |

---

## 3. Tổng Hợp Chi Phí Dự Án

* **Chi phí phần cứng điện tử & cảm biến:** ~800.000 VNĐ (~$32 USD)
* **Chi phí tấm pin quang điện & ắc quy:** ~400.000 VNĐ (~$16 USD)
* **Chi phí in 3D vật liệu PETG (~105g nhựa):** ~50.000 VNĐ (~$2 USD)
* **Tổng ngân sách hoàn thiện 01 mô hình:** **~1.250.000 VNĐ** (~$50 USD)

> [!NOTE]
> Ngân sách này nằm trong khung chi phí đồ án môn học đại học (< 1.500.000 VNĐ), cho phép sinh viên, kỹ sư IoT và cộng đồng Maker dễ dàng tái lập mô hình với chi phí tối thiểu.
