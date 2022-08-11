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
    }

    void refresh_items(events::content& content) {
        content.NetAvatar->log_msg("One moment, updating item data..."); //optional, you could change it to custom one if want :-)
        content.NetAvatar->updatetankpacket(item_database::instance().get_packet(), item_database::instance().get_size());
    }

    void refresh_tribute(events::content& content) {
        content.NetAvatar->log_msg("One moment, updating tribute data..."); //optional, real gt doesnt sends the message
        content.NetAvatar->send(player_tribute::get_packet(), sizeof(int) + sizeof(GameUpdatePacket) + reinterpret_cast<GameUpdatePacket*>(player_tribute::get_packet() + 4)->data_size, ENET_PACKET_FLAG_NO_ALLOCATE | ENET_PACKET_FLAG_RELIABLE);
    }

    void enter_game(events::content& content) {
        try {
            if (!content.NetAvatar->get_net_peer()) {
                content.NetAvatar->log_msg(fmt::format("Welcome back, `w{}``. Sadly, the server didn't found enough space for you. Please re-connect!", content.NetAvatar->m_requested_name));
                return;
            }
            if (content.NetAvatar->m_guest) {
                content.NetAvatar->send_variant({ "OnDialogRequest",
                    "set_default_color|o\n\n"
                    "add_label_with_icon|big|`wGet a GrowID!``|left|206|\n"
                    "add_spacer|small|\n"
                    "add_textbox|By choosing a `wGrowID``, you can use a name and password to logon from any device. Your name will be shown to other players!|\n"
                    "add_text_input|logon|Name ||18|\n"
                    "add_textbox|Your password must contain `w8 to 18 characters, 1 letter, 1 number`` and 1 special character: `w@#!$^&*.,``|\n"
                    "add_text_input_password|password|Password||18|\n"
                    "add_text_input_password|password_verify|Password Verify||18|\n"
                    "add_textbox|Your `wemail`` will only be used for account verification and support. If you enter a fake email, you can't verify your account, recover or change your password.|\n"
                    "add_text_input_password|email|Email||90|\n"
                    "add_textbox|We will never ask you for your password or email, never share it with anyone!|\n"
                    "end_dialog|growid_apply|Cancel|`wGet My GrowID!``|\n"
                    }, content.NetAvatar->get_net_id(), 0);
                return;
            }
            fmt::print("[{}]- logon attempt.\n", content.NetAvatar->get_ip_address());
            content.NetAvatar->log_msg(fmt::format("Welcome back, `w{}``.", content.NetAvatar->m_requested_name));
            content.NetAvatar->send_variant({ "OnRequestWorldSelectMenu", "" }, content.NetAvatar->get_net_id(), -1);
            content.NetAvatar->updategamepacket((uint32_t)NET_MESSAGE_GAME_PACKET, NULL, sizeof(GameUpdatePacket));
        }
        catch (std::exception e) {
            fmt::print("events::actions::enter_game: {}\n", e.what());
            content.NetAvatar->disconnect(0);
            return;
        }
        catch (...) {
            fmt::print("events::actions::enter_game: unknown exception!");
            content.NetAvatar->disconnect(0);
            return;
        }
    }
}