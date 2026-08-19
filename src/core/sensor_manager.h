#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "data_model.h"
#include "efficiency_calc.h"
#include "../sensors/ina226_driver.h"
#include "../sensors/bh1750_driver.h"
#include "../sensors/ds18b20_driver.h"
#include "../config.h"

class SensorManager {
public:
    SensorManager();

    bool begin();
    void readAll();
    const SolarData& getData() const { return _data; }
    void printI2CScan();

private:
    SolarData _data;
    INA226Driver _inaSolar;
    INA226Driver _inaBatt;
    BH1750Driver _bh1750;
    DS18B20Driver _ds18b20;
    EfficiencyCalculator _effCalc;
};
