#include "bh1750_driver.h"

BH1750Driver::BH1750Driver(uint8_t i2cAddress)
    : _i2cAddress(i2cAddress), _isReady(false) {}

bool BH1750Driver::begin(TwoWire &wire) {
    _isReady = _lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, _i2cAddress, &wire);
    return _isReady;
}

bool BH1750Driver::readLight(float &lux, float &irradiance_wm2) {
    if (!_isReady) {
        lux = 0.0f;
        irradiance_wm2 = 0.0f;
        return false;
    }

    lux = _lightMeter.readLightLevel();
    if (lux < 0.0f || isnan(lux)) {
        lux = 0.0f;
        irradiance_wm2 = 0.0f;
        return false;
    }

    // Quy đổi Lux sang W/m² bức xạ mặt trời (G)
    // Hệ số chuẩn: 1 W/m² ≈ 120 Lux (hoặc G = Lux / 120.0)
    irradiance_wm2 = lux / LUX_TO_WM2_DIVISOR;

    return true;
}
