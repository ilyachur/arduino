#pragma once

#include <task.hpp>

#include "soil_moisture_driver.hpp"

class SoilMoistureService : public DriverServiceImpl {
private:
    SoilMoistureDriver m_driver;
    std::string m_name;

public:
    TASK_ID("SoilMoistureDevice");
    SoilMoistureService(const std::string& name, uint8_t pin, uint16_t air_val, uint16_t water_val);

    Event status() const override;
};
