#ifndef _NETAV_
#define _NETAV_

#include <string>
#include <vector>

#include <fmt/include/core.h>
#include <fmt/include/ranges.h>
#include <enet/include/enet.h>

#include <utils/math/common.hpp>

#include <constants/roles_manager.hpp>

#include <proton/packet.hpp>
#include <proton/variant.hpp>

#include <NetAvatar/logon/logon.hpp>

#include <server/server.hpp>

namespace svr {
	class ENetServer;
	class NetAvatar_t {
	public:
		NetAvatar_t(ENetPeer* peer, ENetServer* server)
			: m_peer(peer), m_server(server) {
			if (!m_peer)
				return;
			m_peer->data = this;

			m_ip_address.reserve(16);
			enet_address_get_host_ip(&m_peer->address, m_ip_address.data(), 16);
		}
		~NetAvatar_t() = default;
		uint32_t get_user_id() const;
		uint32_t get_net_id() const;
		[[nodiscard]] ENetPeer* get_net_peer() const;
		[[nodiscard]] const char* get_ip_address() const;

		void disconnect(const enet_uint32& data);

		bool upd_itp();

		void updatetankpacket(TankUpdatePacket pck, uintmax_t size);
		void updatetankpacket(TankUpdatePacket* pck, uintmax_t size);

		void updategamepacket(int32_t type, const void* pck, uintmax_t size);

		void send_variant(const variantlist_t& vlist, int net_id, int delay);

		std::string get_device_name(const uint32_t& platform_id);
		void log_msg(const std::string& message_str);

		void send_logon_attempt();
	public:
		int32_t m_platform_id = PLATFORM_ID_UNKNOWN;
		void* m_logon = NULL;

		std::atomic<bool> m_logged;

		std::string m_ip_address = "";

		std::string m_requested_name = "";
	private:
		ENetPeer* m_peer;

		ENetServer* m_server;

		uint32_t m_user_id = -1;
		uint32_t m_net_id = -1;
	};
}
#endif //! _NETAV_