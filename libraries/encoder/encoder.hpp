#pragma once

#include <functional>
#include <utility>
#include <vector>

class Encoder {
public:
    enum class PullType : bool { DOWN = 0, UP = 1 };
    enum class Type : bool {
        ONE = 0,    // One impulse
        DOUBLE = 1  // Two impulses
    };

    struct State {
        bool left = false;
        bool right = false;
        bool is_pressed = false;
        bool is_released = false;
        bool is_holded = false;

        bool is_clicked = false;
        bool was_holded = false;
        bool was_turned = false;
    };
    using encoder_callback = std::function<void(const State&)>;

    Encoder(uint8_t s1,
            uint8_t s2,
            uint8_t key,
            const encoder_callback& enc_change,
            const encoder_callback& enc_button,
            bool inverted = false,
            uint64_t timeout = 700,
            const Type& type = Type::DOUBLE,
            const PullType& enc_mode = PullType::UP,
            const PullType& btn_mode = PullType::UP);

    Encoder(uint8_t s1,
            uint8_t s2,
            const encoder_callback& enc_change,
            bool inverted = false,
            uint64_t timeout = 700,
            const Type& type = Type::DOUBLE,
            const PullType& enc_mode = PullType::UP);

    ~Encoder();

private:
    class RotateState {
        uint8_t m_state = 0;
        uint8_t up = 0;
        uint8_t down = 0;
        Type type;

    public:
        RotateState(const Type& t) : type(t) {}
        void operator++(int) {
            up++;
        }
        void operator--(int) {
            down++;
        }

        void set_state(uint8_t state) {
            m_state = state;
        }

        uint8_t get_state() {
            return m_state;
        }

        bool apply() {
            if (type == Type::ONE || up == 2 || down == 2) {
                up = 0;
                down = 0;
                return true;
            }
            return false;
        }
    };
    uint64_t m_holdTimeout;
    State m_state;
    RotateState m_rotate_state;
    uint64_t m_timer;
    std::vector<uint8_t> m_pins;
    PullType m_btn_type;
    encoder_callback m_button_callback;
    encoder_callback m_encoder_callback;

    const std::function<void()>& get_encoder_callback();
    void update_state();
};
