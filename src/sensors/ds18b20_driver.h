#pragma once

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "../config.h"

class DS18B20Driver {
public:
    explicit DS18B20Driver(uint8_t pin = PIN_ONE_WIRE);

    bool begin();
    bool readTemperature(float &temp_C);
    bool isConnected() const { return _isReady; }

private:
    uint8_t _pin;
    OneWire _oneWire;
    DallasTemperature _sensors;
    bool _isReady;
};
