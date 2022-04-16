#include "encoder.hpp"

#include <Arduino.h>
#include <FunctionalInterrupt.h>

void Encoder::update_state() {
    if (m_pins.size() == 2)
        return;

    uint64_t now = millis();
    uint64_t delta = now > m_timer ? now - m_timer : 0;
    bool hold_button = digitalRead(m_pins[2]) ^ (m_btn_type == PullType::UP);

    m_state.was_holded = false;
    m_state.is_released = false;
    if (m_state.is_pressed && delta > m_holdTimeout) {
        m_state.is_holded = true;
    }

    if (m_state.is_pressed != hold_button) {
        if (hold_button) {
            m_state.is_pressed = true;
            m_state.is_clicked = false;
            m_state.is_holded = false;
            m_state.was_turned = false;
        } else {
            m_state.is_released = true;
            m_state.is_pressed = false;
            if (m_state.is_holded)
                m_state.was_holded = true;
            if (!m_state.was_turned) {
                m_state.is_clicked = true;
            }
            m_state.is_holded = false;
        }
    }
    m_timer = now;
}

const std::function<void()>& Encoder::get_encoder_callback() {
    static std::function<void()> internal_enc_change = [&]() {
        noInterrupts();
        update_state();
        bool current_state = digitalRead(m_pins[0]);
        if (current_state != m_rotate_state.get_state()) {
            bool left = digitalRead(m_pins[1]) != current_state;
            if (left)
                m_rotate_state--;
            else
                m_rotate_state++;
            if (m_rotate_state.apply()) {
                m_state.was_turned = true;
                State state = m_state;
                if (left)
                    state.left = true;
                else
                    state.right = true;
                m_encoder_callback(state);
            }
        }
        interrupts();
    };
    return internal_enc_change;
}

Encoder::Encoder(uint8_t s1,
                 uint8_t s2,
                 const encoder_callback& enc_change,
                 bool inverted,
                 uint64_t timeout,
                 const Type& type,
                 const PullType& enc_mode)
    : m_holdTimeout(timeout),
      m_rotate_state(type),
      m_pins(2),
      m_encoder_callback(enc_change) {
    m_pins[0] = inverted ? s2 : s1;
    m_pins[1] = inverted ? s1 : s2;

    pinMode(m_pins[0], (enc_mode != PullType::UP) ? INPUT : INPUT_PULLUP);
    pinMode(m_pins[1], (enc_mode != PullType::UP) ? INPUT : INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(m_pins[0]), get_encoder_callback(), CHANGE);
}

Encoder::Encoder(uint8_t s1,
                 uint8_t s2,
                 uint8_t key,
                 const encoder_callback& enc_change,
                 const encoder_callback& enc_button,
                 bool inverted,
                 uint64_t timeout,
                 const Type& type,
                 const PullType& enc_mode,
                 const PullType& btn_mode)
    : m_holdTimeout(timeout),
      m_rotate_state(type),
      m_pins(3),
      m_btn_type(btn_mode),
      m_button_callback(enc_button),
      m_encoder_callback(enc_change) {
    m_pins[0] = inverted ? s2 : s1;
    m_pins[1] = inverted ? s1 : s2;
    m_pins[2] = key;

    pinMode(m_pins[0], (enc_mode != PullType::UP) ? INPUT : INPUT_PULLUP);
    pinMode(m_pins[1], (enc_mode != PullType::UP) ? INPUT : INPUT_PULLUP);
    pinMode(m_pins[2], (btn_mode != PullType::UP) ? INPUT : INPUT_PULLUP);
    std::function<void()> internal_enc_button = [&]() {
        noInterrupts();
        update_state();
        m_button_callback(m_state);
        interrupts();
    };

    attachInterrupt(digitalPinToInterrupt(m_pins[0]), get_encoder_callback(), CHANGE);
    attachInterrupt(digitalPinToInterrupt(m_pins[2]), internal_enc_button, CHANGE);
}

Encoder::~Encoder() {
    detachInterrupt(digitalPinToInterrupt(m_pins[0]));
    if (m_pins.size() > 2)
        detachInterrupt(digitalPinToInterrupt(m_pins[2]));
}
