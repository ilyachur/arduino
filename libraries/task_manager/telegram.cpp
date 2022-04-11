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
        if (m_args.m_authorized_users.find(chat_id.c_str()) == m_args.m_authorized_users.end()) {
            m_bot.sendMessage(chat_id, "Unauthorized user " + chat_id, "");
            continue;
        }
        m_args.m_message_callback(m_bot.messages[i]);
    }
}

TelegramManager::TelegramManager(const Arguments& args, uint64_t interval, bool debug)
    : ScheduledTaskImpl(interval),
      m_client(),
      m_args(args),
      m_bot(m_args.m_bot_tocken.c_str(), m_client) {
    m_client.setInsecure();
    set_debug(debug);
}

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

void TelegramManager::update(const Event& event) {
    m_args.m_on_update(m_bot, event);
}
