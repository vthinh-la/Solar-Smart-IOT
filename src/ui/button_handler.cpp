#include "button_handler.h"

ButtonHandler::ButtonHandler() {
    _btnNext = {PIN_BTN_NEXT, true, 0};
    _btnPrev = {PIN_BTN_PREV, true, 0};
    _btnAuto = {PIN_BTN_AUTOSCROLL, true, 0};
    _btnBacklight = {PIN_BTN_BACKLIGHT, true, 0};
}

void ButtonHandler::begin() {
    pinMode(_btnNext.pin, INPUT_PULLUP);
    pinMode(_btnPrev.pin, INPUT_PULLUP);
    pinMode(_btnAuto.pin, INPUT_PULLUP);
    pinMode(_btnBacklight.pin, INPUT_PULLUP);

    // Đọc trạng thái ban đầu của các nút
    _btnNext.lastState = digitalRead(_btnNext.pin);
    _btnPrev.lastState = digitalRead(_btnPrev.pin);
    _btnAuto.lastState = digitalRead(_btnAuto.pin);
    _btnBacklight.lastState = digitalRead(_btnBacklight.pin);
}

bool ButtonHandler::checkButton(ButtonState &btn) {
    bool reading = digitalRead(btn.pin);
    bool pressed = false;

    // Phát hiện nút nhấn khi chuyển từ HIGH sang LOW (sườn xuống)
    if (reading == LOW && btn.lastState == HIGH) {
        // Chỉ ghi nhận sự kiện nếu đã vượt quá khoảng thời gian chống rung phím
        if (millis() - btn.lastDebounceTime > BTN_DEBOUNCE_TIME_MS) {
            pressed = true;
            btn.lastDebounceTime = millis();
        }
    }
    btn.lastState = reading;
    return pressed;
}

ButtonEvent ButtonHandler::checkEvents() {
    if (checkButton(_btnNext)) return BTN_EVT_NEXT;
    if (checkButton(_btnPrev)) return BTN_EVT_PREV;
    if (checkButton(_btnAuto)) return BTN_EVT_AUTOSCROLL;
    if (checkButton(_btnBacklight)) return BTN_EVT_BACKLIGHT;
    return BTN_EVT_NONE;
}
