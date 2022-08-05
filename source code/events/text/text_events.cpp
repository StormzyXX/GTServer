#include <events/text/text_events.hpp>
#include <proton/variant.hpp>
#include <proton/text/text_scanner.hpp>

namespace svr {
    void requested_name(events::content& content) {
        text_scanner* logon = static_cast<text_scanner*>(content.m_data);
        if (!(
            logon->try_get("platformID", content.v_net->m_platform_id) &&
            logon->try_get("requestedName", content.v_net->m_requested_name)
            )) {
            content.v_net->log_msg("`4Invalid logon.``");
            return;
        }
        switch (content.v_net->m_platform_id)
        {
        case PLATFORM_ID_WINDOWS: {
            content.v_net->m_logon = (void*)(new WindowsPlayer());
            const auto& platform = static_cast<WindowsPlayer*>(content.v_net->m_logon);
            if (!(
                logon->try_get("fz", platform->m_fz) &&
                logon->try_get("wk", platform->m_wk) &&
                logon->try_get("zf", platform->m_zf)
                )) {
                content.v_net->log_msg("`4Invalid logon.``");
                return;
            }
            break;
        }
        default: {
            content.v_net->log_msg(fmt::format("`9Your device(`w{}``) isn't supported yet. Contact Eobard Thawne#0084 for further information.``", content.v_net->get_device_name(content.v_net->m_platform_id)));
            return;
        }
        }
        content.v_net->log_msg(fmt::format("Welcome back, `w{}``.", content.v_net->m_requested_name));
        content.v_net->send_logon_attempt();
    }
}