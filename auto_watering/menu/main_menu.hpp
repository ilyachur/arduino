#pragma once

#include <Arduino.h>

#include <array>
#include <event.hpp>
#include <lcd.hpp>
#include <manager.hpp>
#include <string>

#include "view.hpp"

class MainMenu : public View::Impl {
private:
    uint8_t start_pos = 0;
    uint8_t current_pos = 0;
    std::array<std::string, 4> menu_items;

public:
    MainMenu() {
        menu_items[0] = "Sensors";
        menu_items[1] = "Sensors1";
        menu_items[2] = "Sensors2";
        menu_items[3] = "Sensors3";
    }
    ~MainMenu() {
        // Kill task
    }
    // Render information on the screen
    void render() const override {
        {
            Event event{"MainMenu", LCDManager::static_type_info(), {{"type", "clear"}}};
            TaskManager::get_instance().notify(event);
        }
        for (uint8_t i = 0; i < 2; i++) {
            std::string line = start_pos + i == current_pos ? "> " : "";
            line += menu_items[start_pos + i];
            // print
            Event event{"MainMenu",
                        LCDManager::static_type_info(),
                        {{"type", "print"}, {"col", "0"}, {"row", String(i).c_str()}, {"text", line}}};
            TaskManager::get_instance().notify(event);
        }
    }
    // Select current item
    View select() override {
        // CycleDep
        // if (current_pos == 0)
        //     return View::create<StartMenu>();
        return View();
    }
    // Get previous windows
    View back() override {
        return View();
    }
    void operator++(int) override {
        // Nothing to do
    }
    void operator--(int) override {
        // Nothing to do
    }
};
