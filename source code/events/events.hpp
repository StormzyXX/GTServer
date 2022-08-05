#ifndef _EVENTS_
#define _EVENTS_

#include <functional>
#include <unordered_map>
#include <NetAvatar/NetAvatar.hpp>
#include <server/server.hpp>
#include <proton/packet.hpp>

namespace svr {
    class ENetServer;
    class NetAvatar_t;
    class events {
    public:
        enum class text_event {
            TEXT,
            ACTION
        };

        struct content {
            NetAvatar_t* v_net;
            ENetServer* g_server;
            events* g_event_svr;
            //TODO: database

            void* m_data;
        };
    public:
        events();
        ~events();

        void load_events();

        void register_event(const std::pair<std::string, text_event> data, std::function<void(events::content&)> fn);
        void register_event(const uint8_t& data, std::function<void(events::content&)> fn);

        bool call(const std::pair<std::string, text_event> data, events::content& ctx);

        size_t get_action_events() const { return m_action_event.size(); }
        size_t get_text_events() const { return m_text_events.size(); }
        size_t get_packet_events() const { return m_packet_events.size(); }
    private:
        using text_event_value = std::unordered_map<uint64_t, std::function<void(events::content&)>>;
        using packet_event_value = std::unordered_map<uint8_t, std::function<void(events::content&)>>;

        text_event_value m_action_event;
        text_event_value m_text_events;
        packet_event_value m_packet_events;
    };
}

#endif //! _EVENTS_