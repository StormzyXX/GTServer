#ifndef _CONFIG_
#define _CONFIG_

#include <string>

#include <enet/include/enet.h>

namespace svr {
    namespace constants {
        namespace mysql {
            const std::string& host = "tcp://127.0.0.1:3306";
            const std::string& username = "root";
            const std::string& password = "";
            const std::string& schema = "cf";
        };
        namespace settings {
            constexpr std::string_view server_name = "Growtopia.cf";
        }
    };
}
#endif //! _CONFIG_