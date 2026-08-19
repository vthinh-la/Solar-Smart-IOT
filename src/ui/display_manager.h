#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "../core/data_model.h"
#include "../config.h"

class DisplayManager {
public:
    explicit DisplayManager(uint8_t i2cAddress = I2C_ADDR_LCD1602);

    void begin();
    void update(const SolarData &data);

    void nextPage();
    void prevPage();
    void toggleAutoScroll();
    void toggleBacklight();

private:
    LiquidCrystal_I2C _lcd;
    uint8_t _currentPage;
    bool _autoScroll;
    bool _backlight;
    unsigned long _lastPageChangeTime;
    unsigned long _lastUpdateTime;

    void drawPage(const SolarData &data);
};
