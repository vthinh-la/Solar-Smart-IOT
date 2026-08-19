#pragma once

#include <Arduino.h>

/**
 * @brief Cấu trúc dữ liệu chứa toàn bộ thông số đo đạc và tính toán của hệ thống
 */
struct SolarData {
    // Kênh 1: Tấm pin mặt trời (Solar Harvest -> MPPT)
    float solar_voltage_V       = 0.0f; // Điện áp tấm pin (V)
    float solar_current_A       = 0.0f; // Dòng điện tấm pin (A)
    float solar_power_W         = 0.0f; // Công suất tấm pin phát ra P_out (W)

    // Kênh 2: Khối pin lưu trữ & Tải tiêu thụ (Battery Storage & Load)
    float batt_voltage_V        = 0.0f; // Điện áp khối pin (V)
    float batt_current_A        = 0.0f; // Dòng sạc/xả (+: sạc vào pin, -: xả ra tải) (A)
    float batt_power_W          = 0.0f; // Công suất sạc/xả (W)

    // Môi trường quang điện (Environment Sensing)
    float irradiance_lux        = 0.0f; // Cường độ sáng đo từ BH1750 (Lux)
    float irradiance_wm2        = 0.0f; // Bức xạ mặt trời quy đổi G (W/m²)
    float panel_temp_C          = 0.0f; // Nhiệt độ bề mặt tấm pin (°C)

    // Tính toán hiệu suất (Calculated Solar Efficiency)
    float power_incident_W      = 0.0f; // Công suất bức xạ chiếu tới P_in = G * Area (W)
    float efficiency_pct        = 0.0f; // Hiệu suất chuyển đổi quang-điện tức thời η (%)
    bool  efficiency_valid      = false;// Cờ báo hiệu suất có hợp lệ (đủ điều kiện ánh sáng)

    // Trạng thái kết nối cảm biến (Health status)
    bool  sensor_ok_ina_solar   = false; // INA226 Kênh Solar (0x41)
    bool  sensor_ok_ina_batt    = false; // INA226 Kênh Battery (0x40)
    bool  sensor_ok_bh1750      = false; // BH1750 (0x23)
    bool  sensor_ok_ds18b20     = false; // DS18B20 (GPIO 23)
};
