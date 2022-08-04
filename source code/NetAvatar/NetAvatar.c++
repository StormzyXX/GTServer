#include <NetAvatar/NetAvatar.h++>
#include <constants/constants.h++>
#include <database/itemdb/itemdb.h++>

namespace svr {
	uint32_t NetAvatar_t::get_user_id() const {
		return this->m_user_id;
	}

	uint32_t NetAvatar_t::get_net_id() const {
		return this->m_net_id;
	}

	[[nodiscard]] ENetPeer* NetAvatar_t::get_net_peer() const {
		return this->m_peer;
	}

	[[nodiscard]] const char* NetAvatar_t::get_ip_address() const {
		return m_ip_address.data();
	}

	void NetAvatar_t::disconnect(const enet_uint32& data) {
		if (!m_peer) 
			return; //pervents from falsely sent disconnects
		enet_peer_disconnect_later(m_peer, data);
	}

	bool NetAvatar_t::upd_itp() {
		return enet_address_get_host_ip(&m_peer->address, m_ip_address.data(), 16) < 0 ? false : true;
	}

	void NetAvatar_t::updatetankpacket(TankUpdatePacket pck, uintmax_t size) {
		if (!m_peer)
			return;
		ENetPacket* new_pck = enet_packet_create(nullptr, size, ENET_PACKET_FLAG_RELIABLE);
		if (!new_pck)
			return;
		std::memcpy(new_pck->data, &pck, size);
		if (enet_peer_send(m_peer, 0, new_pck) != 0)
			enet_packet_destroy(new_pck); //free's the data
	}

	void NetAvatar_t::updatetankpacket(TankUpdatePacket* pck, uintmax_t size) {
		if (!m_peer)
			return;
		GameUpdatePacket * updated_packet = reinterpret_cast<GameUpdatePacket*>(&pck->data);
		ENetPacket* new_packet = enet_packet_create(nullptr, size, ENET_PACKET_FLAG_RELIABLE);
		if (!new_packet || !updated_packet)
			return;
		std::memcpy(new_packet->data, &pck->type, 4);
		std::memcpy(new_packet->data + 4, updated_packet, sizeof(GameUpdatePacket) + updated_packet->data_size);

		if (enet_peer_send(m_peer, 0, new_packet) != 0)
			enet_packet_destroy(new_packet); //free's the data
		free(updated_packet); //pervents from memory leaks
	}

	void NetAvatar_t::updategamepacket(int32_t type, const void* pck, uintmax_t size) {
		if (!m_peer)
			return;
		ENetPacket* packet = enet_packet_create(NULL, 5 + size, ENET_PACKET_FLAG_RELIABLE);
		if (!packet)
			return;
		std::memcpy(packet->data, &type, 4);
		packet->data[size + 4] = 0;

		if (pck)
			std::memcpy(packet->data + 4, pck, size);

		if (enet_peer_send(m_peer, 0, packet) != 0)
			enet_packet_destroy(packet); //free's the data
	}

	void NetAvatar_t::send_variant(const variantlist_t& vlist, int net_id, int delay) {
		if (!m_peer)
			return;
		GameUpdatePacket* packet = vlist.serialize();

		packet->net_id = net_id;
		packet->delay = delay;

		updategamepacket(NET_MESSAGE_GAME_PACKET, packet, packet->data_size + sizeof(GameUpdatePacket));
		free(packet);
	}

	void NetAvatar_t::log_msg(const std::string& message_str) {
		const auto& data = fmt::format("action|log\nmsg|{}", message_str);
		this->updategamepacket(NET_MESSAGE_GAME_MESSAGE, data.data(), data.size());
	}
	
	std::string NetAvatar_t::get_device_name(const uint32_t& platform_id) {
		switch (platform_id)
		{
		     case PLATFORM_ID_ANDROID: return "Android"; break;
			 case PLATFORM_ID_BBX: return "BBX"; break;
			 case PLATFORM_ID_FLASH: return "Flash"; break;
			 case PLATFORM_ID_HTML5: return "HTML5"; break;
			 case PLATFORM_ID_IOS: return "Apple IOS"; break;
			 case PLATFORM_ID_LINUX: return "Linux"; break;
			 case PLATFORM_ID_OSX: return "OSX (Mac)"; break;
			 case PLATFORM_ID_WEBOS: return "Web OS"; break;
			 case PLATFORM_ID_WINDOWS: return "Windows"; break;
			 case PLATFORM_ID_WINDOWS_MOBILE: return "Windows Mobile"; break;
			 default: return "Unknown"; break;
		}
		return "Unknown";
	}

	void NetAvatar_t::send_logon_attempt() {
		send_variant({
	        "OnSuperMainStartAcceptLogonHrdxs47254722215a", item_database::instance().get_hash(),
	        "www.growtopia1.com", "0099/01123/cache/", //CDN url & path goes there
	        "www.growtopia1.com", "proto=47|clash_active=0|enableInventoryTab=0|bigBackpack=0|enable_tabs=0|", //don't change cuz its not neccessary
	        0 //<-- replace 0 with tribute data's hash
	    }, -1, 0);
	}
}