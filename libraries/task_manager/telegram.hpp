#pragma once

#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include <Wifi.h>

#include <functional>
#include <unordered_set>

#include "task.hpp"

class TelegramManager : public ScheduledTaskImpl {
public:
    TASK_ID("TelegramManager");
    class Arguments {
    private:
        friend TelegramManager;
        std::string m_bot_tocken;
        std::unordered_set<std::string> m_authorized_users;
        std::function<void(const telegramMessage&)> m_message_callback;
        std::function<void(UniversalTelegramBot&, const Event&)> m_on_update;

    public:
        Arguments() = default;

        Arguments& set_bot_tocken(const std::string& tocken) {
            m_bot_tocken = tocken;
            return *this;
        }

        Arguments& set_authorized_users(const std::unordered_set<std::string>& users) {
            m_authorized_users = users;
            return *this;
        }

        Arguments& set_read_callback(std::function<void(const telegramMessage& message)> callback) {
            m_message_callback = callback;
            return *this;
        }

        Arguments& set_on_update(std::function<void(UniversalTelegramBot&, const Event&)> callback) {
            m_on_update = callback;
            return *this;
        }
    };

    TelegramManager(const Arguments& args, uint64_t interval = 1000, bool debug = false);
    void process() override;
    void update(const Event& event) override;

private:
    WiFiClientSecure m_client;
    Arguments m_args;
    UniversalTelegramBot m_bot;
    void handle_new_messages(int num_messages);
};

