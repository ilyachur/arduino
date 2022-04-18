#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "event.hpp"

#define TASK_ID(ID)                              \
    static const char* static_type_info() {      \
        static constexpr char* name = (char*)ID; \
        return name;                             \
    }                                            \
    const char* type_info() const override {     \
        return static_type_info();               \
    }

struct Status {
    std::vector<uint8_t> used_pins;
    std::string name;
    std::unordered_map<std::string, std::pair<std::string, std::string>> public_val;
    std::unordered_map<std::string, std::string> info;
};

class TaskImpl {
protected:
    bool in_progress;
    uint64_t last_time;
    bool debug;

public:
    TaskImpl();
    virtual ~TaskImpl();
    virtual bool execute() = 0;
    virtual bool is_finished() = 0;

    virtual void start() {}
    virtual void process() = 0;
    virtual void end() {}
    virtual const char* type_info() const = 0;
    virtual void update(const Event& event) {}

    virtual bool is_debug() const;
    virtual void set_debug(bool enable);
    virtual Status status() const {
        return {};
    }
    virtual bool is_device() const {
        return false;
    }
    virtual bool is_service() const {
        return false;
    }
};

class Task {
    std::shared_ptr<TaskImpl> m_impl;
    Task(const std::shared_ptr<TaskImpl>& implementation) : m_impl(implementation) {}

public:
    bool execute() {
        return m_impl->execute();
    }
    bool is_finished() {
        return m_impl->is_finished();
    }
    bool operator==(const Task& task) {
        return m_impl == task.m_impl;
    }

    void start() {
        m_impl->start();
    }
    void process() {
        m_impl->process();
    }
    void end() {
        m_impl->end();
    }
    const char* type_info() const {
        return m_impl->type_info();
    }

    bool is_debug() const {
        return m_impl->is_debug();
    }
    void set_debug(bool enable) {
        return m_impl->set_debug(enable);
    }
    void update(const Event& event) {
        m_impl->update(event);
    }

    Status status() const {
        return m_impl->status();
    }

    bool is_device() const {
        return m_impl->is_device();
    }

    bool is_service() const {
        return m_impl->is_service();
    }

    template <class T, class... Args>
    static Task create(Args... args) {
        return Task(std::make_shared<T>(args...));
    }
};

class Service : public TaskImpl {
public:
    Service() = default;
    bool is_finished() override {
        return false;
    }
    void process() override {}
    bool execute() override {
        return false;
    }
    bool is_service() const override {
        return true;
    }
};

class Device : public Service {
public:
    Device() = default;
    bool is_service() const override {
        return false;
    }
    bool is_device() const override {
        return true;
    }
};

/**
 * Scheduled task, like WifiManager or EmailManager
 */
class ScheduledTaskImpl : public TaskImpl {
protected:
    uint64_t duration;
    uint64_t interval;

public:
    ScheduledTaskImpl(uint64_t interval, uint64_t duration = 0);
    bool execute() override;
    bool is_finished() override;

    void set_interval(uint64_t inter) {
        interval = inter;
    }
};

/**
 * One time task, like get report
 */
class UniqueTaskImpl : public TaskImpl {
private:
    uint64_t duration;

public:
    UniqueTaskImpl(uint64_t duration = 0);
    bool execute() override;
    bool is_finished() override;
};

