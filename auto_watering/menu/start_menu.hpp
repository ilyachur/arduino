#pragma once

#include <event.hpp>
#include <lcd.hpp>
#include <manager.hpp>
#include <task.hpp>

#include "main_menu.hpp"
#include "view.hpp"

class StartMenuUpdater : public ScheduledTaskImpl {
private:
    const View::Impl* m_menu;

public:
    TASK_ID("StartMenuUpdater");
    StartMenuUpdater(const View::Impl* menu, uint64_t interval) : ScheduledTaskImpl(interval), m_menu(menu) {}

    void start() override {}
    void process() override {
        if (m_menu)
            m_menu->render();
    }
    void end() override {}
};

class StartMenu : public View::Impl {
public:
    StartMenu() {
        TaskManager::get_instance().register_task(Task::create<StartMenuUpdater>(this, 5000));
    }
    ~StartMenu() {
        // Kill task
        TaskManager::get_instance().remove_task(StartMenuUpdater::static_type_info());
    }
    // Render information on the screen
    void render() const override {
        {
            Event event{"StartMenu", LCDManager::static_type_info(), {{"type", "clear"}}};
            TaskManager::get_instance().notify(event);
        }
        {
            // print
            Event event{"StartMenu",
                        LCDManager::static_type_info(),
                        {{"type", "print"}, {"col", "0"}, {"row", "0"}, {"text", "Temperature:"}}};
            TaskManager::get_instance().notify(event);
        }
        {
            // print
            Event event{"StartMenu",
                        LCDManager::static_type_info(),
                        {{"type", "print"}, {"col", "0"}, {"row", "1"}, {"text", "Humidity:"}}};
            TaskManager::get_instance().notify(event);
        }
    }
    // Select current item
    View select() override {
        // Nothing to do
        return View();
    }
    // Get previous windows
    View back() override {
        return View::create<MainMenu>();
    }
    void operator++(int) override {
        // Nothing to do
    }
    void operator--(int) override {
        // Nothing to do
    }
};
