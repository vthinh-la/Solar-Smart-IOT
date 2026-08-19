#pragma once

#include <Arduino.h>
#include "data_model.h"
#include "../config.h"

class EfficiencyCalculator {
public:
    EfficiencyCalculator(float panelArea_m2 = PANEL_AREA_M2);

    void setPanelArea(float area_m2);
    float getPanelArea() const { return _panelArea_m2; }

    /**
     * @brief Tính toán công suất bức xạ chiếu tới và hiệu suất quang-điện tức thời
     * @param data Tham chiếu tới cấu trúc SolarData để cập nhật kết quả
     */
    void compute(SolarData &data);

private:
    float _panelArea_m2;
};
