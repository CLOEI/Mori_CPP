#pragma once

#include <vector>
#include <string>
#include <spdlog/spdlog.h>
#include <memory>
#include "bot/bot.hpp"
#include "types/eLoginMethod.hpp"

namespace lib {
    class Manager {
    public:
        void add(std::string username, std::string password, types::eLoginMethod method);

        Bot *get(std::string username);

        void remove(std::string username);

    public:
        std::vector<std::unique_ptr<Bot>> bots;
        std::shared_ptr<spdlog::logger> logger;
    };
}