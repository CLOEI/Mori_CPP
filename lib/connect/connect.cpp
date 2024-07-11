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

void lib::Connect::to_http()
{
    bool success = false;
    while (!success)
    {
        cpr::Response r = cpr::Post(url, cpr::Header{{"User-Agent", "UbiServices_SDK_2022.Release.9_PC64_ansi_static"},
                                                     {"Content-Type", "application/x-www-form-urlencoded"}});
        if (r.status_code == 200)
        {
            parse_server_data(r.text);
            success = true;
        }
        else
        {
            logger->error("Failed to parse data from server, re-parsing...");
        }
    }
}

void lib::Connect::to_enet()
{
    if (enet_initialize() < 0)
    {
        logger->error("Failed to initialize ENET");
        return;
    }

    if (parsed_server_data.find("maint") != parsed_server_data.end())
    {
        logger->error("Server is under maintenance");
        exit(0);
    }

    enet_host = enet_host_create(NULL, 1, 2, 0, 0);
    if (enet_host == nullptr)
    {
        logger->error("Failed to create ENET client");
        return;
    }
    enet_host->usingNewPacket = true;
    enet_host->checksum = enet_crc32;
    enet_host_compress_with_range_coder(enet_host);
    logger->info("Connecting to {}:{}", parsed_server_data["server"], parsed_server_data["port"]);

    ENetAddress address;
    if (enet_address_set_host_ip(&address, parsed_server_data["server"].c_str()) != 0)
    {
        logger->error("Failed to set host ip, restarting...");
    }
    address.port = std::stoi(parsed_server_data["port"]);

    enet_peer = enet_host_connect(enet_host, &address, 2, 0);
    if (enet_peer == nullptr)
    {
        logger->error("Failed to connect to server");
        return;
    }
}

std::string lib::Connect::get_token_legacy(std::string &username, std::string &password)
{
    assert(!legacy_login_url.empty());
    std::string payload = fmt::format("_token={}&growId={}&password={}", get_legacy_form_token(), username, password);

    cpr::Header headers{
        {"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36 Edg/120.0.0.0"},
        {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8"},
        {"Accept-Language", "en-US,en;q=0.5"},
        {"Accept-Encoding", "gzip, deflate, br, zstd"},
        {"Content-Type", "application/x-www-form-urlencoded"},
        {"Origin", "https://login.growtopiagame.com"},
        {"DNT", "1"},
        {"Sec-GPC", "1"},
        {"Connection", "keep-alive"},
        {"Referer", legacy_login_url},
        {"Cookie", session_cookies},
        {"Upgrade-Insecure-Requests", "1"},
        {"Sec-Fetch-Dest", "document"},
        {"Sec-Fetch-Mode", "navigate"},
        {"Sec-Fetch-Site", "same-origin"},
        {"Sec-Fetch-User", "?1"},
        {"sec-ch-ua-platform", "\"Windows\""},
        {"sec-ch-ua", "\"Edge\";v=\"120\", \"Chromium\";v=\"120\", \"Not=A?Brand\";v=\"24\""},
        {"sec-ch-ua-mobile", "?0"},
        {"Priority", "u=1"},
        {"TE", "trailers"}};

    cpr::Response r = cpr::Post(
        cpr::Url{"https://login.growtopiagame.com/player/growid/login/validate"},
        cpr::Header{headers},
        cpr::Body{payload});

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
    cpr::Header headers{
        {"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36 Edg/120.0.0.0"},
        {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8"},
        {"Accept-Language", "en-US,en;q=0.5"},
        {"Accept-Encoding", "gzip, deflate, br, zstd"},
        {"DNT", "1"},
        {"Sec-GPC", "1"},
        {"Connection", "keep-alive"},
        {"Upgrade-Insecure-Requests", "1"},
        {"Sec-Fetch-Dest", "document"},
        {"Sec-Fetch-Mode", "navigate"},
        {"Sec-Fetch-Site", "none"},
        {"Sec-Fetch-User", "?1"},
        {"sec-ch-ua-platform", "\"Windows\""},
        {"sec-ch-ua", "\"Edge\";v=\"120\", \"Chromium\";v=\"120\", \"Not=A?Brand\";v=\"24\""},
        {"sec-ch-ua-mobile", "?0"},
        {"Priority", "u=1"},
        {"TE", "trailers"}};

    cpr::Response r = cpr::Get(url, headers);

    std::string cookies;
    for (auto &cookie : r.cookies)
    {
        cookies += fmt::format("{}={}; ", cookie.GetName(), cookie.GetValue());
    }
    cookies.pop_back();
    cookies.pop_back();
    session_cookies = cookies;

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

void lib::Connect::parse_server_data(std::string data)
{
    std::string server_data = data;
    std::string delimiter = "\n";
    size_t pos = 0;
    std::string token;
    while ((pos = server_data.find(delimiter)) != std::string::npos)
    {
        token = server_data.substr(0, pos);
        size_t pos2 = token.find("|");
        std::string key = token.substr(0, pos2);
        std::string value = token.substr(pos2 + 1, token.length());
        parsed_server_data[key] = value;
        server_data.erase(0, pos + delimiter.length());
    }
}

int lib::Connect::get_oauth_link()
{
    assert(!user_agent.empty());

    std::regex regex("https:\\/\\/login\\.growtopiagame\\.com\\/(apple|google|player\\/growid)\\/(login|redirect)\\?token=[^\"]+");
    cpr::Header header{{"User-Agent", user_agent}};
    cpr::Url url{"https://login.growtopiagame.com/player/login/dashboard"};
    cpr::Response r = cpr::Post(url, header, cpr::Body{"requestedName|\n"});

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
            return -1;
        }
    }
    else
    {
        return -1;
    }
    return 0;
}
