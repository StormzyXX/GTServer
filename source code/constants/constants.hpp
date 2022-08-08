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
        namespace sethmumu {
            const std::vector<std::string> random_names = {
                "BoardCake",
                "BraveFairy",
                "BoardLen",
                "BurpSickle",
                "SicleBoard",
                "FairyMouse",
                "SickleSmell",
                "BobJoy",
                "EinstWave",
                "LiteBob",
                "SolidDuck",
                "RatTickle",
                "TicleLite",
                "WiggleSnake",
                "DeathTiny",
                "ZeroWiggle",
                "BravePie",
                "BadBurp",
                "BadGood",
                "GoodBad",
                "WiggleJoy",
                "CryLaugh",
                "BoardMouse",
                "FlashDuck",
                "DuckWave",
                "BobSickle",
                "GripHead",
                "DeathFairy",
                "DeathGood",
                "SickleBad",
                "TickleRat",
                "MouseEinst",
                "GarWatch",
                "WatchGar"
            };
        }
    };
}
#endif //! _CONFIG_