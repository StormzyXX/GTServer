#ifndef _DATABASE_TRIBUTE_
#define _DATABASE_TRIBUTE_

#include <vector>
#include <filesystem>

#include <database/itemdb/items_info.hpp>
#include <proton/packet.hpp>

namespace svr {
    class player_tribute
    {
    public:
        static void init() { get().interface__init(); }

        static player_tribute& get() { static player_tribute data; return data; }

        static uint32_t get_hash() { return get().interface__get_hash(); }
        static uint8_t* get_packet() { return get().interface__get_packet(); }
    private:
        player_tribute() = default;
        ~player_tribute();

        void interface__init();

        uint32_t interface__get_hash() const;
        uint8_t* interface__get_packet();
    private:
        uint32_t m_hash = 0;
        uint8_t* m_data = nullptr;
        uint8_t* m_p_data = nullptr;
    };
}

#endif //! _DATABASE_TRIBUTE_