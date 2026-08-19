#include "display_manager.h"

DisplayManager::DisplayManager(uint8_t i2cAddress)
    : _lcd(i2cAddress, 16, 2),
      _currentPage(0),
      _autoScroll(true),
      _backlight(true),
      _lastPageChangeTime(0),
      _lastUpdateTime(0) {}

void DisplayManager::begin() {
    _lcd.init();
    if (_backlight) {
        _lcd.backlight();
    } else {
        _lcd.noBacklight();
    }
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print(F(" Solar IoT Mon  "));
    _lcd.setCursor(0, 1);
    _lcd.print(F("  Starting...   "));
    delay(1000);
    _lcd.clear();
    _lastPageChangeTime = millis();
    _lastUpdateTime = millis();
}

void DisplayManager::nextPage() {
    _currentPage = (_currentPage + 1) % 5;
    _lastPageChangeTime = millis();
    _lcd.clear(); // Xóa màn hình khi chuyển trang để tránh ký tự thừa từ trang cũ
}

void DisplayManager::prevPage() {
    if (_currentPage == 0) {
        _currentPage = 4;
    } else {
        _currentPage--;
    }
    _lastPageChangeTime = millis();
    _lcd.clear();
}

void DisplayManager::toggleAutoScroll() {
    _autoScroll = !_autoScroll;
    _lastPageChangeTime = millis();

    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print(F("   AutoScroll   "));
    _lcd.setCursor(0, 1);
    _lcd.print(_autoScroll ? F("      [ON]      ") : F("     [OFF]      "));
    delay(1000);
    _lcd.clear();
}

void DisplayManager::toggleBacklight() {
    _backlight = !_backlight;
    if (_backlight) {
        _lcd.backlight();
    } else {
        _lcd.noBacklight();
    }
}

void DisplayManager::update(const SolarData &data) {
    unsigned long currentMillis = millis();

    // 1. Tự động chuyển trang sau mỗi khoảng thời gian cài đặt
    if (_autoScroll && (currentMillis - _lastPageChangeTime >= INTERVAL_AUTOSCROLL_MS)) {
        nextPage();
    }

    // 2. Định kỳ cập nhật màn hình LCD
    if (currentMillis - _lastUpdateTime >= INTERVAL_LCD_UPDATE_MS) {
        _lastUpdateTime = currentMillis;
        drawPage(data);
    }
}

void DisplayManager::drawPage(const SolarData &data) {
    char line1[17];
    char line2[17];
    // Đổ đầy ký tự khoảng trắng mặc định để định dạng chính xác 16 ký tự
    memset(line1, ' ', 16);
    line1[16] = '\0';
    memset(line2, ' ', 16);
    line2[16] = '\0';

    switch (_currentPage) {
        case 0: // Solar Performance
            // S: XX.XV X.XXA
            snprintf(line1, sizeof(line1), "S:%4.1fV %5.2fA  ", data.solar_voltage_V, data.solar_current_A);
            // P_PV: XXX.XX W
            snprintf(line2, sizeof(line2), "P_PV:%6.2f W   ", data.solar_power_W);
            break;

        case 1: // Battery & Load
            // B: XX.XV X.XXA
            snprintf(line1, sizeof(line1), "B:%4.1fV %5.2fA  ", data.batt_voltage_V, data.batt_current_A);
            // P_B: XXX.XXW S/X
            {
                char stateChar = 'I'; // Idle
                if (data.batt_current_A > 0.05f) {
                    stateChar = 'S'; // Charging
                } else if (data.batt_current_A < -0.05f) {
                    stateChar = 'X'; // Discharging
                }
                snprintf(line2, sizeof(line2), "P_B:%5.1fW [%c]   ", data.batt_power_W, stateChar);
            }
            break;

        case 2: // Environment
            // T_Pan: XX.X C
            if (data.sensor_ok_ds18b20) {
                // Sử dụng mã 0xDF cho ký tự độ (°C)
                snprintf(line1, sizeof(line1), "T_Pan:%5.1f\xDF""C    ", data.panel_temp_C);
            } else {
                snprintf(line1, sizeof(line1), "T_Pan: ERROR    ");
            }
            // Rad: XXXX.X W/m2
            if (data.sensor_ok_bh1750) {
                snprintf(line2, sizeof(line2), "Rad:%6.1f W/m2  ", data.irradiance_wm2);
            } else {
                snprintf(line2, sizeof(line2), "Rad: ERROR      ");
            }
            break;

        case 3: // Efficiency
            // P_in: XXX.XX W
            snprintf(line1, sizeof(line1), "P_in:%6.2f W   ", data.power_incident_W);
            // Eff: XX.X% [OK]
            if (data.efficiency_valid) {
                snprintf(line2, sizeof(line2), "Eff:%5.1f%% [OK]  ", data.efficiency_pct);
            } else {
                snprintf(line2, sizeof(line2), "Eff:---%% [LO_SUN]");
            }
            break;

        case 4: // System Health & Config status
            // PV:OK BT:OK L:OK
            snprintf(line1, sizeof(line1), "PV:%s BT:%s L:%s",
                     data.sensor_ok_ina_solar ? "OK " : "ERR",
                     data.sensor_ok_ina_batt  ? "OK " : "ERR",
                     data.sensor_ok_bh1750    ? "OK " : "ERR");
            // T:OK AUTO:ON/OFF
            snprintf(line2, sizeof(line2), "T:%s AUTO:%s   ",
                     data.sensor_ok_ds18b20   ? "OK " : "ERR",
                     _autoScroll ? "ON " : "OFF");
            break;
    }

    // Đảm bảo kết thúc bằng NULL tại vị trí 16 để tránh tràn hiển thị
    line1[16] = '\0';
    line2[16] = '\0';

    // Vẽ trực tiếp lên LCD mà không dùng clear() nhằm hạn chế nhấp nháy màn hình
    _lcd.setCursor(0, 0);
    _lcd.print(line1);
    _lcd.setCursor(0, 1);
    _lcd.print(line2);
}
