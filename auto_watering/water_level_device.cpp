#include "water_level_device.hpp"

#include <Arduino.h>

WaterLevelDevice::WaterLevelDevice(const std::string& name, uint8_t pin, uint16_t min_val, uint16_t max_val)
    : m_name(name),
      m_pin(pin),
      m_min(min_val),
      m_max(max_val) {}

Status WaterLevelDevice::status() const {
    Status stat;
    stat.used_pins.emplace_back(m_pin);
    stat.name = m_name;
    stat.public_val["level"] = {String(percent()).c_str(), "%"};
    stat.info["value"] = String(value()).c_str();
    return stat;
}
uint16_t WaterLevelDevice::value() const {
    return analogRead(m_pin);
}
uint16_t WaterLevelDevice::percent() const {
    int val = map(value(), m_min, m_max, 0, 100);
    if (val > 100)
        val = 100;
    if (val < 0)
        val = 0;
    return static_cast<uint16_t>(val);
}

