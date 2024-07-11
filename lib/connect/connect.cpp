#include "connect.hpp"
#include <cpr/cpr.h>
#include <iostream>
#include <regex>

std::string lib::Connect::user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36 Edg/120.0.0.0";
std::string lib::Connect::legacy_login_url = "";
std::string lib::Connect::google_login_url = "";
std::string lib::Connect::apple_login_url = "";

std::string lib::Connect::get_token(std::string &username, std::string &password, types::eLoginMethod method)
{
    this->logger->info("Getting token");
    switch (method)
    {
    case types::eLoginMethod::LEGACY_SIGNIN:
        return this->get_token_legacy(username, password);
        break;
    case types::eLoginMethod::GOOGLE_SIGNIN:
        return this->get_token_google(username, password);
        break;
    case types::eLoginMethod::APPLE_SIGNIN:
        return this->get_token_apple(username, password);
        break;
    }
    return "";
}

std::string lib::Connect::get_token_legacy(std::string &username, std::string &password)
{
    return "legacy";
}

std::string lib::Connect::get_token_google(std::string &username, std::string &password)
{
    return "google";
}

std::string lib::Connect::get_token_apple(std::string &username, std::string &password)
{
    return "apple";
}

void lib::Connect::get_oauth_link()
{
    assert(!user_agent.empty());
    std::regex regex("https:\\/\\/login\\.growtopiagame\\.com\\/(apple|google|player\\/growid)\\/(login|redirect)\\?token=[^\"]+");
    cpr::Url url{"https://login.growtopiagame.com/player/login/dashboard"};
    cpr::Response r = cpr::Post(url, cpr::Header{{"User-Agent", user_agent}}, cpr::Body{"requestedName|BraveDuck"});

    if (r.status_code == 200)
    {
        std::vector<std::string> tokens;
        std::smatch match;
        std::string::const_iterator searchStart(r.text.cbegin());

        while (std::regex_search(searchStart, r.text.cend(), match, regex))
        {
            tokens.push_back(match.str());
            searchStart = match.suffix().first;
        }

        if (tokens.size() >= 3)
        {
            apple_login_url = tokens[0];
            google_login_url = tokens[1];
            legacy_login_url = tokens[2];
            spdlog::info("Got oauth links for apple, google and legacy");
        }
        else
        {
            spdlog::error("Failed to get oauth tokens");
        }
    }
}
