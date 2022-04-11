#pragma once

#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include <Wifi.h>

#include <unordered_set>

#include "task.hpp"

class TelegramManager : public ScheduledTaskImpl {
private:
    WiFiClientSecure m_client;
    UniversalTelegramBot m_bot;
    std::unordered_set<std::string> m_authorized_users;
    void handle_new_messages(int num_messages);

public:
    TelegramManager(const String& bot_tocken,
                    const std::unordered_set<std::string>& authorized_users,
                    uint64_t interval = 1000,
                    bool debug = false);
    void start() override;
    void process() override;
    void end() override;
    const char* name() const override;

    void set_interval(uint64_t interval);
};

