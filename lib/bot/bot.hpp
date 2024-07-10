#pragma once

#include <string>
#include <spdlog/spdlog.h>
#include "connect/connect.hpp"
#include "types/eLoginMethod.hpp"

namespace lib {
    class Bot : public Connect {
    public:
        Bot(const std::string &username, const std::string &password, types::eLoginMethod method,
            std::shared_ptr<spdlog::logger> logger);

        void disconnect();

    private:
        void spoof();

    public:
        bool is_ingame = false;
        std::shared_ptr<spdlog::logger> logger;
        std::string username;
        std::string password;
        types::eLoginMethod method;
    };
}