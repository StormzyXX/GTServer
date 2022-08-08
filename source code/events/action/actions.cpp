#include <events/action/actions.hpp>
#include <proton/variant.hpp>
#include <proton/text/text_scanner.hpp>
#include <database/itemdb/itemdb.hpp>
#include <database/player_tribute/tribute_data.hpp>

namespace svr {
    void action(events::content& content) {
        events* actions = static_cast<events*>(content.m_data);
        text_scanner* action_finder = static_cast<text_scanner*>(content.m_data);

        std::string action;
        if (action_finder->try_get("action", action))
        {
            if (!actions->call({ action, events::text_event::ACTION }, content)) {
                fmt::print("[{}]- Sent invalid action: {}.\n", content.NetAvatar->get_ip_address(), action);
                return;
            }
        }
        return;
        free(actions);
    }
    void refresh_items(events::content& content) {
        fmt::print("[{}]- Updating items request.", content.NetAvatar->get_ip_address());
        content.NetAvatar->log_msg("`4One moment, updating item data...``"); //optional, you could change it to custom one if want :-)
        content.NetAvatar->updatetankpacket(item_database::instance().get_packet(), item_database::instance().get_size());
    }
    void refresh_tribute(events::content& content) {
        fmt::print("[{}]- Updating tribute request.", content.NetAvatar->get_ip_address());
        content.NetAvatar->log_msg("`4One moment, updating tribute data...``"); //optional, real gt doesnt sends the message
        content.NetAvatar->send(player_tribute::get_packet(), sizeof(int) + sizeof(GameUpdatePacket) + reinterpret_cast<GameUpdatePacket*>(player_tribute::get_packet() + 4)->data_size, ENET_PACKET_FLAG_NO_ALLOCATE | ENET_PACKET_FLAG_RELIABLE);
    }
}