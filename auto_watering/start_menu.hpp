#pragma once

#include "view.hpp"

class StartMenu : public View::Impl {
public:
    StartMenu();
    ~StartMenu();
    // Render information on the screen
    void render() const override;
    // Select current item
    View select() override;
    // Get previous windows
    View back() override;
    void operator++(int) override;
    void operator--(int) override;
};
