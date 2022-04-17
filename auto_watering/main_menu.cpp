#include "main_menu.hpp"

#include <event.hpp>
#include <lcd.hpp>
#include <manager.hpp>

#include "start_menu.hpp"

MainMenu::MainMenu() {
    menu_items[0] = MenuItem("Sensors", []() {
        return View::create<StartMenu>();
    });
    menu_items[1] = {"Sensors1"};
    menu_items[2] = {"Sensors2"};
    menu_items[3] = {"Sensors3"};
}
MainMenu::~MainMenu() {
    // Kill task
}
// Render information on the screen
void MainMenu::render() const {
    {
        Event event{"MainMenu", LCDManager::static_type_info(), {{"type", "clear"}}};
        TaskManager::get_instance().notify(event);
    }
    for (uint8_t i = 0; i < 2; i++) {
        std::string line = start_pos + i == current_pos ? ">" : " ";
        line += menu_items[start_pos + i].title();
        // print
        Event event{"MainMenu",
                    LCDManager::static_type_info(),
                    {{"type", "print"}, {"col", "0"}, {"row", String(i).c_str()}, {"text", line}}};
        TaskManager::get_instance().notify(event);
    }
}
// Select current item
View MainMenu::select() {
    // CycleDep
    return menu_items[current_pos]();
}
// Get previous windows
View MainMenu::back() {
    return View();
}
void MainMenu::operator++(int) {
    if (current_pos == 3)
        return;
    current_pos++;
    while (start_pos + 1 < current_pos)
        start_pos += 2;
}
void MainMenu::operator--(int) {
    if (current_pos == 0)
        return;
    current_pos--;
    while (start_pos > current_pos)
        start_pos -= 2;
}
