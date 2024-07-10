#include "manager.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"

using namespace lib;

void Manager::add(std::string username, std::string password, types::eLoginMethod method) {
    this->logger = spdlog::stdout_color_mt(username);
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");

    logger->info("Adding bot");
    bots.push_back(std::make_unique<Bot>(username, password, method, logger));
}

Bot *Manager::get(std::string username) {
    logger->info("Getting bot info");
    for (auto &bot: bots) {
        if (bot->username == username) {
            return bot.get();
        }
    }
    return nullptr;
}

void Manager::remove(std::string username) {
    logger->info("Removing bot");
    auto it = std::find_if(bots.begin(), bots.end(),
                           [&username](const std::unique_ptr<Bot> &bot) { return bot->username == username; });
    if (it != bots.end()) {
        bots.erase(it);
    }
}