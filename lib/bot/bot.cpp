#include "bot.hpp"
#include <enet/enet.h>

#include <utility>

using namespace lib;

Bot::Bot(const std::string &username, const std::string &password, types::eLoginMethod method,
         std::shared_ptr<spdlog::logger> logger) : Connect(logger)
{
    this->username = username;
    this->password = password;
    this->method = method;
    this->logger = std::move(logger);

    Bot::get_token(this->username, this->password, this->method);
}

void Bot::disconnect()
{
    enet_peer_disconnect(enet_peer.get(), 0);
}

void Bot::spoof()
{
}
