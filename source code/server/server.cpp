#include <server/server.hpp>
#include <fmt/include/core.h>
#include <NetAvatar/NetAvatar.hpp>
#include <proton/packet.hpp>
#include <utils/packet/update_packet.hpp>
#include <proton/text/text_scanner.hpp>

namespace svr
{
    ENetServer::ENetServer(const uint8_t& instanceId, const std::string& address, const uint16_t& port, const size_t& max_peers) : m_instanceId(instanceId), m_address(address), m_port(port), m_max_peers(max_peers) 
    {
    }
    ENetServer::~ENetServer() 
    {
       for (auto& peer : m_peers)
       enet_peer_disconnect_later(peer, 0U);
       return EXIT_FAILURE; 
    }
    void ENetServer::set_component(events* ev, database* db) {
        this->m_event_manager = ev;
        this->m_database = db;
    }

    std::string ENetServer::get_host() 
    {
       if (!m_host) 
		   return {};

	char ipString[16];
	enet_address_get_host_ip(&m_host->m_address, ipString, sizeof ipString);
	    return ipStr;
        return this->m_host->address.port;
    }
    bool ENetServer::start() 
    {
        ENetAddress address;
        address.port = m_port;
        enet_address_set_host(&address, m_address.c_str());

        m_host = enet_host_create(&address, m_max_peers, 2, 0, 0);
        if (!m_host)
            return false;

        m_host->checksum = enet_crc32;
        enet_host_compress_with_range_coder(m_host);
        fmt::print("starting server: {}, {}:{}.\n", m_instanceId, m_address, m_port);
        m_running.store(true);
        
        return true;
    }


    void ENetServer::start_service() 
    {
        m_service = std::thread{ &ENetServer::service, this };
        m_service.detach();
    }
    void ENetServer::service() 
    {
        while (m_running.load()) 
        {
            if (enet_host_service(m_host, &m_event, 100) // 100ms timeout
                continue;
            switch (m_event.type) 
            {
                case ENET_EVENT_TYPE_CONNECT: 
                {
                     NetAvatar_t* NetClient = new NetAvatar_t(m_event.peer, this);
                     NetClient->updatetankpacket({ NET_MESSAGE_SERVER_HELLO }, sizeof(TankUpdatePacket));
                     break;
                }
                case ENET_EVENT_TYPE_DISCONNECT: {
                    NetAvatar_t* Disconnected_NetClient = static_cast<NetAvatar_t*>(m_event.peer->data);
                    if (!Disconnected_NetClient)
                        return;
                    delete Disconnected_NetClient;
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE: 
                {
                    if (!m_event.peer || !m_event.peer->data)
                        return;
                    if (m_event.packet->dataLength < sizeof(TankUpdatePacket::type) + 1 || m_event.packet->dataLength > 0x200)
                        return;

                    NetAvatar_t* NetClient = static_cast<NetAvatar_t*>(m_event.peer->data);
                    const auto& tank_packet = reinterpret_cast<TankUpdatePacket*>(m_event.packet->data);
                    switch (tank_packet->type)
                    {
                        case NET_MESSAGE_GENERIC_TEXT: case NET_MESSAGE_GAME_MESSAGE:
                        {
                            const auto& str = packet::get_tank_packet(m_event.packet);
                            text_scanner text;
                            if (!text.parse(str) || text.get_data().size() == 0) {
                                NetClient->disconnect(0U);
                                fmt::print("[{}]- Sent invalid packet, disconnected from server.\n", NetClient->get_ip_address());
                                return;
                            }
                            std::string ev_function = str.substr(0, str.find('|'));
                            events::content cache{ NetClient, this, m_event_manager, m_database, &text};
                            if (!m_event_manager->call({ ev_function, events::text_event::TEXT }, cache)) {
                                fmt::print("[{}]- Unhandled packet type {}: {}.\n", NetClient->get_ip_address(), tank_packet->type, str); //prints unhandled packets that client sends to server
                                break;
                            }
                            if (!m_event_manager->call({ ev_function, events::text_event::ACTION }, cache)) {
                                break;
                            }
                            break;
                        }
                        case NET_MESSAGE_GAME_PACKET:
                        {
                            GameUpdatePacket* update_packet = reinterpret_cast<GameUpdatePacket*>(tank_packet->data);
                            if (!update_packet)
                                break;
                            if (m_event.packet->dataLength < 56) {
                                fmt::print("[{}]- Invalid packet (size is too small).\n", NetClient->get_ip_address()); //handles packets
                                break;
                            }
                            break;
                        }
                    }
                    enet_packet_destroy(m_event.packet);
                }
                case ENET_EVENT_TYPE_NONE:
                default:
                    break;
            }
        }
    }
}
