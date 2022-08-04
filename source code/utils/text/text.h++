#ifndef _TEXT_
#define _TEXT_

#include <cctype>
#include <regex>
#include <string_view>

namespace utils::text {

    inline bool email(const std::string& val) 
    {
        const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
        return std::regex_match(val, pattern); // taken from https://www.codespeedy.com/program-to-validate-email-id-using-regex-in-cpp/
    }

    inline bool discord(const std::string& val) 
    {
        const std::regex pattern("(\\w+)#(\\d)+");
        return std::regex_match(val, pattern); //taken from -
    }

    constexpr bool to_lowercase(std::string& str) 
    {
        if (str.empty())
            return false;

        for (auto& ch : str) 
        {
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
                ch = std::tolower(ch);
            else if (!(ch >= '0' && ch <= '9'))
                return false;
        }
        return true;
    }

    constexpr uint32_t quick_hash(const std::string_view& data) 
    {
        uint32_t hash = 5381;

        for (const auto& c : data)
            hash = ((hash << 5) + hash) + c;
        return hash;
    }

    constexpr uint32_t operator "" _qh(const char* str, std::size_t len) 
    {
        return utils::text::quick_hash(std::string_view{ str, len });
    }

    uint32_t hash_file(const char* file, int32_t length) 
    {
        if (!file) 
            return 0;
        unsigned char* n = (unsigned char*)file;
        uint32_t acc = 0x55555555;

        if (length == 0)
        {
            while (*n)
                acc = (acc >> 27) + (acc << 5) + *n++;
            return acc;
        }
        for (int32_t i = 0; i < length; i++)
            acc = (acc >> 27) + (acc << 5) + *n++;
        return acc;
    }
}

#endif //! _TEXT_