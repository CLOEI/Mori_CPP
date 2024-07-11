#pragma once

#include <enet/enet.h>
#include <memory>
#include <spdlog/spdlog.h>
#include "types/eLoginMethod.hpp"
#include <cpr/cpr.h>

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
        void to_http();
        void to_enet();
        static int get_oauth_link();

    private:
        std::string get_token_legacy(std::string &username, std::string &password);
        std::string get_token_google(std::string &username, std::string &password);
        std::string get_token_apple(std::string &username, std::string &password);
        std::string get_legacy_form_token();
        void parse_server_data(std::string data);

    public:
        ENetHost *enet_host{nullptr};
        ENetPeer *enet_peer{nullptr};
        std::map<std::string, std::string> parsed_server_data;
        cpr::Url url{"https://www.growtopia1.com/growtopia/server_data.php"};
        std::shared_ptr<spdlog::logger> logger;
        std::string session_cookies;

    public:
        static std::string user_agent;
        static std::string legacy_login_url;
        static std::string google_login_url;
        static std::string apple_login_url;
    };
}