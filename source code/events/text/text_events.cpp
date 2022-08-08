#include <events/text/text_events.hpp>
#include <proton/variant.hpp>
#include <proton/text/text_scanner.hpp>

namespace svr {
    void requested_name(events::content& content) {
        text_scanner* logon = static_cast<text_scanner*>(content.m_data);
        if (!(
            logon->try_get("platformID", content.NetAvatar->m_platform_id) &&
            logon->try_get("requestedName", content.NetAvatar->m_requested_name)
            )) {
            content.NetAvatar->log_msg("`4Invalid logon.``");
            return;
        }
        switch (content.NetAvatar->m_platform_id)
        {
        case PLATFORM_ID_WINDOWS: {
            content.NetAvatar->m_logon = (void*)(new WindowsPlayer());
            const auto& platform = static_cast<WindowsPlayer*>(content.NetAvatar->m_logon);
            if (!(
                logon->try_get("fz", platform->m_fz) &&
                logon->try_get("wk", platform->m_wk) &&
                logon->try_get("zf", platform->m_zf)
                )) {
                content.NetAvatar->log_msg("`4Invalid logon.``");
                return;
            }
            break;
        }
        default: {
            content.NetAvatar->log_msg(fmt::format("`9Your device(`w{}``) isn't supported yet. Contact Eobard Thawne#0084 for further information.``", content.NetAvatar->get_device_name(content.NetAvatar->m_platform_id)));
            return;
        }
        //endhere
        }
        if (content.NetAvatar->m_requested_name.empty()) {
            //todo: get random name from constants::random_names vector and set
        }
        if (content.NetAvatar->m_requested_name.length() < 3) {
            content.NetAvatar->log_msg("You'll need a name `$3 chars`` or longer to play online with. (select cancel and enter a longer name)");
            return;
        }
        content.NetAvatar->log_msg(fmt::format("Welcome back, `w{}``.", content.NetAvatar->m_requested_name));
        content.NetAvatar->send_logon_attempt();
    }
}