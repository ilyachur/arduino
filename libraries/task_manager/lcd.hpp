#pragma once

#include <LiquidCrystal.h>

#include "task.hpp"

class LCDManager : public ServiceTaskImpl {
private:
    // Create An LCD Object. Signals: [ RS, EN, D4, D5, D6, D7 ]
    LiquidCrystal m_lcd;

    void set_cursor(const Event& event);

public:
    TASK_ID("LCDManager");
    LCDManager(int rs, int en, int d4, int d5, int d6, int d7, int w, int h, bool debug = false);
    void update(const Event& event) override;
};
