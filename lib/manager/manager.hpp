#pragma once

#include <vector>
#include <string>
#include <spdlog/spdlog.h>
#include "lib/bot/bot.hpp"

namespace lib
{
  enum eLoginMethod
  {
    APPLE_SIGNIN,
    GOOGLE_SIGNIN,
    LEGACY_SIGNIN,
  };

  class Manager
  {
  public:
    void add(std::string username, std::string password, eLoginMethod method);
    void get(std::string username);
    void remove(std::string username);

  public:
    std::vector<std::unique_ptr<Bot>> bots;
    spdlog::logger logger;
  };
}