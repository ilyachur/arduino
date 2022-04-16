#pragma once
#include <memory>

class View {
public:
    class Impl {
    public:
        // Render information on the screen
        virtual void render() const = 0;
        // Select current item
        virtual View select() = 0;
        // Get previous windows
        virtual View back();
        // Increment menu item
        virtual void operator++(int) = 0;
        virtual void operator--(int) = 0;
    };

    template <class T, class... Args>
    static View create(Args... args) {
        return View(std::make_shared<T>(args...));
    }

    void render() const {
        m_impl->render();
    }

    View select() {
        return m_impl->select();
    }

    View back() {
        return m_impl->back();
    }

    View() = default;

    bool empty() {
        return m_impl != nullptr;
    }

private:
    std::shared_ptr<Impl> m_impl;
    View(const std::shared_ptr<Impl>& impl) : m_impl(impl) {
        m_impl->render();
    }
};
