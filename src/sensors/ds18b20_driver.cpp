#include "ds18b20_driver.h"

DS18B20Driver::DS18B20Driver(uint8_t pin)
    : _pin(pin), _oneWire(pin), _sensors(&_oneWire), _isReady(false) {}

bool DS18B20Driver::begin() {
    _sensors.begin();
    uint8_t deviceCount = _sensors.getDeviceCount();
    if (deviceCount > 0) {
        _sensors.setResolution(11); // Độ phân giải 11-bit (0.125°C, chuyển đổi ~375ms)
        _isReady = true;
    } else {
        _isReady = false;
    }
    return _isReady;
}

bool DS18B20Driver::readTemperature(float &temp_C) {
    _sensors.requestTemperatures();
    float temp = _sensors.getTempCByIndex(0);

    // DEVICE_DISCONNECTED_C = -127.0°C hoặc 85.0°C (giá trị power-on reset khi lỗi đọc)
    if (temp == DEVICE_DISCONNECTED_C || temp == 85.0f || isnan(temp)) {
        temp_C = 0.0f;
        _isReady = false;
        return false;
    }

    temp_C = temp;
    _isReady = true;
    return true;
}
