#include <database/player_tribute/tribute_data.hpp>
#include <filesystem>
#include <fstream>
#include <fmt/include/core.h>
#include <utils/binary/writer.hpp>
#include <utils/tools/tools.hpp>

namespace svr {
    player_tribute::~player_tribute()
    {
        free(m_data);
        free(m_p_data);
    }

    void player_tribute::interface__init()
    {
        std::string epic_players = "Thawne ; Rebillion \n\n";
        std::string exceptional_mentors = "No players are this amazing yet! Could you be the first? \n\n";

        int data_size = epic_players.length() + exceptional_mentors.length();
        m_data = static_cast<uint8_t*>(malloc((size_t)data_size));
        uint16_t pos = 0;

        uint16_t epic_players_len = static_cast<uint16_t>((uint16_t)epic_players.length());
        memcpy(m_data, &epic_players_len, 2);
        pos += 2;
        memcpy(m_data + pos, epic_players.c_str(), epic_players_len);
        pos += epic_players_len;

        uint16_t exceptional_mentors_len = static_cast<uint16_t>((uint16_t)exceptional_mentors.length());
        memcpy(m_data + pos, &exceptional_mentors_len, 2);
        pos += 2;
        memcpy(m_data + pos, exceptional_mentors.c_str(), exceptional_mentors_len);
        pos += exceptional_mentors_len;

        m_hash = utils::tools::hash(m_data, data_size); //sets hash

        int p_size = 4 + sizeof(GameUpdatePacket) + data_size;
        m_p_data = static_cast<uint8_t*>(malloc((size_t)p_size));

        memset(m_p_data, 0, (size_t)p_size);
        uint32_t type = NET_MESSAGE_GAME_PACKET;
        memcpy(m_p_data, &type, (size_t)4);

        reinterpret_cast<GameUpdatePacket*>(m_p_data + 4)->type = NET_GAME_PACKET_SEND_PLAYER_TRIBUTE_DATA;
        reinterpret_cast<GameUpdatePacket*>(m_p_data + 4)->net_id = -1;
        reinterpret_cast<GameUpdatePacket*>(m_p_data + 4)->flags |= NET_GAME_PACKET_FLAGS_EXTENDED;
        reinterpret_cast<GameUpdatePacket*>(m_p_data + 4)->data_size = (uint32_t)data_size;
        memcpy(&reinterpret_cast<GameUpdatePacket*>(m_p_data + 4)->data, m_data, reinterpret_cast<GameUpdatePacket*>(m_p_data + 4)->data_size);

        //clearing memory...
        free(m_data);
        free(m_p_data);
        fmt::print("player tribute serialization -> succeed.\n");
    }

    uint32_t player_tribute::interface__get_hash() const {
        return m_hash;
    }
    uint8_t* player_tribute::interface__get_packet() {
        return m_p_data;
    }
}