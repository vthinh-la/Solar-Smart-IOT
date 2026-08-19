#include "ina226_driver.h"

INA226Driver::INA226Driver(uint8_t i2cAddress)
    : _i2cAddress(i2cAddress), _ina(i2cAddress), _isReady(false) {}

bool INA226Driver::begin(TwoWire &wire, float rShunt, float iMax) {
    (void)wire; // INA226_WE uses default Wire or internal instance
    if (!_ina.init()) {
        _isReady = false;
        return false;
    }

    // Cấu hình shunt resistor và dải dòng đo tối đa
    _ina.setResistorRange(rShunt, iMax);

    // Cấu hình trung bình 16 mẫu đo để chống nhiễu
    _ina.setAverage(INA226_AVERAGE_16);

    // Thời gian chuyển đổi 1.1ms cho Bus và Shunt
    _ina.setConversionTime(INA226_CONV_TIME_1100);

    _isReady = true;
    return true;
}

bool INA226Driver::readData(float &voltage_V, float &current_A, float &power_W) {
    if (!_isReady) {
        voltage_V = 0.0f;
        current_A = 0.0f;
        power_W = 0.0f;
        return false;
    }

    voltage_V = _ina.getBusVoltage_V();
    float current_mA = _ina.getCurrent_mA();
    current_A = current_mA / 1000.0f;
    power_W = voltage_V * fabsf(current_A);

    // Kiểm tra tính hợp lệ của dữ liệu (tránh trường hợp bus I2C bị đơ)
    if (isnan(voltage_V) || isnan(current_A)) {
        return false;
    }

    return true;
}
