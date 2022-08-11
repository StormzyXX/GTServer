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
        if (content.NetAvatar->m_requested_name.empty() || content.NetAvatar->m_requested_name.length() < 3) {
            content.NetAvatar->log_msg("You'll need a name `$3 chars`` or longer to play online with. (select cancel and enter a longer name)");
            return;
        }
        content.NetAvatar->send_variant({ "OnDialogRequest",
            "set_default_color|`o\n\n"
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
            "end_dialog|growid_apply|Cancel|`wGet My GrowID!``|\n" }, -1, 0);
    }
    void tankid_name(events::content& content) {
        text_scanner* logon = static_cast<text_scanner*>(content.m_data);
        if (!(
            logon->try_get("tankIDName", content.NetAvatar->m_tankid_name) &&
            logon->try_get("tankIDPass", content.NetAvatar->m_tankid_pass)
            )) {
            content.NetAvatar->log_msg("`4Invalid logon.``");
            return;
        }
        if (content.NetAvatar->m_tankid_name.empty()) {
            content.NetAvatar->send_variant({ "SetHasGrowID", 0, "", content.NetAvatar->m_tankid_pass }, -1, 0);
        }
    }
}