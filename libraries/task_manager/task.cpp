#include "task.hpp"

#include <Arduino.h>

#include "manager.hpp"

TaskImpl::TaskImpl() : in_progress(false), last_time(-1), debug(false) {}
TaskImpl::~TaskImpl() {}

bool TaskImpl::is_debug() const {
    return debug;
}
void TaskImpl::set_debug(bool enable) {
    debug = enable;
}

ScheduledTaskImpl::ScheduledTaskImpl(uint64_t interval, uint64_t duration) : duration(duration), interval(interval) {}

bool ScheduledTaskImpl::execute() {
    uint64_t current_time = millis();
    if (in_progress) {
        if (current_time - last_time < duration && current_time > last_time) {
            process();
            return true;
        }
        last_time = current_time;
        in_progress = false;
        end();
        return true;
    }
    if (current_time - last_time < interval && current_time >= last_time)
        return false;
    in_progress = true;
    start();
    process();
    return true;
}
bool ScheduledTaskImpl::is_finished() {
    return false;
}

UniqueTaskImpl::UniqueTaskImpl(uint64_t duration) : duration(duration) {}

bool UniqueTaskImpl::execute() {
    uint64_t current_time = millis();
    if (in_progress) {
        if (current_time - last_time < duration && current_time > last_time) {
            process();
            return true;
        }
        last_time = current_time;
        in_progress = false;
        end();
        return true;
    }
    in_progress = true;
    start();
    process();
    return true;
}
bool UniqueTaskImpl::is_finished() {
    return !in_progress;
}

void DriverServiceImpl::update(const Event& event) {
    if (event.to.empty() && event.args.size() == 1 && event.args.find("get") != event.args.end() &&
        event.args.at("get") == "status")
        TaskManager::get_instance().notify(status());
}
