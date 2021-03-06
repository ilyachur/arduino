#include <lcd.hpp>
#include <manager.hpp>
#include <ota.hpp>
#include <task.hpp>
#include <telegram.hpp>
#include <wifi.hpp>

#include "controller.hpp"
#include "relay.hpp"
#include "soil_moisture_device.hpp"
#include "utils/private_constants.hpp"
#include "water_level_device.hpp"

TaskManager& task_manager = TaskManager::get_instance();
MenuController menu_controller({35, 32, 33}, true);

class Led : public ScheduledTaskImpl {
private:
    const int m_led;
    int m_led_state;

public:
    TASK_ID("Led");
    Led(int led, int state, uint64_t interval) : ScheduledTaskImpl(interval), m_led(led), m_led_state(state) {
        pinMode(led, OUTPUT);
    }

    void start() override {}
    void process() override {
        m_led_state = not(m_led_state);
        digitalWrite(m_led, m_led_state);
    }
    void update(const Event& event) {
        if (event.to != type_info())
            return;
        auto it_t = event.args.find("type");
        auto it_n = event.args.find("val");
        if (it_t != event.args.end() && it_t->second == "led_timeout" && it_n != event.args.end()) {
            String value = it_n->second.c_str();
            set_interval(value.toInt());
        }
    }
    void end() override {}
};

void setup() {
    task_manager.set_debug(true);

    Serial.begin(115200);
    while (!Serial) {
    }
    delay(100);
    Serial.flush();

    task_manager.log("Booting");
    // LCD handler. Signals: [ RS, EN, D4, D5, D6, D7 ], w and h
    task_manager.register_task(Task::create<LCDManager>(13, 12, 14, 27, 26, 25, 16, 2, task_manager.is_debug()));

    {
        // clear LCD
        Event event{"Main", LCDManager::static_type_info(), {{"type", "clear"}}};
        task_manager.notify(event);
    }
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
                                 10000,
                                 task_manager.is_debug()));

    // Init telegram bot
    task_manager.register_task(Task::create<TelegramManager>(
        TelegramManager::Arguments()
            .set_bot_tocken(bot_tocken)
            .set_authorized_users(authorized_users)
            .set_read_callback([&](const telegramMessage& message) {
                if (task_manager.is_debug())
                    TaskManager::get_instance().log(message.text);
                if (message.text == "/start") {
                    String from_name = message.from_name;
                    String welcome = "Welcome, " + from_name + ".\n";
                    welcome += "Use the following commands to control your outputs.\n\n";
                    welcome += "/status Return status of all available devices\n";
                    welcome += "/relay_on [N] Enable relay pin\n";
                    welcome += "/relay_off [N] Disable relay pin\n";
                    welcome += "/led_timeout N Led timeout in ms";
                    Event event{TelegramManager::static_type_info(),
                                TelegramManager::static_type_info(),
                                {{"type", "send"}, {"chat_id", message.chat_id.c_str()}, {"message", welcome.c_str()}}};
                    TaskManager::get_instance().notify(event);
                } else if (message.text == "/status") {
                    std::string status_str;
                    for (const auto& dev : TaskManager::get_instance().get_devices()) {
                        auto state = dev.status();
                        status_str += "Device: " + state.name + "\n";
                        status_str += "    pins: ";
                        for (const auto& pin : state.used_pins)
                            status_str += String(pin).c_str() + std::string(" ");
                        status_str += "\n";
                        if (state.public_val.size()) {
                            status_str += "    values:\n";
                            for (const auto& param : state.public_val)
                                status_str +=
                                    "        " + param.first + ": " + param.second.first + param.second.second + "\n";
                        }
                        if (state.info.size()) {
                            status_str += "    other info:\n";
                            for (const auto& param : state.info)
                                status_str += "        " + param.first + ": " + param.second + "\n";
                        }
                        status_str += "\n";
                    }
                    Event event{TelegramManager::static_type_info(),
                                TelegramManager::static_type_info(),
                                {{"type", "send"}, {"chat_id", message.chat_id.c_str()}, {"message", status_str}}};
                    TaskManager::get_instance().notify(event);
                } else if (message.text.startsWith("/relay_on")) {
                    Event event{
                        TelegramManager::static_type_info(),
                        RelayDevice::static_type_info(),
                        {{"enable", "on"}, {"pin", message.text.substring(message.text.indexOf(" ") + 1).c_str()}}};
                    TaskManager::get_instance().notify(event);
                } else if (message.text.startsWith("/relay_off")) {
                    Event event{
                        TelegramManager::static_type_info(),
                        RelayDevice::static_type_info(),
                        {{"enable", "off"}, {"pin", message.text.substring(message.text.indexOf(" ") + 1).c_str()}}};
                    TaskManager::get_instance().notify(event);
                } else if (message.text.startsWith("/led_timeout")) {
                    Event event{TelegramManager::static_type_info(),
                                Led::static_type_info(),
                                {{"type", "led_timeout"},
                                 {"val", message.text.substring(message.text.indexOf(" ") + 1).c_str()}}};
                    TaskManager::get_instance().notify(event);
                }
            })
            .set_on_update([](UniversalTelegramBot& bot, const Event& event) {
                TaskManager::get_instance().log(event.to.c_str());
                TaskManager::get_instance().log(event.from.c_str());
                if (event.to != TelegramManager::static_type_info())
                    return;
                if (event.args.find("type") == event.args.end())
                    return;
                if (event.args.at("type") == "send" && event.args.find("chat_id") != event.args.end() &&
                    event.args.find("message") != event.args.end()) {
                    bot.sendMessage(event.args.at("chat_id").c_str(), event.args.at("message").c_str(), "");
                }
            }),
        1000,
        task_manager.is_debug()));

    // Control led on the board
    task_manager.register_task(Task::create<Led>(2, LOW, 5000));

    // Register devices
    task_manager.register_task(Task::create<SoilMoistureDevice>("dev1", 34, 2610, 820));
    task_manager.register_task(
        Task::create<RelayDevice>("relay", std::vector<uint8_t>{23}, true, task_manager.is_debug()));
    task_manager.register_task(Task::create<WaterLevelDevice>("W_level", 39, 1500, 2200));

    task_manager.log("Ready");
    {
        // print
        Event event{"Main",
                    LCDManager::static_type_info(),
                    {{"type", "print"}, {"col", "0"}, {"row", "0"}, {"text", "Ready"}}};
        task_manager.notify(event);
    }

    menu_controller.redraw();
}

void loop() {
    task_manager.execute();
}
