#pragma once

#include <Wifi.h>

#include "task.hpp"

class WifiManager : public ScheduledTaskImpl {
private:
    // Wifi status
    wl_status_t wifiStatus;

public:
    TASK_ID("WifiManager");
    WifiManager(const std::string& ssid, const std::string& pass, uint64_t interval = 1000, bool debug = false);
    void process() override;
};
