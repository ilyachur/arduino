#include "lcd.hpp"

LCDManager::LCDManager(int rs, int en, int d4, int d5, int d6, int d7, int w, int h, bool debug)
    : m_lcd(rs, en, d4, d5, d6, d7) {
    m_lcd.begin(w, h);
    set_debug(debug);
}

void LCDManager::set_cursor(const Event& event) {
    int col = String(event.args.at("col").c_str()).toInt();
    int row = String(event.args.at("row").c_str()).toInt();
    m_lcd.setCursor(col, row);
}

void LCDManager::update(const Event& event) {
    if (event.to != type_info() || event.args.find("type") == event.args.end())
        return;
    const auto action_type = event.args.at("type");

    if (action_type == "clear") {
        m_lcd.clear();
    } else if (action_type == "home") {
        m_lcd.home();
    } else if (action_type == "no_display") {
        m_lcd.noDisplay();
    } else if (action_type == "display") {
        m_lcd.display();
    } else if (action_type == "no_blink") {
        m_lcd.noBlink();
    } else if (action_type == "blink") {
        m_lcd.blink();
    } else if (action_type == "no_cursor") {
        m_lcd.noCursor();
    } else if (action_type == "cursor") {
        m_lcd.cursor();
    } else if (action_type == "set_cursor") {
        set_cursor(event);
    } else if (action_type == "print") {
        if (event.args.find("col") != event.args.end() && event.args.find("row") != event.args.end())
            set_cursor(event);
        m_lcd.print(event.args.at("text").c_str());
    }
}
