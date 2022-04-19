#pragma once

#include <task.hpp>
#include <vector>

class RelayDevice : public Device {
public:
    TASK_ID("RelayDevice");
    RelayDevice(const std::string& name, const std::vector<uint8_t>& pins, bool high_type = true, bool debug = false);

    Status status() const override;
    void update(const Event& event) override;

    // Disable all outputs
    void off() const;
    void off(uint8_t relay_idx) const;

    void on() const;
    void on(uint8_t relay_idx) const;

private:
    std::string m_name;
    std::vector<uint8_t> m_pins;
    bool m_type;
};

