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
    switch (method)
    {
    case types::eLoginMethod::LEGACY_SIGNIN:
        this->logger->info("Getting legacy token");
        return this->get_token_legacy(username, password);
        break;
    case types::eLoginMethod::GOOGLE_SIGNIN:
        this->logger->info("Getting google token");
        return this->get_token_google(username, password);
        break;
    case types::eLoginMethod::APPLE_SIGNIN:
        this->logger->info("Getting apple token");
        return this->get_token_apple(username, password);
        break;
    }
    return "";
}

std::string lib::Connect::get_token_legacy(std::string &username, std::string &password)
{
    assert(!legacy_login_url.empty());
    std::string _token = this->get_legacy_form_token();
    cpr::Url url{"https://login.growtopiagame.com/player/growid/login/validate"};
    cpr::Response r = cpr::Post(url, cpr::Header{{"User-Agent", user_agent}, {"Referer", legacy_login_url}}, cpr::Body{fmt::format("_token={}&growId={}&password={}", _token, username, password)});

    if (r.status_code == 200)
    {
        std::cout << r.text << std::endl;
    }
    else
    {
        if (strstr(r.text.c_str(), "Page Expired"))
        {
            logger->error("Auth failed: Page Expired");
        }
        else if (strstr(r.text.c_str(), "Bad Request"))
        {
            logger->error("Auth failed: Bad Request");
        }
        else if (strstr(r.text.c_str(), "Not Found"))
        {
            logger->error("Auth failed: Not Found");
        }
        else
        {

            std::cout << r.text << std::endl;
            logger->error("Auth failed: Unknown error");
        }
    }
    return "";
}

std::string lib::Connect::get_token_google(std::string &username, std::string &password)
{
    return "";
}

std::string lib::Connect::get_token_apple(std::string &username, std::string &password)
{
    return "";
}

std::string lib::Connect::get_legacy_form_token()
{
    cpr::Url url{legacy_login_url};
    cpr::Response r = cpr::Get(url, cpr::Header{{"User-Agent", user_agent}});
    if (r.status_code == 200)
    {
        std::regex regex("name=\"_token\"\\s+type=\"hidden\"\\s+value=\"([^\"]*)\"");
        std::smatch match;
        std::string::const_iterator searchStart(r.text.cbegin());
        if (std::regex_search(searchStart, r.text.cend(), match, regex))
        {
            return match.str(1);
        }
    }
    return "";
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
