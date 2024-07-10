#pragma once

#include <string>
#include <spdlog/spdlog.h>
#include "lib/connect/connect.hpp"

namespace lib
{
  class Bot : Connect
  {
  public:
    Bot(const std::string &username, const std::string &password, eLoginMethod method, spdlog::logger &logger);
    void disconnect();

  private:
    void spoof();

  public:
    bool is_ingame = false;
    spdlog::logger &logger;
    std::string &username;
    std::string &password;
    eLoginMethod method;
  }
}