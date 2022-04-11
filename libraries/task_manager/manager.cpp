#include "manager.hpp"

std::unique_ptr<TaskManager> TaskManager::manager;

TaskManager::TaskManager(): debug(false) {}

TaskManager& TaskManager::get_instance() {
    if (!manager) {
        manager.reset(new TaskManager());
    }
    return *manager;
}

void TaskManager::set_debug(bool enable) {
    debug = enable;
}
bool TaskManager::is_debug() {
    return debug;
}

void TaskManager::register_task(const Task& task) {
    tasks.emplace_back(task);
}

void TaskManager::remove_task(const Task& task) {
    tasks.remove_if([&](Task& existed_task) {
        return existed_task == task;
    });
}
void TaskManager::execute() {
    tasks.remove_if([](Task& task) {
        task.execute();
        return task.is_finished();
    });
}

void TaskManager::log(String message, bool withoutNewLine) {
    if (is_debug()) {
        String finalMessage = "[TaskManager] ";
        finalMessage += message;
        if (withoutNewLine)
            Serial.print(finalMessage);
        else
            Serial.println(finalMessage);
    }
}

void TaskManager::log(const Task& task, String message, bool withoutNewLine) {
    if (task.is_debug()) {
        String finalMessage = "[";
        finalMessage += String(task.name());
        finalMessage += "] ";
        finalMessage += message;
        if (withoutNewLine)
            Serial.print(finalMessage);
        else
            Serial.println(finalMessage);
    }
}

void TaskManager::log(const TaskImpl* task, String message, bool withoutNewLine) {
    if (task->is_debug()) {
        String finalMessage = "[";
        finalMessage += String(task->name());
        finalMessage += "] ";
        finalMessage += message;
        if (withoutNewLine)
            Serial.print(finalMessage);
        else
            Serial.println(finalMessage);
    }
}

