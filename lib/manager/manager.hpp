#pragma once

#include <vector>
#include <string>

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
    std::vector<int> bots;
  };
}