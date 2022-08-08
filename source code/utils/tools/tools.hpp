#ifndef _TEXT_
#define _TEXT_

#include <cctype>
#include <regex>
#include <string_view>

namespace utils::tools {
    std::string cypher(const std::string& input, uint32_t id) {
        if (input.empty()) 
            return "";
        constexpr std::string_view secret = "PBG892FXX982ABC*";
        std::string return_value(input.size(), 0);
        for (uint32_t i = 0; i < input.size(); ++i) {
            return_value[i] = input[i] ^ secret[(i + id) % secret.size()];
        }
        return return_value;
    }
    uint32_t hash(const void* data, uintmax_t data_len) {
        if (!data) 
            return 0;
        uint32_t hash = 0x55555555;

        for (uintmax_t i = 0; i < data_len; ++i) {
            hash = (hash >> 27) + (hash << 5) + static_cast<const uint8_t*>(data)[i];
        }
        return hash;
    }
}
#endif //! _TEXT_