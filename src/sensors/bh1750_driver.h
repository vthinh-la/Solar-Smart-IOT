#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>
#include "../config.h"

class BH1750Driver {
public:
    explicit BH1750Driver(uint8_t i2cAddress = I2C_ADDR_BH1750);

    bool begin(TwoWire &wire = Wire);
    bool readLight(float &lux, float &irradiance_wm2);
    bool isConnected() const { return _isReady; }

private:
    uint8_t _i2cAddress;
    BH1750 _lightMeter;
    bool _isReady;
};
