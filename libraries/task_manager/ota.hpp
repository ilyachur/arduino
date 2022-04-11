#pragma once

#include <ArduinoOTA.h>

#include <functional>

#include "task.hpp"

class OTAManager : public ScheduledTaskImpl {
private:
    bool m_initialized;

    bool init();

public:
    class Arguments {
    private:
        std::string m_hostname;
        std::string m_pass;
        std::string m_pass_hash;
        std::string m_p_label;
        uint16_t m_port = 0;
        bool m_reboot = true;
        std::function<void()> m_on_start;
        std::function<void()> m_on_end;
        std::function<void(unsigned int, unsigned int)> m_on_progress;
        std::function<void(ota_error_t)> m_on_error;

        friend OTAManager;

    public:
        Arguments() = default;

        Arguments& set_hostname(const std::string& val) {
            m_hostname = val;
            return *this;
        }

        Arguments& set_password(const std::string& val) {
            m_pass = val;
            return *this;
        }

        Arguments& set_password_hash(const std::string& val) {
            m_pass_hash = val;
            return *this;
        }

        Arguments& set_partition_label(const std::string& val) {
            m_p_label = val;
            return *this;
        }

        Arguments& set_port(uint16_t val) {
            m_port = val;
            return *this;
        }

        Arguments& set_reboot_on_success(bool val) {
            m_reboot = val;
            return *this;
        }

        Arguments& set_on_start(const std::function<void()>& val) {
            m_on_start = val;
            return *this;
        }

        Arguments& set_on_end(const std::function<void()>& val) {
            m_on_end = val;
            return *this;
        }

        Arguments& set_on_progress(const std::function<void(unsigned int, unsigned int)>& val) {
            m_on_progress = val;
            return *this;
        }

        Arguments& set_on_error(const std::function<void(ota_error_t)>& val) {
            m_on_error = val;
            return *this;
        }
    };
    OTAManager(const Arguments& args, uint64_t interval = 1000, bool debug = false);
    void start() override;
    void process() override;
    void end() override;
    const char* name() const override;

    void set_update_interval(uint64_t interval);
};

