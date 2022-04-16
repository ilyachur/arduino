#pragma once

#include <Preferences.h>

#include <list>
#include <memory>

#include "task.hpp"

/**
 * Three types of commands:
 *  * Scheduled tasks (Examples: check email, check wifi)
 *  * EMail tasks
 *  * Devices tasks
 */
class TaskManager {
private:
    TaskManager();
    TaskManager(const TaskManager&) = delete;
    // debug mode
    bool debug;
    // Store all information about commands
    Preferences preferences;

    std::list<Task> tasks;
    static std::unique_ptr<TaskManager> manager;

public:
    static TaskManager& get_instance();
    void register_task(const Task& task);
    void remove_task(const Task& task);
    void remove_task(const char* type_info);
    void set_debug(bool enable);
    bool is_debug();
    void execute();

    void log(String message = "", bool withoutNewLine = false);
    void log(const Task& task, String message = "", bool withoutNewLine = false);
    void log(const TaskImpl* task, String message = "", bool withoutNewLine = false);
    void notify(const Event& event);
};

