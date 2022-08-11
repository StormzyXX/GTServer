#ifndef _DATABASE_ITEM_DB_
#define _DATABASE_ITEM_DB_

#include <vector>
#include <filesystem>

#include <database/itemdb/items_info.hpp>
#include <proton/packet.hpp>

namespace svr
{
    class item_database
    {
    public:
        static item_database& instance() { static item_database items; return items; }

        static bool init() { return instance().interface__init(); }

        [[nodiscard]] uint32_t get_hash() const { return m_hash; }

        [[nodiscard]] GameTextPacket* get_packet() const;
        [[nodiscard]] uint32_t get_size() const;
    private:
        item_database() = default;
        ~item_database();

        bool interface__init();
    private:
        uint32_t m_hash = 0;
        char* m_data{};
        size_t m_data_size = 0;

        uint32_t m_item_count = 0;
        uint16_t m_version = 14;

        TankUpdatePacket* m_packet;
    private:
        std::vector<item> m_items{};
    };
}

#endif //! _DATABASE_ITEM_DB_