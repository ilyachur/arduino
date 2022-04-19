#include "relay.hpp"

#include <Arduino.h>

RelayDevice::RelayDevice(const std::string& name, const std::vector<uint8_t>& pins, bool high_type, bool device)
    : m_name(name),
      m_pins(pins),
      m_type(high_type) {
    set_debug(device);
    for (const auto& pin : m_pins) {
        pinMode(pin, OUTPUT);
    }
    off();
}

void RelayDevice::off() const {
    for (size_t i = 0; i < m_pins.size(); i++)
        off(i);
}

void RelayDevice::off(uint8_t idx) const {
    if (m_type)
        digitalWrite(m_pins[idx], LOW);
    else
        digitalWrite(m_pins[idx], HIGH);
}

void RelayDevice::on() const {
    for (size_t i = 0; i < m_pins.size(); i++)
        on(i);
}

void RelayDevice::on(uint8_t idx) const {
    if (m_type)
        digitalWrite(m_pins[idx], HIGH);
    else
        digitalWrite(m_pins[idx], LOW);
}

Status RelayDevice::status() const {
    Status stat;
    stat.used_pins = m_pins;
    stat.name = m_name;

    for (size_t i = 0; i < m_pins.size(); i++) {
        if (m_type)
            stat.public_val[String(i).c_str()] = {digitalRead(m_pins[i]) == HIGH ? "on" : "off", ""};
        else
            stat.public_val[String(i).c_str()] = {digitalRead(m_pins[i]) == LOW ? "on" : "off", ""};
    }
    return stat;
}

void RelayDevice::update(const Event& event) {
    if (event.to != type_info())
        return;
    uint8_t pin = static_cast<uint8_t>(String(event.args.at("pin").c_str()).toInt());
    if (event.args.at("enable") == "on")
        on(pin);
    else
        off(pin);
}

void RelayDevice::select(const std::string& param) {
    uint8_t i = static_cast<uint8_t>(String(param.c_str()).toInt());
    if (m_type)
        digitalRead(m_pins[i]) == LOW ? on(i) : off(i);
    else
        digitalRead(m_pins[i]) == HIGH ? on(i) : off(i);
}
