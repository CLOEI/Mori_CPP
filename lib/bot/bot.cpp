#include "bot.hpp"
#include <enet/enet.h>

using namespace lib;

Bot::Bot(const std::string &username, const std::string &password, eLoginMethod method, spdlog::logger &logger)
{
  this->username = username;
  this->password = password;
  this->method = method;
  this->logger = logger;
}

void lib::Bot::disconnect()
{
  enet_peer_disconnect(enet_peer, 0);
}

void lib::Bot::spoof()
{
}
