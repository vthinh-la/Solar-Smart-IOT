#pragma once

#include <Arduino.h>
#include "../config.h"

enum ButtonEvent {
    BTN_EVT_NONE = 0,
    BTN_EVT_NEXT,
    BTN_EVT_PREV,
    BTN_EVT_AUTOSCROLL,
    BTN_EVT_BACKLIGHT
};

class ButtonHandler {
public:
    ButtonHandler();

    void begin();
    ButtonEvent checkEvents();

private:
    struct ButtonState {
        uint8_t pin;
        bool lastState;
        unsigned long lastDebounceTime;
    };

    ButtonState _btnNext;
    ButtonState _btnPrev;
    ButtonState _btnAuto;
    ButtonState _btnBacklight;

    bool checkButton(ButtonState &btn);
};
