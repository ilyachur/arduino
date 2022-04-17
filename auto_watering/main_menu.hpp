#pragma once

#include <array>

#include "view.hpp"

class MainMenu : public View::Impl {
private:
    uint8_t start_pos = 0;
    uint8_t current_pos = 0;
    std::array<MenuItem, 4> menu_items;

public:
    MainMenu();
    ~MainMenu();
    // Render information on the screen
    void render() const override;
    // Select current item
    View select() override;
    // Get previous windows
    View back() override;
    void operator++(int) override;
    void operator--(int) override;
};
