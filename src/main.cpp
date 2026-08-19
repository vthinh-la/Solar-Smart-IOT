#include <Arduino.h>
#include "config.h"
#include "core/sensor_manager.h"
#include "ui/button_handler.h"
#include "ui/display_manager.h"

#define ERA_DEBUG
#define DEFAULT_MQTT_HOST "mqtt1.eoh.io"
#define ERA_AUTH_TOKEN "14eeaf48-a3b5-4828-8a80-4b60bb36a972"
#include <ERa.hpp>

SensorManager sensorManager;
ButtonHandler buttonHandler;
DisplayManager displayManager;

unsigned long lastSensorRead = 0;
unsigned long lastSerialPrint = 0;
unsigned long lastEraSync = 0;

/* Callback khi kết nối thành công tới E-Ra */
ERA_CONNECTED() {
    ERA_LOG(ERA_PSTR("ERa"), ERA_PSTR("✅ ERa Cloud Connected Successfully!"));
}

/* Callback khi mất kết nối tới E-Ra */
ERA_DISCONNECTED() {
    ERA_LOG(ERA_PSTR("ERa"), ERA_PSTR("⚠️ ERa Cloud Disconnected!"));
}

/**
 * @brief Đẩy toàn bộ dữ liệu đo đạc lên E-Ra Cloud thông qua các chân ảo (Virtual Pins)
 */
void sendDataToERa(const SolarData &data) {
    if (!ERa.connected()) return;

    // Kênh 1: Thông số tấm pin mặt trời
    ERa.virtualWrite(VPIN_SOLAR_VOLT, data.solar_voltage_V);
    ERa.virtualWrite(VPIN_SOLAR_CURR, data.solar_current_A);
    ERa.virtualWrite(VPIN_SOLAR_PWR, data.solar_power_W);

    // Kênh 2: Thông số ắc quy & tải
    ERa.virtualWrite(VPIN_BATT_VOLT, data.batt_voltage_V);
    ERa.virtualWrite(VPIN_BATT_CURR, data.batt_current_A);
    ERa.virtualWrite(VPIN_BATT_PWR, data.batt_power_W);

    // Môi trường
    ERa.virtualWrite(VPIN_TEMP_PANEL, data.panel_temp_C);
    ERa.virtualWrite(VPIN_IRRADIANCE, data.irradiance_wm2);

    // Hiệu suất chuyển đổi quang điện
    ERa.virtualWrite(VPIN_EFFICIENCY, data.efficiency_pct);
}

void printSerialDebug(const SolarData &data) {
    Serial.println(F("========================================="));
    Serial.println(F("📊 SOLAR IoT TELEMETRY DEBUG REPORT"));
    Serial.println(F("========================================="));

    // Solar Panel Status
    Serial.print(F("☀️ Solar Panel: "));
    if (data.sensor_ok_ina_solar) {
        Serial.printf("%5.2f V | %5.3f A | %6.2f W\n", data.solar_voltage_V, data.solar_current_A, data.solar_power_W);
    } else {
        Serial.println(F("SENSOR ERROR"));
    }

    // Battery Status
    Serial.print(F("🔋 Battery    : "));
    if (data.sensor_ok_ina_batt) {
        Serial.printf("%5.2f V | %5.3f A | %6.2f W (%s)\n",
            data.batt_voltage_V,
            data.batt_current_A,
            data.batt_power_W,
            data.batt_current_A > 0.01f ? "CHARGING" : (data.batt_current_A < -0.01f ? "DISCHARGING" : "IDLE"));
    } else {
        Serial.println(F("SENSOR ERROR"));
    }

    // Environment
    Serial.print(F("🌡️ Temp Panel : "));
    if (data.sensor_ok_ds18b20) {
        Serial.printf("%5.1f °C\n", data.panel_temp_C);
    } else {
        Serial.println(F("SENSOR ERROR"));
    }

    Serial.print(F("💡 Irradiance : "));
    if (data.sensor_ok_bh1750) {
        Serial.printf("%7.1f Lux | %6.2f W/m²\n", data.irradiance_lux, data.irradiance_wm2);
    } else {
        Serial.println(F("SENSOR ERROR"));
    }

    // Efficiency
    Serial.print(F("⚡ Efficiency : "));
    if (data.efficiency_valid) {
        Serial.printf("%5.2f %% (Pin: %5.2f W, Pout: %5.2f W)\n", data.efficiency_pct, data.power_incident_W, data.solar_power_W);
    } else {
        Serial.println(F("INVALID (Low Light)"));
    }

    // Cloud Status
    Serial.print(F("🌐 E-Ra Cloud : "));
    Serial.println(ERa.connected() ? F("ONLINE") : F("CONNECTING..."));

    Serial.println(F("-----------------------------------------"));
}

void setup() {
    Serial.begin(115200);
    while(!Serial && millis() < 2000);
    Serial.println(F("\n🚀 STARTING SOLAR MONITORING SYSTEM WITH E-RA CLOUD..."));

    // Khởi tạo các cảm biến
    sensorManager.begin();

    // Khởi tạo các phím bấm điều hướng
    buttonHandler.begin();

    // Khởi tạo màn hình hiển thị
    displayManager.begin();

    // Cấu hình quét WiFi & Khởi động kết nối E-Ra Cloud
    ERa.setScanWiFi(true);
    ERa.begin(WIFI_SSID, WIFI_PASS);

    Serial.println(F("[SYSTEM] Initialization Complete!\n"));
}

void loop() {
    // 0. Duy trì kết nối và giao tiếp E-Ra Cloud
    ERa.run();

    // 1. Quét phím điều khiển liên tục (Real-time check)
    ButtonEvent evt = buttonHandler.checkEvents();
    if (evt != BTN_EVT_NONE) {
        switch (evt) {
            case BTN_EVT_NEXT:
                Serial.println(F("[UI] Button NEXT pressed"));
                displayManager.nextPage();
                break;
            case BTN_EVT_PREV:
                Serial.println(F("[UI] Button PREV pressed"));
                displayManager.prevPage();
                break;
            case BTN_EVT_AUTOSCROLL:
                Serial.println(F("[UI] Button AUTOSCROLL pressed"));
                displayManager.toggleAutoScroll();
                break;
            case BTN_EVT_BACKLIGHT:
                Serial.println(F("[UI] Button BACKLIGHT pressed"));
                displayManager.toggleBacklight();
                break;
            default:
                break;
        }
    }

    // 2. Định kỳ đọc dữ liệu từ các cảm biến
    unsigned long currentMillis = millis();
    if (currentMillis - lastSensorRead >= INTERVAL_SENSOR_READ_MS) {
        lastSensorRead = currentMillis;
        sensorManager.readAll();
    }

    // 3. Cập nhật giao diện màn hình LCD
    displayManager.update(sensorManager.getData());

    // 4. Định kỳ đẩy dữ liệu lên E-Ra Cloud
    if (currentMillis - lastEraSync >= INTERVAL_ERA_SYNC_MS) {
        lastEraSync = currentMillis;
        sendDataToERa(sensorManager.getData());
    }

    // 5. In thông tin debug ra cổng Serial Monitor
    if (currentMillis - lastSerialPrint >= INTERVAL_SERIAL_DEBUG_MS) {
        lastSerialPrint = currentMillis;
        printSerialDebug(sensorManager.getData());
    }
}