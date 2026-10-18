# 📖 User Guide — Hướng Dẫn Vận Hành & Thiết Lập E-Ra Cloud

Tài liệu này hướng dẫn chi tiết cách biên dịch nạp firmware, thiết lập kết nối đám mây **E-Ra IoT Cloud** và sử dụng giao diện điều khiển 4 nút nhấn trên màn hình LCD1602.

---

## 1. Yêu Cầu Môi Trường Cài Đặt (Prerequisites)

* **Phần cứng:** Cáp dữ liệu Micro-USB nối máy tính với cổng nạp ESP32 DevKit V1.
* **Driver giao tiếp nạp:** Cài đặt driver máy tính tương ứng với chip nạp trên ESP32:
  * [CP210x USB to UART Bridge VCP Drivers](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers) (nếu dùng chip Silicon Labs CP2102)
  * [CH340 Driver](http://www.wch-ic.com/downloads/CH341SER_EXE.html) (nếu dùng chip WCH CH340)
* **Phần mềm:** Visual Studio Code cài tiện ích mở rộng **PlatformIO IDE** (hoặc Arduino IDE 2.x).

---

## 2. Cấu Hình Thông Tin Mạng & Token Đám Mây

Mở tệp mã nguồn [`src/config.h`](../src/config.h) và tiến hành thay đổi các định nghĩa theo thông tin cá nhân của bạn:

```cpp
// ==========================================
// 🌐 CẤU HÌNH CLOUD IOT E-RA & WIFI
// ==========================================
#define DEFAULT_MQTT_HOST "mqtt1.eoh.io"

// Nhập mã Token sinh ra từ tài khoản E-Ra Cloud của bạn
#define ERA_AUTH_TOKEN "YOUR_ERA_AUTH_TOKEN_HERE"

// Nhập tên và mật khẩu mạng Wi-Fi băng tần 2.4 GHz
#define WIFI_SSID "Ten_Wifi_Nha_Ban"
#define WIFI_PASS "Mat_Khau_Wifi"

// ==========================================
// ☀️ THÔNG SỐ TẤM PIN MẶT TRỜI
// ==========================================
// Diện tích bề mặt hữu dụng phần cell quang điện (m²)
// Tấm 20W kích thước cell đo thực tế là 0.155 m²
#define PANEL_AREA_M2 0.155f
```

> [!WARNING]
> * ESP32 chỉ hỗ trợ mạng Wi-Fi băng tần **2.4 GHz**. Không kết nối vào sóng Wi-Fi thuần 5.0 GHz.
> * Tuyệt đối không commit mã Token bí mật và mật khẩu Wi-Fi cá nhân lên các kho lưu trữ GitHub công khai.

---

## 3. Biên Dịch & Nạp Firmware Lên ESP32

Mở terminal trong thư mục dự án và thực hiện các lệnh PlatformIO:

```bash
# 1. Biên dịch kiểm tra mã nguồn (Build)
pio run

# 2. Cắm cáp nạp vào máy tính và nạp chương trình (Upload)
pio run --target upload

# 3. Mở cửa sổ theo dõi log Serial thời gian thực (Baud rate 115200)
pio device monitor
```

Nếu nạp thành công, cổng Serial Monitor sẽ hiển thị nhật ký khởi tạo:
```text
🚀 STARTING SOLAR MONITORING SYSTEM WITH E-RA CLOUD...
[I2C] Scanning devices on SDA:21, SCL:22...
[I2C] Found INA226 Solar at 0x41
[I2C] Found INA226 Battery at 0x40
[I2C] Found BH1750 at 0x23
[I2C] Found LCD1602 at 0x27
[ONE-WIRE] DS18B20 initialized successfully!
[SYSTEM] Initialization Complete!
✅ ERa Cloud Connected Successfully!
```

---

## 4. Hướng Dẫn Thiết Lập Giao Diện E-Ra Cloud (Web & Mobile)

Nền tảng **E-Ra IoT Cloud** (do EoH Ltd phát triển) cung cấp dịch vụ quản lý thiết bị trực tuyến hoàn toàn miễn phí cho các ứng dụng IoT:

1. Truy cập trang quản trị [https://app.eoh.io](https://app.eoh.io) hoặc tải ứng dụng di động **E-Ra** trên Google Play / App Store.
2. Đăng ký tài khoản và đăng nhập.
3. Chọn mục **Devices** $\rightarrow$ Nhấn **Add Device** $\rightarrow$ Chọn loại thiết bị **ESP32** kết nối qua **MQTT**.
4. Hệ thống sẽ tạo một chuỗi ký tự **Auth Token** duy nhất. Sao chép chuỗi này dán vào `#define ERA_AUTH_TOKEN` trong tệp `src/config.h`.
5. Tạo trang bảng điều khiển (**Dashboard**) và kéo các Widget hiển thị tương ứng với các chân ảo (Virtual Pins):

| Tên Widget Đề Xuất | Loại Widget | Ánh Xạ Chân Ảo (V-Pin) | Đơn Vị | Màu Sắc Khuyến Nghị |
|:---|:---:|:---:|:---:|:---:|
| **Điện Áp Solar** | Radial Gauge / Value Card | `V0` | `V` | 🟡 Vàng cam |
| **Dòng Điện Solar** | Value Card | `V1` | `A` | 🟡 Vàng cam |
| **Công Suất Solar** | Number Display / Sparkline | `V2` | `W` | 🟠 Cam đậm |
| **Điện Áp Pin 3S** | Linear Battery Bar | `V3` | `V` | 🟢 Xanh lá |
| **Dòng Sạc/Xả Pin** | Value Card | `V4` | `A` | 🟢 Xanh lá |
| **Công Suất Pin/Tải**| Value Card | `V5` | `W` | 🟢 Xanh lá |
| **Nhiệt Độ Tấm Pin** | Thermometer / Value Card | `V6` | `°C` | 🔴 Đỏ |
| **Bức Xạ Mặt Trời** | Sun Gauge / Line Chart | `V7` | `W/m²` | 🟣 Tím |
| **Hiệu Suất Chuyển Đổi** | Percentage Donut / Line Chart | `V8` | `%` | 🔵 Xanh dương |

| Giao Diện Giám Sát Trên Trình Duyệt Web | Giao Diện Giám Sát Trên App Điện Thoại |
|:---:|:---:|
| ![Web Dashboard](images/dashboard_web.jpg) | ![Mobile App](images/dashboard_mobile.jpg) |

---

## 5. Hướng Dẫn Thao Tác Cụm 4 Phím Bấm Vật Lý & Menu LCD1602

Bo mạch được trang bị 4 phím điều hướng độc lập phục vụ theo dõi tại chỗ khi không có kết nối mạng:

```
    [ NEXT ]         [ PREV ]       [ AUTOSCROLL ]     [ BACKLIGHT ]
    (GPIO 17)        (GPIO 5)         (GPIO 18)          (GPIO 19)
```

1. **Phím NEXT (GPIO 17 - TX2):** Chuyển sang trang màn hình kế tiếp ($1 \rightarrow 2 \rightarrow 3 \rightarrow 4 \rightarrow 5 \rightarrow 1$).
2. **Phím PREV (GPIO 5):** Lùi lại trang màn hình trước đó.
3. **Phím AUTOSCROLL (GPIO 18):** Bật hoặc tắt chế độ tự động chuyển trang theo chu kỳ $5\text{ giây/lần}$. Khi bật, một biểu tượng `*` sẽ xuất hiện ở góc màn hình.
4. **Phím BACKLIGHT (GPIO 19):** Bật hoặc tắt đèn nền màn hình LCD1602. Vào ban đêm hoặc khi chạy bằng pin lưu trữ, tắt đèn nền giúp tiết kiệm đáng kể năng lượng.

### Nội Dung 5 Trang Màn Hình LCD:
* **Trang 1 — Thông số Tấm Pin (Solar Harvest):**
  ```text
  SOLAR: 16.9V 0.74A
  POWER: 12.52 W
  ```
* **Trang 2 — Khối Pin Lưu Trữ (Battery 3S):**
  ```text
  BATT:  12.1V 0.35A
  STATE: CHARGING
  ```
* **Trang 3 — Môi Trường Quang Học & Nhiệt Độ:**
  ```text
  TEMP: 45.2 C
  IRR : 515.0 W/m2
  ```
* **Trang 4 — Hiệu Suất Chuyển Đổi (Efficiency):**
  ```text
  Pin : 79.8W Pout:12.5W
  EFFICIENCY: 15.8%
  ```
* **Trang 5 — Trạng Thái Mạng & Hệ Thống (System Status):**
  ```text
  WiFi: CONNECTED
  ERa : ONLINE [V0-V8]
  ```
