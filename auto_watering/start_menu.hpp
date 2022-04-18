#pragma once

#include <map>
#include <string>

#include "view.hpp"

class StartMenuUpdater;

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

private:
    friend StartMenuUpdater;
    mutable std::map<std::string, std::string> m_data;

    size_t start_pos = 0;
    size_t current_pos = 0;
};
