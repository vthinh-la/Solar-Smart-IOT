#include <Arduino.h>
#include "config.h"
#include "core/sensor_manager.h"
#include "ui/button_handler.h"
#include "ui/display_manager.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#define ERA_DEBUG
#include <ERa.hpp>

SensorManager sensorManager;
ButtonHandler buttonHandler;
DisplayManager displayManager;

unsigned long lastSensorRead = 0;
unsigned long lastSerialPrint = 0;
QueueHandle_t eraDataQueue = nullptr;
volatile bool eraOnline = false;

/* Callback khi kết nối thành công tới E-Ra */
ERA_CONNECTED() {
    eraOnline = true;
    ERA_LOG(ERA_PSTR("ERa"), ERA_PSTR("✅ ERa Cloud Connected Successfully!"));
}

/* Callback khi mất kết nối tới E-Ra */
ERA_DISCONNECTED() {
    eraOnline = false;
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
    // Publish V8 immediately, including when the value stays at 0 in low light.
    ERa.virtualWrite(VPIN_EFFICIENCY, data.efficiency_pct, true);
    Serial.printf("[ERa] V8 efficiency: %.2f %% (%s)\n",
                  data.efficiency_pct,
                  data.efficiency_valid ? "valid" : "low light");
}

// WiFi and MQTT waits stay off the task that updates sensors and the LCD.
void eraTask(void *parameter) {
    (void)parameter;
    // Load both WiFi networks saved by ERa before applying firmware defaults.
    ERa.setPersistent(true);
    // Let ESP32 associate by SSID. A missed scan must not prevent WiFi 2.
    ERa.setScanWiFi(false);
    ERa.setNonBlocking(true);
    ERa.begin(WIFI_SSID, WIFI_PASS);

    Serial.printf("[ERa] Primary WiFi: %s | Backup WiFi: %s\n",
                  ERaConfig.ssid,
                  ERaConfig.hasBackup ? ERaConfig.backupSSID : "not configured");

    unsigned long lastSync = millis();
    for (;;) {
        ERa.run();
        eraOnline = ERa.connected();

        const unsigned long now = millis();
        if (now - lastSync >= INTERVAL_ERA_SYNC_MS) {
            lastSync = now;
            SolarData snapshot;
            if (xQueuePeek(eraDataQueue, &snapshot, 0) == pdTRUE) {
                sendDataToERa(snapshot);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
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
    Serial.println(eraOnline ? F("ONLINE") : F("OFFLINE"));

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
    eraDataQueue = xQueueCreate(1, sizeof(SolarData));
    if (eraDataQueue == nullptr ||
        xTaskCreatePinnedToCore(eraTask, "ERa", 12288, nullptr, 1, nullptr, 0) != pdPASS) {
        Serial.println(F("[ERa] Cannot start cloud task; local display remains active"));
    }

    Serial.println(F("[SYSTEM] Initialization Complete!\n"));
}

void loop() {
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
        if (eraDataQueue != nullptr) {
            SolarData snapshot = sensorManager.getData();
            xQueueOverwrite(eraDataQueue, &snapshot);
        }
    }

    // 3. Cập nhật giao diện màn hình LCD
    displayManager.update(sensorManager.getData());

    // 4. In thông tin debug ra cổng Serial Monitor
    if (currentMillis - lastSerialPrint >= INTERVAL_SERIAL_DEBUG_MS) {
        lastSerialPrint = currentMillis;
        printSerialDebug(sensorManager.getData());
    }
}
