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
        if (!empty())
            m_impl->render();
    }

    View select() {
        if (!empty())
            return m_impl->select();
        return View();
    }

    View back() {
        if (!empty())
            return m_impl->back();
        return View();
    }

    View() = default;

    bool empty() const {
        return m_impl == nullptr;
    }

private:
    std::shared_ptr<Impl> m_impl;
    View(const std::shared_ptr<Impl>& impl) : m_impl(impl) {
        m_impl->render();
    }
};
