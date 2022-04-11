#include "telegram.hpp"

#include <WiFiClientSecure.h>

#include <string>
#include <vector>

#include "manager.hpp"

// Handle new receive messages
void TelegramManager::handle_new_messages(int num_messages) {
    if (is_debug()) {
        TaskManager::get_instance().log(this, "Handle new messages " + String(num_messages));
    }
    for (int i = 0; i < num_messages; i++) {
        // Chat id of the requester
        String chat_id = String(m_bot.messages[i].chat_id);
        if (m_authorized_users.find(chat_id.c_str()) == m_authorized_users.end()) {
            m_bot.sendMessage(chat_id, "Unauthorized user " + chat_id, "");
            continue;
        }
        // Print the received message
        String text = m_bot.messages[i].text;
        if (is_debug())
            TaskManager::get_instance().log(this, text);
        String from_name = m_bot.messages[i].from_name;
        if (text == "/start") {
            String welcome = "Welcome, " + from_name + ".\n";
            welcome += "Use the following commands to control your outputs.\n\n";
            m_bot.sendMessage(chat_id, welcome, "");
        }
    }
}

TelegramManager::TelegramManager(const String& bot_tocken,
                                 const std::unordered_set<std::string>& authorized_users,
                                 uint64_t interval,
                                 bool debug)
    : ScheduledTaskImpl(interval),
      m_client(),
      m_bot(bot_tocken, m_client),
      m_authorized_users(authorized_users) {
    m_client.setInsecure();
    set_debug(debug);
}

void TelegramManager::start() {}
void TelegramManager::process() {
    TaskManager::get_instance().log(this, m_bot.name);
    TaskManager::get_instance().log(this, m_bot.userName);
    int num_messages = m_bot.getUpdates(m_bot.last_message_received + 1);
    TaskManager::get_instance().log(this, String(num_messages));
    while (num_messages) {
        if (is_debug())
            TaskManager::get_instance().log(this, "got response");
        handle_new_messages(num_messages);
        num_messages = m_bot.getUpdates(m_bot.last_message_received + 1);
    }
}
void TelegramManager::end() {}

const char* TelegramManager::name() const {
    static constexpr char* name = (char*)"TelegramManager";
    return name;
}

void TelegramManager::set_interval(uint64_t interval) {
    this->interval = interval;
}
