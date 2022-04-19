#pragma once

#include <task.hpp>

class WaterLevelDevice : public Device {
private:
    std::string m_name;
    uint8_t m_pin;
    uint16_t m_min;
    uint16_t m_max;

public:
    TASK_ID("WaterLevelDevice");
    WaterLevelDevice(const std::string& name, uint8_t pin, uint16_t min_val, uint16_t max_val);

    Status status() const override;

    uint16_t value() const;
    uint16_t percent() const;
};

