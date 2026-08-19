#include "sensor_manager.h"

SensorManager::SensorManager()
    : _inaSolar(I2C_ADDR_INA226_SOLAR),
      _inaBatt(I2C_ADDR_INA226_BATT),
      _bh1750(I2C_ADDR_BH1750),
      _ds18b20(PIN_ONE_WIRE),
      _effCalc(PANEL_AREA_M2) {}

void SensorManager::printI2CScan() {
    Serial.println(F("\n--- [I2C Bus Scan] ---"));
    uint8_t count = 0;
    for (uint8_t addr = 1; addr < 127; ++addr) {
        Wire.beginTransmission(addr);
        uint8_t error = Wire.endTransmission();
        if (error == 0) {
            Serial.print(F("Found I2C device at 0x"));
            if (addr < 16) Serial.print("0");
            Serial.print(addr, HEX);
            
            // Chú thích thiết bị
            if (addr == I2C_ADDR_INA226_SOLAR) Serial.print(F(" (INA226 Solar)"));
            else if (addr == I2C_ADDR_INA226_BATT) Serial.print(F(" (INA226 Battery)"));
            else if (addr == I2C_ADDR_BH1750) Serial.print(F(" (BH1750 Light)"));
            else if (addr == I2C_ADDR_LCD1602) Serial.print(F(" (LCD 1602)"));
            
            Serial.println();
            count++;
        }
    }
    if (count == 0) {
        Serial.println(F("No I2C devices found! Check wiring and pull-up resistors."));
    }
    Serial.println(F("----------------------\n"));
}

bool SensorManager::begin() {
    // Khởi tạo I2C Bus với tần số 100kHz (hoặc 400kHz)
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    Wire.setClock(100000);

    // Scan để debug
    printI2CScan();

    // Khởi tạo INA226 Solar (0x41)
    _data.sensor_ok_ina_solar = _inaSolar.begin(Wire, INA226_SHUNT_RESISTANCE_OHM, INA226_MAX_CURRENT_AMPS);
    if (_data.sensor_ok_ina_solar) {
        Serial.println(F("[INIT] INA226 Solar (0x41) OK"));
    } else {
        Serial.println(F("[WARN] INA226 Solar (0x41) FAILED"));
    }

    // Khởi tạo INA226 Battery (0x40)
    _data.sensor_ok_ina_batt = _inaBatt.begin(Wire, INA226_SHUNT_RESISTANCE_OHM, INA226_MAX_CURRENT_AMPS);
    if (_data.sensor_ok_ina_batt) {
        Serial.println(F("[INIT] INA226 Battery (0x40) OK"));
    } else {
        Serial.println(F("[WARN] INA226 Battery (0x40) FAILED"));
    }

    // Khởi tạo BH1750 (0x23)
    _data.sensor_ok_bh1750 = _bh1750.begin(Wire);
    if (_data.sensor_ok_bh1750) {
        Serial.println(F("[INIT] BH1750 Light Meter (0x23) OK"));
    } else {
        Serial.println(F("[WARN] BH1750 Light Meter (0x23) FAILED"));
    }

    // Khởi tạo DS18B20 (GPIO 23)
    _data.sensor_ok_ds18b20 = _ds18b20.begin();
    if (_data.sensor_ok_ds18b20) {
        Serial.println(F("[INIT] DS18B20 Temp Sensor OK"));
    } else {
        Serial.println(F("[WARN] DS18B20 Temp Sensor FAILED"));
    }

    return true;
}

void SensorManager::readAll() {
    // 1. Đọc INA226 Kênh 1: Solar
    if (_data.sensor_ok_ina_solar) {
        bool ok = _inaSolar.readData(_data.solar_voltage_V, _data.solar_current_A, _data.solar_power_W);
        if (!ok) {
            // Thử khởi tạo lại nếu lỗi bus
            _data.sensor_ok_ina_solar = _inaSolar.begin(Wire, INA226_SHUNT_RESISTANCE_OHM, INA226_MAX_CURRENT_AMPS);
        }
    } else {
        // Thử kết nối lại
        _data.sensor_ok_ina_solar = _inaSolar.begin(Wire, INA226_SHUNT_RESISTANCE_OHM, INA226_MAX_CURRENT_AMPS);
    }

    // 2. Đọc INA226 Kênh 2: Battery & Load
    if (_data.sensor_ok_ina_batt) {
        bool ok = _inaBatt.readData(_data.batt_voltage_V, _data.batt_current_A, _data.batt_power_W);
        if (!ok) {
            _data.sensor_ok_ina_batt = _inaBatt.begin(Wire, INA226_SHUNT_RESISTANCE_OHM, INA226_MAX_CURRENT_AMPS);
        }
    } else {
        _data.sensor_ok_ina_batt = _inaBatt.begin(Wire, INA226_SHUNT_RESISTANCE_OHM, INA226_MAX_CURRENT_AMPS);
    }

    // 3. Đọc BH1750 Bức xạ
    if (_data.sensor_ok_bh1750) {
        bool ok = _bh1750.readLight(_data.irradiance_lux, _data.irradiance_wm2);
        if (!ok) {
            _data.sensor_ok_bh1750 = _bh1750.begin(Wire);
        }
    } else {
        _data.sensor_ok_bh1750 = _bh1750.begin(Wire);
    }

    // 4. Đọc DS18B20 Nhiệt độ
    if (_data.sensor_ok_ds18b20) {
        bool ok = _ds18b20.readTemperature(_data.panel_temp_C);
        if (!ok) {
            _data.sensor_ok_ds18b20 = _ds18b20.begin();
        }
    } else {
        _data.sensor_ok_ds18b20 = _ds18b20.begin();
    }

    // 5. Tính toán hiệu suất tức thời
    _effCalc.compute(_data);
}
