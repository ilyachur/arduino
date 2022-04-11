#include "ota.hpp"

#include "manager.hpp"
#include "wifi.hpp"

bool OTAManager::init() {
    auto w_status = WiFi.status();
    if (w_status != WL_CONNECTED && m_initialized)
        m_initialized = false;
    if (w_status != WL_CONNECTED)
        return false;
    if (!m_initialized) {
        Serial.print("OTA IP address: ");
        Serial.println(WiFi.localIP());
        m_initialized = true;
    }
    return m_initialized;
}
OTAManager::OTAManager(const Arguments& args, uint64_t interval, bool debug) : ScheduledTaskImpl(interval) {
    set_debug(debug);
    // Port defaults to 8266
    if (args.m_port > 0)
        ArduinoOTA.setPort(args.m_port);

    // Hostname defaults to esp8266-[ChipID]
    if (!args.m_hostname.empty())
        ArduinoOTA.setHostname(args.m_hostname.c_str());

    // No authentication by default
    if (!args.m_pass.empty())
        ArduinoOTA.setPassword(args.m_pass.c_str());

    // Password can be set with it's md5 value as well
    if (!args.m_pass_hash.empty())
        ArduinoOTA.setPasswordHash(args.m_pass_hash.c_str());
    if (!args.m_p_label.empty())
        ArduinoOTA.setPartitionLabel(args.m_p_label.c_str());

    ArduinoOTA.setRebootOnSuccess(args.m_reboot);
    ArduinoOTA.onStart(args.m_on_start);
    ArduinoOTA.onEnd(args.m_on_end);
    ArduinoOTA.onProgress(args.m_on_progress);
    ArduinoOTA.onError(args.m_on_error);

    ArduinoOTA.begin();
    delay(1000);
}

void OTAManager::start() {
    init();
}

void OTAManager::process() {
    ArduinoOTA.handle();
}
void OTAManager::end() {}
