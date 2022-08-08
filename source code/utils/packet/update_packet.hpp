#ifndef _UPDATE_PACKET_
#define _UPDATE_PACKET_

#include <enet/include/enet.h>
#include <memory>
#include <string>
#include <proton/packet.hpp>

namespace svr::packet {
    std::string get_tank_packet(ENetPacket* pck) { //basically growtopia noobs's messageType but simple
        pck->data[pck->dataLength--] = 0;
        return reinterpret_cast<char*>(pck->data + sizeof(TankUpdatePacket::type));
    }
}

#endif // UTILS__PACKET_H