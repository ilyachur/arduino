#include "start_menu.hpp"

#include <event.hpp>
#include <lcd.hpp>
#include <manager.hpp>
#include <map>
#include <task.hpp>

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
            Event event;
            event.from = "StartMenuUpdater";
            event.args["get"] = "status";
            TaskManager::get_instance().notify(event);
        }
        if (m_menu)
            m_menu->render();
    }
    void end() override {}

    void update(const Event& event) override {
        if (event.args.find("status") != event.args.end() && event.args.find("name") != event.args.end() &&
            event.args.find("params") != event.args.end()) {
            // TODO: Split params
            std::string param = event.args.at("params");
            m_menu->m_data[event.args.at("name")] = event.args.at(param + "_val") + event.args.at(param + "_type");
        }
    }
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
        line += it->first + ": " + it->second;
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
    // Nothing to do
    return View();
}
// Get previous windows
View StartMenu::back() {
    return View::create<MainMenu>();
}
void StartMenu::operator++(int) {
    // Nothing to do
}
void StartMenu::operator--(int) {
    // Nothing to do
}
