#pragma once

#include <map>
#include <string>
#include <task.hpp>
#include <utility>

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
    struct TaskParam {
        size_t m_dev_idx;
        std::string m_param_name;
        std::string m_param_val;
    };
    friend StartMenuUpdater;
    mutable std::map<std::string, TaskParam> m_data;

    size_t start_pos = 0;
    size_t current_pos = 0;
};
