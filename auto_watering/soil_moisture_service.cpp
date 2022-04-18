#include "soil_moisture_service.hpp"

SoilMoistureService::SoilMoistureService(const std::string& name, uint8_t pin, uint16_t air_val, uint16_t water_val)
    : m_driver(pin, air_val, water_val),
      m_name(name) {}

Event SoilMoistureService::status() const {
    Event ev;
    ev.from = type_info();
    ev.args["status"] = m_driver.get_pin();
    ev.args["name"] = m_name;
    ev.args["params"] = "Moisture";
    ev.args["Moisture_val"] = String(m_driver.percent()).c_str();
    ev.args["Moisture_type"] = "%";
    return ev;
}
