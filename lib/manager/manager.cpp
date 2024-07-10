#include "manager.hpp"

using namespace lib;

void Manager::add(std::string username, std::string password, eLoginMethod method)
{
  this->logger = spdlog::stdout_color_mt(username);
  logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");

  bots.push_back(std::make_unique<Bot>(username, password, method, logger));
}

void Manager::get(std::string username)
{
  for (auto &bot : bots)
  {
    if (bot->username == username)
    {
      return bot.get();
    }
  }
  return nullptr;
}

void Manager::remove(std::string username)
{
  auto it = std::find_if(bots.begin(), bots.end(),
                         [&username](const std::unique_ptr<Bot> &bot)
                         { return bot->username == username; });
  if (it != bots.end())
  {
    bots.erase(it);
  }
}