#pragma once

#include <enet/enet.h>
#include <memory>
#include <spdlog/spdlog.h>
#include "types/eLoginMethod.hpp"

namespace lib
{
    class Connect
    {
    public:
        explicit Connect(std::shared_ptr<spdlog::logger> logger)
        {
            this->logger = std::move(logger);
        }
        std::string get_token(std::string &username, std::string &password, types::eLoginMethod method);
        static void get_oauth_link();

    private:
        std::string get_token_legacy(std::string &username, std::string &password);
        std::string get_token_google(std::string &username, std::string &password);
        std::string get_token_apple(std::string &username, std::string &password);
        std::string get_legacy_form_token();

    public:
        std::unique_ptr<ENetHost> enet_host{nullptr};
        std::unique_ptr<ENetPeer> enet_peer{nullptr};
        std::shared_ptr<spdlog::logger> logger;

    public:
        static std::string user_agent;
        static std::string legacy_login_url;
        static std::string google_login_url;
        static std::string apple_login_url;
    };
}