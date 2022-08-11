#include <events/events.hpp>
#include <fmt/include/core.h>
#include <utils/text/text.hpp>
#include <events/text/text_events.hpp>
#include <events/action/actions.hpp>


namespace svr {
    events::events() 
    {
        m_action_event = std::unordered_map<uint64_t, std::function<void(events::content&)>>();
        m_text_events = std::unordered_map<uint64_t, std::function<void(events::content&)>>();
        m_packet_events = std::unordered_map<uint8_t, std::function<void(events::content&)>>();
    }
    events::~events() 
    {
        m_action_event.clear();
        m_text_events.clear();
        m_packet_events.clear();
    }

    void events::load_events() 
    {
        this->register_event({ "requestedName", text_event::TEXT }, requested_name);
        this->register_event({ "action", text_event::TEXT }, action);
        this->register_event({ "refresh_item_data", text_event::ACTION }, refresh_items);
        this->register_event({ "refresh_player_tribute_data", text_event::ACTION }, refresh_tribute);
        this->register_event({ "enter_game", text_event::ACTION }, enter_game);
        this->register_event({ "tankIDName", text_event::TEXT }, tankid_name);
    }

    void events::register_event(const std::pair<std::string, text_event> data, std::function<void(events::content&)> fn)
    {
        switch (data.second) 
        {
            case events::text_event::TEXT: 
            {
                m_text_events.insert_or_assign(utils::text::quick_hash(data.first), fn);
                break;
            }
            case events::text_event::ACTION: 
            {
                m_action_event.insert_or_assign(utils::text::quick_hash(data.first), fn);
                break;
            }
            default: 
                break;
        }
    }

    void events::register_event(const uint8_t& data, std::function<void(events::content&)> fn)
    {
        m_packet_events.insert_or_assign(data, fn);
    }

    bool events::call(const std::pair<std::string, text_event> data, events::content& ctx)
    {
        switch (data.second)
        {
            case events::text_event::TEXT: {
                if (auto it = m_text_events.find(utils::text::quick_hash(data.first)); it != m_text_events.end())
                {
                    it->second(ctx);
                    return true;
                }
                return false;
                break;
            }
            case events::text_event::ACTION: {
                if (auto it = m_action_event.find(utils::text::quick_hash(data.first)); it != m_action_event.end())
                {
                    it->second(ctx);
                    return true;
                }
                return false;
                break;
            }
        default:
            return false;
            break;
        }
    }
}
