#pragma once

#include <task.hpp>

class SoilMoistureDevice : public Device {
private:
    uint8_t m_pin;
    uint16_t m_air_val;
    uint16_t m_water_val;
    std::string m_name;

public:
    TASK_ID("SoilMoistureDevice");
    SoilMoistureDevice(const std::string& name, uint8_t pin, uint16_t air_val, uint16_t water_val);

    Status status() const override;

    uint16_t value() const;
    uint16_t percent() const;
};
