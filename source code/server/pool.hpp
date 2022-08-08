#ifndef _POOL_
#define _POOL_
#include <unordered_map>
#include <string>
#include <server/server.hpp>
#include <chrono>

namespace svr {
    class pool 
    {
    public:
        pool() = default;

        ~pool() = default;

        std::unordered_map<uint8_t, ENetServer*> get_servers() 
        {
            return m_servers;
        }

        ENetServer* start_instance() 
        {
            uint8_t instanceId = static_cast<uint8_t>(m_servers.size());
            auto server = new ENetServer(++instanceId, m_address, m_port++, m_max_peers);
            m_servers.insert_or_assign(instanceId, server);
            return server;
        }

        void stop_instance(const uint8_t& instanceId) 
        {
            if (auto it = m_servers.find(instanceId); it != m_servers.end()) {
                it->second->stop();
                m_servers.erase(instanceId);
                return;
            }
            return;
        }

        void set_user_id(const int& uid)
        {
            user_id = uid;
        }

        int get_user_id(bool increase = true) 
        {
            return increase ? ++user_id : user_id;
        }
    private:
        std::string m_address{ "0.0.0.0" };

        uint16_t m_port{ 69 };
        size_t m_max_peers{ 240 };

        int user_id{ 0 };
        std::unordered_map<uint8_t, ENetServer*> m_servers{};
    };
}
#endif //! _POOL_