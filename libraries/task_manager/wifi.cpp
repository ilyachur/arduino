#include "wifi.hpp"

#include "manager.hpp"
#include "task.hpp"

WifiManager::WifiManager(const std::string& ssid, const std::string& pass, uint64_t interval, bool debug)
    : ScheduledTaskImpl(interval),
      wifiStatus(WL_NO_SHIELD) {
    set_debug(debug);
    TaskManager::get_instance().log(this, "Connecting to SSID: " + String(ssid.c_str()));
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());
}

void WifiManager::process() {
    wl_status_t newStatus = WiFi.status();

    if (newStatus != wifiStatus) {
        switch (WiFi.status()) {
        case WL_CONNECTED:
            TaskManager::get_instance().log(this, "Wifi connected", false);
            break;
        case WL_DISCONNECTED:
            TaskManager::get_instance().log(this, "Wifi disconnected", false);
            break;
        case WL_NO_SHIELD:
            TaskManager::get_instance().log(this, "No Wifi shield", false);
            break;
        case WL_IDLE_STATUS:
            TaskManager::get_instance().log(this, "Wifi IDLE", false);
            break;
        case WL_NO_SSID_AVAIL:
            TaskManager::get_instance().log(this, "SSID doesn't available", false);
            break;
        case WL_SCAN_COMPLETED:
            TaskManager::get_instance().log(this, "Wifi scan completed", false);
            break;
        case WL_CONNECT_FAILED:
            TaskManager::get_instance().log(this, "Wifi connect failed", false);
            break;
        case WL_CONNECTION_LOST:
            TaskManager::get_instance().log(this, "Wifi connection lost", false);
            break;
        }

        if (newStatus == WL_DISCONNECTED || newStatus == WL_CONNECTION_LOST || newStatus == WL_NO_SSID_AVAIL) {
            TaskManager::get_instance().log(this, "Connecting", false);
        }
    }
    if (newStatus == WL_DISCONNECTED || newStatus == WL_CONNECTION_LOST || newStatus == WL_NO_SSID_AVAIL) {
        TaskManager::get_instance().log(".", true);
    }

    wifiStatus = newStatus;
}
