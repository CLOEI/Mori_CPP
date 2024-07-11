#include "bot.hpp"
#include <enet/enet.h>
#include <iostream>

#include <utility>

using namespace lib;

Bot::Bot(const std::string &username, const std::string &password, types::eLoginMethod method,
         std::shared_ptr<spdlog::logger> logger) : Connect(logger)
{
    this->username = username;
    this->password = password;
    this->method = method;
    this->logger = std::move(logger);

    // this->to_http();
    // this->to_enet();
    // this->start_event();
    this->get_token(this->username, this->password, this->method);
}

void Bot::disconnect()
{
    enet_peer_disconnect(enet_peer, 0);
}

void lib::Bot::start_event()
{
    ENetEvent event;
    this->is_running = true;
    while (is_running)
    {
        while (enet_host_service(enet_host, &event, 1000) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                logger->info("Connected to the server");
                break;
            case ENET_EVENT_TYPE_RECEIVE:
            {
                logger->info("Received a packet");
                uint32_t type = *(uint32_t *)event.packet->data;
                std::cout << "Type: " << std::to_string(type) << std::endl;
                if (type == 1)
                {
                    // this->get_token(username, password, method);
                    std::string data = "requestedName|\n";
                    ENetPacket *pkt = enet_packet_create(nullptr, sizeof(uint32_t) + data.length(), ENET_PACKET_FLAG_RELIABLE);
                    *(uint32_t *)pkt->data = 2;
                    memcpy(&pkt->data[sizeof(uint32_t)], data.c_str(), data.length());
                    enet_peer_send(enet_peer, 0, pkt);
                }
                if (type == 3)
                {
                    std::string data = (char *)event.packet->data + sizeof(uint32_t);
                    std::cout << "Data: " << data << std::endl;
                }
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT:
                logger->error("Disconnected, reconnecting to server...");
                break;
            }
            enet_packet_destroy(event.packet);
        }
    }
}

void Bot::spoof()
{
}
