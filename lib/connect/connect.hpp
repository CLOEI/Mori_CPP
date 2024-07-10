#pragma once

#include <enet/enet.h>
#include <memory>
#include <spdlog/spdlog.h>
#include "types/eLoginMethod.hpp"

namespace lib {
    class Connect {
    public:
        explicit Connect(std::shared_ptr<spdlog::logger> logger)
        {
            this->logger = std::move(logger);
        }
        void get_token(std::string &username, std::string &password, types::eLoginMethod method);

    public:
        std::unique_ptr<ENetHost> enet_host{nullptr};
        std::unique_ptr<ENetPeer> enet_peer{nullptr};
        std::shared_ptr<spdlog::logger> logger;
    };
}