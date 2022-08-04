#include <events/action/actions.h++>
#include <proton/variant.h++>
#include <proton/text/text_scanner.h++>
#include <database/itemdb/itemdb.h++>

namespace svr {
    void refresh_items(events::content& content) {
        fmt::print("[{}]- Updating items request.", content.v_net->get_ip_address());
        content.v_net->log_msg("One moment, updating item data...");
        content.v_net->updatetankpacket(item_database::instance().get_packet(), item_database::instance().get_size());
    }
}