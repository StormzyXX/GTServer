#ifndef _ACTION_EVENTS_
#define _ACTION_EVENTS_
#include <events/events.hpp>

namespace svr {
    void action(events::content& content);
    void refresh_items(events::content& content);
    void refresh_tribute(events::content& content);
}

#endif //! _ACTION_EVENTS_