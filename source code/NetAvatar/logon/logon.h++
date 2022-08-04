#ifndef _NETAV_LOGON_
#define _NETAV_LOGON_

#include <string>
#include <vector>
namespace svr {
    enum : int32_t {
        PLATFORM_ID_UNKNOWN = -1,
        PLATFORM_ID_WINDOWS,
        PLATFORM_ID_IOS,
        PLATFORM_ID_OSX,
        PLATFORM_ID_LINUX,
        PLATFORM_ID_ANDROID,
        PLATFORM_ID_WINDOWS_MOBILE,
        PLATFORM_ID_WEBOS,
        PLATFORM_ID_BBX,
        PLATFORM_ID_FLASH,
        PLATFORM_ID_HTML5
    };
    enum class dialog_type {
        DIALOGTYPE_GROWID_REGISTER,
    };

    class WindowsPlayer
    {
    public:
        std::string m_fz; //fz
        std::string m_machine_hash; //?
        std::string m_wk; //wk
        std::string m_zf; //zf
    };

    class IOSPlayer
    {
    public:
        std::string m_aid; //aid
        std::string m_vid; //vid
        std::string m_machine_hash; //?
    };

    class AndroidPlayer
    {
    public:
        std::string m_gid; //gid
        std::string m_tr; //?
    };
}
#endif //! _NETAV_LOGON_