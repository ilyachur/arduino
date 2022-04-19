#include "start_menu.hpp"

#include <event.hpp>
#include <lcd.hpp>
#include <manager.hpp>
#include <map>

#include "main_menu.hpp"

class StartMenuUpdater : public ScheduledTaskImpl {
private:
    const StartMenu* m_menu;

public:
    TASK_ID("StartMenuUpdater");
    StartMenuUpdater(const StartMenu* menu, uint64_t interval) : ScheduledTaskImpl(interval), m_menu(menu) {}

    void start() override {}
    void process() override {
        {
            // Ask devices
            size_t idx(0);
            for (const auto& task : TaskManager::get_instance().get_devices()) {
                const auto status = task.status();
                for (const auto& param : status.public_val)
                    m_menu->m_data[status.name /*  + " " + param.first */] =
                        StartMenu::TaskParam{idx, param.first, param.second.first + param.second.second};
                idx++;
            }
        }
        if (m_menu)
            m_menu->render();
    }
    void end() override {}
};

StartMenu::StartMenu() {
    TaskManager::get_instance().register_task(Task::create<StartMenuUpdater>(this, 5000));
}

StartMenu::~StartMenu() {
    // Kill task
    TaskManager::get_instance().remove_task(StartMenuUpdater::static_type_info());
}
// Render information on the screen
void StartMenu::render() const {
    {
        Event event{"StartMenu", LCDManager::static_type_info(), {{"type", "clear"}}};
        TaskManager::get_instance().notify(event);
    }
    uint8_t screen_size = 2, i = 0;
    size_t count_el = 0;
    for (auto it = m_data.begin(); it != m_data.end() && i < screen_size; it++, count_el++) {
        if (count_el < start_pos) {
            continue;
        }
        std::string line = start_pos + i == current_pos ? ">" : " ";
        line += it->first + ": " + it->second.m_param_val;
        // print
        Event event{"StartMenu",
                    LCDManager::static_type_info(),
                    {{"type", "print"}, {"col", "0"}, {"row", String(i).c_str()}, {"text", line}}};
        TaskManager::get_instance().notify(event);
        i++;
    }
}
// Select current item
View StartMenu::select() {
    size_t count_el = 0;
    for (auto it = m_data.begin(); it != m_data.end(); it++, count_el++) {
        if (count_el != current_pos) {
            continue;
        }
        size_t idx(0);
        for (auto&& const_dev : TaskManager::get_instance().get_devices()) {
            auto& dev = const_cast<Task&>(const_dev);
            if (idx == it->second.m_dev_idx) {
                dev.select(it->second.m_param_name);
            }
            idx++;
        }
        break;
    }
    return View();
}
// Get previous windows
View StartMenu::back() {
    return View::create<MainMenu>();
}
void StartMenu::operator++(int) {
    if (current_pos == 3)
        return;
    current_pos++;
    while (start_pos + 1 < current_pos)
        start_pos += 2;
}
void StartMenu::operator--(int) {
    if (current_pos == 0)
        return;
    current_pos--;
    while (start_pos > current_pos)
        start_pos -= 2;
}
