#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <INA226_WE.h>
#include "../config.h"

class INA226Driver {
public:
    explicit INA226Driver(uint8_t i2cAddress);

    bool begin(TwoWire &wire = Wire, float rShunt = INA226_SHUNT_RESISTANCE_OHM, float iMax = INA226_MAX_CURRENT_AMPS);
    bool readData(float &voltage_V, float &current_A, float &power_W);
    bool isConnected() const { return _isReady; }

private:
    uint8_t _i2cAddress;
    INA226_WE _ina;
    bool _isReady;
};
