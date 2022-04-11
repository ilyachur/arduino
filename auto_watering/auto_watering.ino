#include <ArduinoOTA.h>
#include <ESPmDNS.h>

#include <manager.hpp>
#include <ota.hpp>
#include <telegram.hpp>
#include <wifi.hpp>

#include "utils/private_constants.hpp"

TaskManager& task_manager = TaskManager::get_instance();
//переменные для мигания светодиодом через millis

const int led = 2;
unsigned long previousMillis = 0;
const long interval = 5000;
int ledState = LOW;

void setup() {
    pinMode(led, OUTPUT);

    task_manager.set_debug(true);

    Serial.begin(115200);
    while (!Serial) {
    }
    delay(100);
    Serial.flush();

    Serial.println("Booting");
    task_manager.register_task(Task::create<WifiManager>(ssid, password, 1000, task_manager.is_debug()));
    // Doesn't work with Arduino IDE 2.0
    task_manager.register_task(
        Task::create<OTAManager>(OTAManager::Arguments()
                                     .set_password(dev_passwd)
                                     .set_on_start([]() {
                                         String type;
                                         if (ArduinoOTA.getCommand() == U_FLASH)
                                             type = "sketch";
                                         else  // U_SPIFFS
                                             type = "filesystem";

                                         // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using
                                         // SPIFFS.end()
                                         Serial.println("Start updating " + type);
                                     })
                                     .set_on_end([]() {
                                         Serial.println("End");
                                     })
                                     .set_on_progress([](unsigned int progress, unsigned int total) {
                                         Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
                                     })
                                     .set_on_error([](ota_error_t error) {
                                         Serial.printf("Error[%u]: ", error);
                                         if (error == OTA_AUTH_ERROR)
                                             Serial.println("Auth Failed");
                                         else if (error == OTA_BEGIN_ERROR)
                                             Serial.println("Begin Failed");
                                         else if (error == OTA_CONNECT_ERROR)
                                             Serial.println("Connect Failed");
                                         else if (error == OTA_RECEIVE_ERROR)
                                             Serial.println("Receive Failed");
                                         else if (error == OTA_END_ERROR)
                                             Serial.println("Endk Failed");
                                     }),
                                 1,
                                 task_manager.is_debug()));

    // Init telegram bot
    task_manager.register_task(
        Task::create<TelegramManager>(bot_tocken, authorized_users, 1000, task_manager.is_debug()));

    Serial.println("Ready");
}

void loop() {
    task_manager.execute();

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        ledState = not(ledState);
        digitalWrite(led, ledState);
    }
}
