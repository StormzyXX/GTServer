#ifndef SERVICE_
#define SERVICE_
#include <atomic>
#include <stdint.h>
#include <string>
#include <thread>
#include <vector>

#include <enet/include/enet.h>
#include <fmt/include/core.h>
#include <events/events.h++>

namespace svr
{
    class events;
    class ENetServer {
    public:
        ENetServer(const uint8_t& instanceId, const std::string& address, const uint16_t& port, const size_t& max_peers);
        ~ENetServer();
        void set_component(events* ev);

        std::pair<std::string, uint16_t> get_host();
        bool start();
        bool stop();

        void start_service();
        void service();
    private:
        uint8_t m_instanceId;
        std::string m_address;
        uint16_t m_port;

        ENetHost* m_host;
        ENetEvent m_event;
        size_t m_max_peers;
        std::atomic<bool> m_running{ false };
        std::thread m_service;

        std::vector<ENetPeer*> m_peers{};
    private:
        events* m_event_manager;
        //database* m_database;
    };
}
#endif // !SERVICE_

