#pragma once

#include <encoder.hpp>
#include <manager.hpp>

#include "start_menu.hpp"
#include "view.hpp"

class MenuController {
private:
    View m_view;
    Encoder m_encoder;
    bool debug;

    const std::function<void(const Encoder::State&)>& menu_switch() {
        static std::function<void(const Encoder::State&)> callback = [&](const Encoder::State& state) {
            if (state.right)
                m_view++;
            else if (state.left)
                m_view--;
            m_view.render();
        };
        return callback;
    }
    const std::function<void(const Encoder::State&)>& menu_click() {
        static std::function<void(const Encoder::State&)> callback = [&](const Encoder::State& state) {
            if (state.was_holded && state.is_clicked) {
                auto new_view = m_view.back();
                if (!new_view.empty())
                    m_view = new_view;
            } else if (state.is_clicked) {
                auto new_view = m_view.select();
                if (!new_view.empty())
                    m_view = new_view;
            }
        };
        return callback;
    }

public:
    struct EncoderAttributes {
        uint8_t s1;
        uint8_t s2;
        uint8_t key;
    };
    MenuController(const EncoderAttributes& enc, bool debug = false)
        : m_encoder(enc.s1, enc.s2, enc.key, menu_switch(), menu_click()) {
        // Load start page
        m_view = View::create<StartMenu>();
    }

    void redraw() {
        m_view.render();
    }
};
