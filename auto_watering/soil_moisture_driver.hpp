#pragma once

#include <Arduino.h>

class SoilMoistureDriver {
private:
    uint8_t m_pin;
    uint16_t m_air_val;
    uint16_t m_water_val;

public:
    SoilMoistureDriver(uint8_t pin, uint16_t air, uint16_t water) : m_pin(pin), m_air_val(air), m_water_val(water) {}

    uint16_t value() const {
        return analogRead(m_pin);
    }
    uint16_t percent() const {
        int val = map(value(), m_air_val, m_water_val, 0, 100);
        if (val > 100)
            val = 100;
        if (val < 0)
            val = 0;
        return static_cast<uint16_t>(val);
    }

    uint8_t get_pin() const {
        return m_pin;
    }
};
