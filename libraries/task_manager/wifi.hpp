#pragma once

#include <Wifi.h>

#include "task.hpp"

class WifiManager : public ScheduledTaskImpl {
private:
    // Wifi status
    wl_status_t wifiStatus;

public:
    WifiManager(const std::string& ssid, const std::string& pass, uint64_t interval = 1000, bool debug = false);
    void start() override;
    void process() override;
    void end() override;
    const char* name() const override;

    void set_update_interval(uint64_t interval);
};
