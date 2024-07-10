#pragma once

#include <enet/enet.h>
#include <memory>

namespace lib
{
  class Connect
  {
  public:
    void get_token(std::string &username, std::string &password, eLoginMethod method);

  public:
    std::unique_ptr<ENetHost> enet_host{nullptr};
    std::unique_ptr<ENetPeer> enet_peer{nullptr};
  }
}