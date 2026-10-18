#pragma once

#include <Arduino.h>

// ==========================================
// 🔌 SƠ ĐỒ CHÂN (PINOUT MATRIX)
// ==========================================
// I2C Bus Pins
#define PIN_I2C_SDA 21
#define PIN_I2C_SCL 22

// 1-Wire Pin for DS18B20
#define PIN_ONE_WIRE 23

// 4 Nút nhấn vật lý (Active-LOW với INPUT_PULLUP)
#define PIN_BTN_NEXT 17       // TX2: Trang tiếp theo
#define PIN_BTN_PREV 5        // D5: Trang trước đó
#define PIN_BTN_AUTOSCROLL 18 // D18: Bật/Tắt tự động chuyển trang
#define PIN_BTN_BACKLIGHT 19  // D19: Bật/Tắt đèn nền LCD

// ==========================================
// 🧭 ĐỊA CHỈ I2C (I2C ADDRESSES)
// ==========================================
#define I2C_ADDR_INA226_SOLAR 0x41 // Kênh 1: Tấm pin mặt trời vào MPPT
#define I2C_ADDR_INA226_BATT 0x40  // Kênh 2: Khối pin lưu trữ & Tải xả
#define I2C_ADDR_BH1750 0x23       // Cảm biến độ rọi BH1750
#define I2C_ADDR_LCD1602 0x27      // Màn hình LCD 1602 I2C (PCF8574)

// ==========================================
// ⚙️ THÔNG SỐ CẢM BIẾN & CALIBRATION
// ==========================================
// Điện trở shunt cho module INA226 (Ohm)
#define INA226_SHUNT_RESISTANCE_OHM 0.1f // 100 mOhm (mặc định module phổ biến)

// Dòng điện tối đa dự kiến qua INA226 để calibrate (Amps)
#define INA226_MAX_CURRENT_AMPS 3.2768f

// Hệ số chuyển đổi Quang thông mặt trời: Lux sang Bức xạ W/m²
// 1 W/m² bức xạ mặt trời xấp xỉ 120 Lux (hoặc 1 Lux ≈ 0.0079 W/m² = 1/126.8
// W/m²)
#define LUX_TO_WM2_DIVISOR 120.0f

// ==========================================
// ☀️ THÔNG SỐ TẤM PIN MẶT TRỜI (SOLAR PANEL)
// ==========================================
// Diện tích hữu dụng bề mặt tấm pin (đơn vị: mét vuông - m²)
// NGƯỜI DÙNG CẦN ĐO VÀ ĐIỀU CHỈNH GIÁ TRỊ NÀY THEO TẤM PIN THỰC TẾ
#define PANEL_AREA_M2 0.155f

// Ngưỡng bức xạ tối thiểu để tính toán hiệu suất (tránh chia cho số quá nhỏ khi
// trời tối)
#define MIN_IRRADIANCE_WM2_FOR_EFF 10.0f

// ==========================================
// ⏱️ THỜI GIAN ĐIỀU KHIỂN & CHU KỲ (TIMINGS)
// ==========================================
#define INTERVAL_SENSOR_READ_MS 1000  // Chu kỳ đọc cảm biến: 1 giây
#define INTERVAL_LCD_UPDATE_MS 500    // Chu kỳ cập nhật hiển thị LCD: 500 ms
#define INTERVAL_SERIAL_DEBUG_MS 2000 // Chu kỳ in thông tin Serial: 2 giây
#define INTERVAL_AUTOSCROLL_MS 5000   // Chu kỳ tự động cuộn trang LCD: 5 giây
#define BTN_DEBOUNCE_TIME_MS 500  // Thời gian lọc rung phím (Debounce): 500 ms
#define INTERVAL_ERA_SYNC_MS 2000 // Chu kỳ gửi dữ liệu lên E-Ra Cloud: 2 giây

// ==========================================
// 🌐 CẤU HÌNH CLOUD IOT E-RA & WIFI
// ==========================================
#define DEFAULT_MQTT_HOST "mqtt1.eoh.io"
#ifndef ERA_AUTH_TOKEN
#define ERA_AUTH_TOKEN "YOUR_ERA_AUTH_TOKEN" // Nhập Auth Token từ tài khoản E-Ra Cloud
#endif
#define WIFI_SSID "YOUR_WIFI_SSID"           // Nhập tên mạng Wi-Fi 2.4 GHz
#define WIFI_PASS "YOUR_WIFI_PASSWORD"       // Nhập mật khẩu Wi-Fi

// Virtual Pins (Chân ảo trên E-Ra Dashboard)
#define VPIN_SOLAR_VOLT V0 // Điện áp Solar (V)
#define VPIN_SOLAR_CURR V1 // Dòng điện Solar (A)
#define VPIN_SOLAR_PWR V2  // Công suất Solar (W)
#define VPIN_BATT_VOLT V3  // Điện áp Ắc quy (V)
#define VPIN_BATT_CURR V4  // Dòng sạc/xả Ắc quy (A)
#define VPIN_BATT_PWR V5   // Công suất Ắc quy (W)
#define VPIN_TEMP_PANEL V6 // Nhiệt độ tấm pin (°C)
#define VPIN_IRRADIANCE V7 // Bức xạ mặt trời (W/m²)
#define VPIN_EFFICIENCY V8 // Hiệu suất chuyển đổi (%)
