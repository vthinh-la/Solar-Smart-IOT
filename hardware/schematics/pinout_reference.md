# 📌 Pinout & Electrical Reference — Ma Trận Kết Nối Điện Tử

Tài liệu này đóng vai trò là bảng tra cứu nhanh (Quick Reference) toàn bộ chân kết nối, địa chỉ bus và mức điện áp của trạm **Solar-Smart-IOT**.

---

## 1. Bản Đồ Chân Vi Điều Khiển ESP32 DevKit V1

```
                         +-------------------+
                         |    ESP32 DEVKIT   |
                         |                   |
                     3V3 | [ ]           [ ] | GND -------- Common Ground
                      EN | [ ]           [ ] | GPIO 23 ---- DS18B20 1-Wire (DQ)
       (ADC) GPIO 36/VP  | [ ]           [ ] | GPIO 22 ---- I2C SCL (Shared Bus)
       (ADC) GPIO 39/VN  | [ ]           [ ] | GPIO 1  ---- TX0 (Debug Serial)
       (ADC) GPIO 34     | [ ]           [ ] | GPIO 3  ---- RX0 (Debug Serial)
       (ADC) GPIO 35     | [ ]           [ ] | GPIO 21 ---- I2C SDA (Shared Bus)
             GPIO 32     | [ ]           [ ] | GPIO 19 ---- BTN 4 (Backlight Toggle)
             GPIO 33     | [ ]           [ ] | GPIO 18 ---- BTN 3 (Autoscroll Toggle)
             GPIO 25     | [ ]           [ ] | GPIO 5  ---- BTN 2 (Previous Page)
             GPIO 26     | [ ]           [ ] | GPIO 17 ---- BTN 1 (Next Page)
             GPIO 27     | [ ]           [ ] | GPIO 16     (Reserved)
             GPIO 14     | [ ]           [ ] | GPIO 4      (Reserved)
             GPIO 12     | [ ]           [ ] | GPIO 0      (Boot Mode)
             GPIO 13     | [ ]           [ ] | GPIO 2      (Onboard LED)
                     GND | [ ]           [ ] | GPIO 15     (Reserved)
                     VIN | [ ]           [ ] | 5V --------- 5.0V DC Input (From Mini360)
                         +-------------------+
```

---

## 2. Thiết Lập Địa Chỉ Phần Cứng Bus I2C

| Module Ngoại Vi | Địa Chỉ Mặc Định | Địa Chỉ Được Thiết Lập | Chân Cấu Hình Phần Cứng | Mục Đích Sử Dụng |
|:---|:---:|:---:|:---:|:---|
| **INA226 (Kênh Solar)** | `0x40` | **`0x41`** | `A0` nối `VCC`, `A1` nối `GND` | Đo điện áp bus và dòng điện phát từ tấm pin năng lượng mặt trời |
| **INA226 (Kênh Battery)** | `0x40` | **`0x40`** | `A0` nối `GND`, `A1` nối `GND` | Đo điện áp khối pin lưu trữ 3S và dòng nạp/xả đến tải |
| **BH1750 (Độ Rọi)** | `0x23` | **`0x23`** | `ADDR` nối `GND` (hoặc để hở) | Thu thập cường độ ánh sáng quang học môi trường (Lux) |
| **LCD 1602 (PCF8574)** | `0x27` / `0x3F`| **`0x27`** | Chân A0/A1/A2 để hở trên mạch backpack | Hiển thị 5 trang thông số đo đạc tại chỗ |

---

## 3. Bản Vẽ Sơ Đồ Khối & Đấu Nối

* **Sơ đồ khối tổng thể:** [system_block_diagram.png](system_block_diagram.png)
* **Sơ đồ đấu nối nguyên lý chi tiết:** [wiring_schematic.png](wiring_schematic.png)
