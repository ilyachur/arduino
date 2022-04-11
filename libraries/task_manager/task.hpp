#pragma once

#include <memory>
#include <string>

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

    virtual void start() = 0;
    virtual void process() = 0;
    virtual void end() = 0;
    virtual const char* name() const = 0;

    virtual bool is_debug() const;
    virtual void set_debug(bool enable);
};

class Task {
    std::shared_ptr<TaskImpl> impl;
    Task(const std::shared_ptr<TaskImpl>& implementation): impl(implementation) {}

public:
    bool execute() {
        return impl->execute();
    }
    bool is_finished() {
        return impl->is_finished();
    }
    bool operator==(const Task& task) {
        return impl == task.impl;
    }

    void start() {
        impl->start();
    }
    void process() {
        impl->process();
    }
    void end() {
        impl->end();
    }
    const char* name() const {
        return impl->name();
    }

    bool is_debug() const {
        return impl->is_debug();
    }
    void set_debug(bool enable) {
        return impl->set_debug(enable);
    }
    template <class T, class ... Args>
    static Task create(Args ... args) {
      return Task(std::make_shared<T>(args ...));
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

