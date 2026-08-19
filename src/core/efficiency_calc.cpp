#include "efficiency_calc.h"

EfficiencyCalculator::EfficiencyCalculator(float panelArea_m2)
    : _panelArea_m2(panelArea_m2 > 0.001f ? panelArea_m2 : PANEL_AREA_M2) {}

void EfficiencyCalculator::setPanelArea(float area_m2) {
    if (area_m2 > 0.001f) {
        _panelArea_m2 = area_m2;
    }
}

void EfficiencyCalculator::compute(SolarData &data) {
    // 1. Tính công suất bức xạ chiếu tới tấm pin: P_incident = G (W/m²) * Area (m²)
    data.power_incident_W = data.irradiance_wm2 * _panelArea_m2;

    // 2. Kiểm tra điều kiện tính hiệu suất:
    // Cần có ánh sáng tối thiểu (G >= 10 W/m²) và diện tích tấm pin hợp lệ
    if (data.irradiance_wm2 < MIN_IRRADIANCE_WM2_FOR_EFF || data.power_incident_W <= 0.0001f) {
        data.efficiency_pct = 0.0f;
        data.efficiency_valid = false;
        return;
    }

    // 3. Đảm bảo công suất phát ra không âm
    float p_out = data.solar_power_W;
    if (p_out < 0.0f) {
        p_out = 0.0f;
    }

    // 4. Tính toán hiệu suất tức thời: η = (P_out / P_in) * 100%
    float eff = (p_out / data.power_incident_W) * 100.0f;

    // 5. Xử lý giới hạn vật lý (Tránh lỗi do đọc sai cảm biến hoặc đèn flash rọi cục bộ)
    if (eff > 99.9f) {
        eff = 99.9f;
    } else if (eff < 0.0f) {
        eff = 0.0f;
    }

    data.efficiency_pct = eff;
    data.efficiency_valid = true;
}
