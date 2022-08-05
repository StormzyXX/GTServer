#ifndef _VARLIST_
#define _VARLIST_
#include <utility>
#include <memory>
#include <string>
#include <typeinfo>
#include <variant>
#include <vector>
#include <iomanip>
#include <fmt/include/ranges.h>
#include <proton/packet.hpp>
#include <utils/math/common.hpp>
#include <utils/binary/writer.hpp>

namespace svr {
    enum class VariantType
    {
        VARIANT_UNKNOWN,
        VARIANT_FLOAT,
        VARIANT_STR,
        VARIANT_VEC2,
        VARIANT_VEC3,
        VARIANT_UINT,
        VARIANT_INT = 9
    };

    using variant = std::variant<float, std::string, CL_Vec2f, CL_Vec3f, uint32_t, int>;

    class variantlist_t
    {
    public:
        variantlist_t(variant& v0) : m_variants({ v0 }) {}
        variantlist_t(variant& v0, variant& v1) : m_variants({ v0,v1 }) {}
        variantlist_t(variant& v0, variant& v1, variant& v2) : m_variants({ v0,v1,v2 }) {}
        variantlist_t(variant& v0, variant& v1, variant& v2, variant& v3) : m_variants({ v0,v1,v2,v3 }) {}
        variantlist_t(variant& v0, variant& v1, variant& v2, variant& v3, variant& v4) : m_variants({ v0,v1,v2,v3,v4 }) {}
        variantlist_t(variant& v0, variant& v1, variant& v2, variant& v3, variant& v4, variant& v5) : m_variants({ v0,v1,v2,v3,v4,v5 }) {}
        variantlist_t(variant& v0, variant& v1, variant& v2, variant& v3, variant& v4, variant& v5, variant& v6) : m_variants({ v0,v1,v2,v3,v4,v5,v6 }) {}

        variantlist_t(const variant& v0) : m_variants({ v0 }) {}
        variantlist_t(const variant& v0, const variant& v1) : m_variants({ v0,v1 }) {}
        variantlist_t(const variant& v0, const variant& v1, const variant& v2) : m_variants({ v0,v1,v2 }) {}
        variantlist_t(const variant& v0, const variant& v1, const variant& v2, const variant& v3) : m_variants({ v0,v1,v2,v3 }) {}
        variantlist_t(const variant& v0, const variant& v1, const variant& v2, const variant& v3, const variant& v4) : m_variants({ v0,v1,v2,v3,v4 }) {}
        variantlist_t(const variant& v0, const variant& v1, const variant& v2, const variant& v3, const variant& v4, const variant& v5) : m_variants({ v0,v1,v2,v3,v4,v5 }) {}
        variantlist_t(const variant& v0, const variant& v1, const variant& v2, const variant& v3, const variant& v4, const variant& v5, const variant& v6) : m_variants({ v0,v1,v2,v3,v4,v5,v6 }) {}

        int get_type(const variant& var) const
        {
            switch (var.index())
            {
            case 0:
                return (int)VariantType::VARIANT_FLOAT;
            case 1:
                return (int)VariantType::VARIANT_STR;
            case 2:
                return (int)VariantType::VARIANT_VEC2;
            case 3:
                return (int)VariantType::VARIANT_VEC3;
            case 4:
                return (int)VariantType::VARIANT_UINT;
            case 5:
                return (int)VariantType::VARIANT_INT;
            default:
                return (int)VariantType::VARIANT_UNKNOWN;
            }
        }

        // gotta make this cleaner, soon.
        GameUpdatePacket* serialize() const
        {
            uint32_t size = (uint32_t)(sizeof(GameUpdatePacket) + 1 + (m_variants.size() * 2));

            for (uint8_t i = 0; i < m_variants.size(); ++i)
            {
                const variant& var = m_variants.at(i);
                int type = get_type(var);

                switch (type)
                {
                case (int)VariantType::VARIANT_FLOAT:
                case (int)VariantType::VARIANT_UINT:
                case (int)VariantType::VARIANT_INT:
                    size += 4;
                    break;

                case (int)VariantType::VARIANT_STR:
                    size += 4 + (uint32_t)std::get<std::string>(var).size();
                    break;

                case (int)VariantType::VARIANT_VEC2:
                    size += 8;
                    break;

                case (int)VariantType::VARIANT_VEC3:
                    size += 12;
                    break;

                default:
                    break;
                }
            }

            GameUpdatePacket* p = static_cast<GameUpdatePacket*>(malloc(size));

            memset(p, 1, size);

            uint8_t* extra = static_cast<uint8_t*>(&p->data);
            uint32_t pos = 1;
            uint8_t cnt = 0;

            for (uint8_t i = 0; i < m_variants.size(); ++i)
            {
                const variant& var = m_variants.at(i);
                int type = get_type(var);
                extra[pos] = static_cast<uint8_t>(i);
                extra[pos + 1] = static_cast<uint8_t>(type);

                switch (type)
                {
                case (int)VariantType::VARIANT_FLOAT:
                {
                    float v_float = std::get<float>(var);
                    memcpy(extra + pos + 2, &v_float, 4);
                    pos += 6;
                    break;
                }

                case (int)VariantType::VARIANT_STR:
                {
                    std::string v_str = std::get<std::string>(var);
                    uint32_t len = static_cast<uint32_t>(v_str.size());
                    memcpy(extra + pos + 2, &len, 4);
                    memcpy(extra + pos + 6, v_str.data(), len);
                    pos += 6 + len;
                    break;
                }

                case (int)VariantType::VARIANT_VEC2:
                {
                    CL_Vec2f v_vec = std::get<CL_Vec2f>(var);
                    memcpy(extra + pos + 2, &v_vec.x, 4);
                    memcpy(extra + pos + 6, &v_vec.y, 4);
                    pos += 10;
                    break;
                }

                case (int)VariantType::VARIANT_VEC3:
                {
                    CL_Vec3f v_vec = std::get<CL_Vec3f>(var);
                    memcpy(extra + pos + 2, &v_vec.x, 4);
                    memcpy(extra + pos + 6, &v_vec.y, 4);
                    memcpy(extra + pos + 10, &v_vec.z, 4);
                    pos += 14;
                    break;
                }

                case (int)VariantType::VARIANT_UINT:
                {
                    uint32_t v_int = std::get<uint32_t>(var);
                    memcpy(extra + pos + 2, &v_int, 4);
                    pos += 6;
                    break;
                }

                case (int)VariantType::VARIANT_INT:
                {
                    int v_int = std::get<int>(var);
                    memcpy(extra + pos + 2, &v_int, 4);
                    pos += 6;
                    break;
                }

                default:
                    break;
                }

                cnt = i;
            }

            extra[0] = cnt + 1;

            p->type = 1;
            p->net_id = -1;
            p->flags |= 0x8;
            p->data_size = pos;

            return p;
        }

    private:
        std::vector<variant> m_variants;
    };
}

#endif //! _VARLIST_