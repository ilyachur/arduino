#include "soil_moisture_device.hpp"

#include <Arduino.h>

SoilMoistureDevice::SoilMoistureDevice(const std::string& name, uint8_t pin, uint16_t air_val, uint16_t water_val)
    : m_pin(pin),
      m_air_val(air_val),
      m_water_val(water_val),
      m_name(name) {}

Status SoilMoistureDevice::status() const {
    Status stat;
    stat.used_pins.emplace_back(m_pin);
    stat.name = m_name;
    stat.public_val["moisture"] = {String(percent()).c_str(), "%"};
    stat.info["value"] = String(value()).c_str();
    return stat;
}
uint16_t SoilMoistureDevice::value() const {
    return analogRead(m_pin);
}
uint16_t SoilMoistureDevice::percent() const {
    int val = map(value(), m_air_val, m_water_val, 0, 100);
    if (val > 100)
        val = 100;
    if (val < 0)
        val = 0;
    return static_cast<uint16_t>(val);
}

